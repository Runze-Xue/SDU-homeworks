import os
import math
import time
import torch
import torch.nn as nn
import torch.optim as optim
import random
import spacy
import jieba
import csv 
import argparse
import re

from nltk.translate.bleu_score import corpus_bleu, SmoothingFunction

from torch.utils.data import Dataset, DataLoader
from torchtext.vocab import build_vocab_from_iterator
from torch.nn.utils.rnn import pad_sequence

from constrains import (N_EPOCHS, ENC_EMB_DIM, ENC_HID_DIM, DEC_HID_DIM,
                        ENC_DROPOUT, DEC_EMB_DIM, DEC_DROPOUT, device, SEED,
                        ORIGINAL_TRAIN_FILE, PROCESSED_TRAIN_FILE, ORIGINAL_VALID_FILE, PROCESSED_VALID_FILE,
                        ORIGINAL_TEST_FILE, PROCESSED_TEST_FILE, BATCH_SIZE, CLIP
                        )
from train import train, evaluate, epoch_time 
from model import Seq2Seq, Attention, Encoder, Decoder 
from data_process import (preprocess_to_tsv, load_sentences) 

parser = argparse.ArgumentParser()

parser.add_argument('-m', '--model_path',type=str, default=None)
parser.add_argument('-t','--test_model', type=str, default=None)
args = parser.parse_args()

os.environ["TMP"] = r"temp"
os.environ["TEMP"] = r"temp"

random.seed(SEED)
torch.manual_seed(SEED)
if torch.cuda.is_available():
    torch.cuda.manual_seed(SEED)
    torch.backends.cudnn.deterministic = True
    torch.backends.cudnn.benchmark = False

print(f"Using device: {device}")

print('Data process: ')
preprocess_to_tsv(ORIGINAL_TRAIN_FILE, PROCESSED_TRAIN_FILE)
preprocess_to_tsv(ORIGINAL_TEST_FILE, PROCESSED_TEST_FILE)

spacy_en = spacy.load('en_core_web_sm')

def tokenize_en(text):
    return [tok.text.lower() for tok in spacy_en.tokenizer(text)][::-1]

def tokenize_zh(text):
    return [tok.lower() for tok in jieba.cut(text)]

UNK_TOKEN, PAD_TOKEN, SOS_TOKEN, EOS_TOKEN = '<unk>', '<pad>', '<sos>', '<eos>'
special_symbols = [UNK_TOKEN, PAD_TOKEN, SOS_TOKEN, EOS_TOKEN]

def yield_tokens_from_tsv(file_path, tokenizer_fn, column_index):
    with open(file_path, 'r', encoding='utf-8') as f:
        reader = csv.reader(f, delimiter='\t')
        for row in reader:
            if row:
                yield tokenizer_fn(row[column_index])

print("Building source vocabulary...")
SRC_VOCAB = build_vocab_from_iterator(
    yield_tokens_from_tsv(PROCESSED_TRAIN_FILE, tokenize_en, 0),
    min_freq=1,
    specials=special_symbols,
    special_first=True
)
SRC_VOCAB.set_default_index(SRC_VOCAB[UNK_TOKEN])
SRC_PAD_IDX = SRC_VOCAB[PAD_TOKEN]
SRC_SOS_IDX = SRC_VOCAB[SOS_TOKEN]
SRC_EOS_IDX = SRC_VOCAB[EOS_TOKEN]

print("Building target vocabulary...")
TRG_VOCAB = build_vocab_from_iterator(
    yield_tokens_from_tsv(PROCESSED_TRAIN_FILE, tokenize_zh, 1),
    min_freq=1,
    specials=special_symbols,
    special_first=True
)
TRG_VOCAB.set_default_index(TRG_VOCAB[UNK_TOKEN])
TRG_PAD_IDX = TRG_VOCAB[PAD_TOKEN]
TRG_SOS_IDX = TRG_VOCAB[SOS_TOKEN]
TRG_EOS_IDX = TRG_VOCAB[EOS_TOKEN]

print(f"Unique tokens in source (en) vocabulary: {len(SRC_VOCAB)}")
print(f"Unique tokens in target (zh) vocabulary: {len(TRG_VOCAB)}")

class TranslationDataset(Dataset):
    def __init__(self, tsv_file_path, src_tokenizer, trg_tokenizer, src_vocab, trg_vocab):
        self.src_tokenizer = src_tokenizer
        self.trg_tokenizer = trg_tokenizer
        self.src_vocab = src_vocab
        self.trg_vocab = trg_vocab
        self.data = []
        self.raw_src_texts = []
        self.raw_trg_texts_tokenized = []

        with open(tsv_file_path, 'r', encoding='utf-8') as f:
            reader = csv.reader(f, delimiter='\t')
            for row in reader:
                if len(row) == 2:
                    src_text_raw, trg_text_raw = row[0], row[1]
                    self.data.append((src_text_raw, trg_text_raw))
                    self.raw_src_texts.append(src_text_raw) 
                    self.raw_trg_texts_tokenized.append(self.trg_tokenizer(trg_text_raw))


    def __len__(self):
        return len(self.data)

    def __getitem__(self, idx):
        src_text, trg_text = self.data[idx]
        src_tokens_numericalized = [SRC_SOS_IDX] + [self.src_vocab[token] for token in self.src_tokenizer(src_text)] + [SRC_EOS_IDX]
        trg_tokens_numericalized = [TRG_SOS_IDX] + [self.trg_vocab[token] for token in self.trg_tokenizer(trg_text)] + [TRG_EOS_IDX]
        return torch.tensor(src_tokens_numericalized, dtype=torch.long), torch.tensor(trg_tokens_numericalized, dtype=torch.long)

def collate_batch(batch):
    src_list, trg_list = [], []
    for (src_item, trg_item) in batch:
        src_list.append(src_item)
        trg_list.append(trg_item)
    src_padded = pad_sequence(src_list, batch_first=True, padding_value=SRC_PAD_IDX)
    trg_padded = pad_sequence(trg_list, batch_first=True, padding_value=TRG_PAD_IDX)
    return src_padded.to(device), trg_padded.to(device)

train_dataset = TranslationDataset(PROCESSED_TRAIN_FILE, tokenize_en, tokenize_zh, SRC_VOCAB, TRG_VOCAB)
valid_dataset = TranslationDataset(PROCESSED_VALID_FILE, tokenize_en, tokenize_zh, SRC_VOCAB, TRG_VOCAB)
test_dataset = TranslationDataset(PROCESSED_VALID_FILE, tokenize_en, tokenize_zh, SRC_VOCAB, TRG_VOCAB)

print(f"Number of training examples: {len(train_dataset)}")
print(f"Number of validation examples: {len(valid_dataset)}")
print(f"Number of testing examples: {len(test_dataset)}")

train_iterator = DataLoader(train_dataset, batch_size=BATCH_SIZE,
                            shuffle=True, collate_fn=collate_batch)
valid_iterator = DataLoader(valid_dataset, batch_size=BATCH_SIZE,
                            shuffle=False, collate_fn=collate_batch)
test_iterator  = DataLoader(test_dataset, batch_size=BATCH_SIZE,
                        shuffle=False, collate_fn=collate_batch)

print("Start model build init... ")
INPUT_DIM = len(SRC_VOCAB)
OUTPUT_DIM = len(TRG_VOCAB)
attn = Attention(ENC_HID_DIM, DEC_HID_DIM)
enc = Encoder(INPUT_DIM, ENC_EMB_DIM, ENC_HID_DIM, DEC_HID_DIM, ENC_DROPOUT)
dec = Decoder(OUTPUT_DIM, DEC_EMB_DIM, ENC_HID_DIM, DEC_HID_DIM, DEC_DROPOUT, attn)
model = Seq2Seq(enc, dec, SRC_PAD_IDX, device).to(device)

def init_weights(m):
    for name, param in m.named_parameters():
        if 'weight' in name:
            nn.init.normal_(param.data, mean=0, std=0.01)
        else:
            nn.init.constant_(param.data, 0)

optimizer = optim.Adam(model.parameters())
criterion = nn.CrossEntropyLoss(ignore_index=TRG_PAD_IDX)

best_valid_loss = float('inf')

if args.model_path:
    print(f"Loading model from: {args.model_path}")
    model.load_state_dict(torch.load(args.model_path, map_location=device))
    print("Model state_dict loaded successfully.")
    filename = os.path.basename(args.model_path)
    epoch_match = re.search(r'epoch-(\d+)', filename)
    if epoch_match:
        start_epoch = int(epoch_match.group(1))
        print(f"Inferred start epoch from filename: {start_epoch}. Will continue from epoch {start_epoch + 1}.")
    else:
        print("Could not infer start epoch from filename. Starting from epoch 0 for loaded model.")
        start_epoch = 0
else:
    print("No pre-trained model specified. Initializing weights randomly.")
    model.apply(init_weights)

if args.test_model is None:
    print("Starting training...")
    effective_start_epoch = start_epoch + 1 if args.model_path and epoch_match else 0
    for epoch_idx in range(effective_start_epoch, N_EPOCHS):
        epoch = epoch_idx + 1 
        start_time = time.time()
        train_loss = train(model, train_iterator, optimizer, criterion, CLIP)
        end_time = time.time()
        epoch_mins, epoch_secs = epoch_time(start_time, end_time)
        torch.save(model.state_dict(), f'0602-model-epoch-{epoch}.pt')
        print(f"\tModel saved for epoch {epoch}.")
        print(f'Epoch: {epoch:02} | Time: {epoch_mins}m {epoch_secs}s')
        print(f'\tTrain Loss: {train_loss:.3f} | Train PPL: {math.exp(train_loss):7.3f}')

def translate_sentence_new(sentence_str, src_tokenizer, src_vocab, trg_vocab, model, device, max_len=50):
    model.eval()
    if isinstance(sentence_str, str):
        tokens = src_tokenizer(sentence_str)
    else:
        tokens = [token.lower() for token in sentence_str]

    src_indexes = [SRC_SOS_IDX] + [src_vocab.get_stoi().get(token, SRC_VOCAB[UNK_TOKEN]) for token in tokens] + [SRC_EOS_IDX]
    src_tensor = torch.LongTensor(src_indexes).unsqueeze(0).to(device)
    src_mask_translate = (src_tensor != SRC_PAD_IDX).to(device)

    with torch.no_grad():
        encoder_outputs, hidden = model.encoder(src_tensor)

    trg_indexes = [TRG_SOS_IDX]
    for i in range(max_len):
        trg_tensor = torch.LongTensor([trg_indexes[-1]]).to(device)
        with torch.no_grad():
            output, hidden = model.decoder(trg_tensor, hidden, encoder_outputs, src_mask_translate) # Pass src_mask
        pred_token_idx = output.argmax(1).item()
        trg_indexes.append(pred_token_idx)
        if pred_token_idx == TRG_EOS_IDX:
            break
    trg_tokens = [trg_vocab.get_itos()[i] for i in trg_indexes]
    return trg_tokens[1:-1] if trg_tokens[-1] == EOS_TOKEN else trg_tokens[1:]

def calculate_bleu_score(dataset, model, src_tokenizer, src_vocab, trg_vocab, device, max_len=50):
    targets_tokenized = [] 
    predictions_tokenized = [] 
    model.eval()
    for i in range(len(dataset)):
        src_text_raw = dataset.raw_src_texts[i]
        trg_tokens_raw = dataset.raw_trg_texts_tokenized[i]
        prediction_tokens = translate_sentence_new(
            src_text_raw,
            src_tokenizer,
            src_vocab,
            trg_vocab,
            model,
            device,
            max_len
        )

        predictions_tokenized.append(prediction_tokens)
        targets_tokenized.append([trg_tokens_raw]) 

        if i < 5: 
             print(f"Source: {src_text_raw}")
             print(f"Target: {' '.join(trg_tokens_raw)}")
             print(f"Predicted: {' '.join(prediction_tokens)}\n")
    chencherry = SmoothingFunction()
    bleu = corpus_bleu(targets_tokenized, predictions_tokenized, smoothing_function=chencherry.method4)
    return bleu * 100 

if args.test_model is not None:
    print(f"Loading model for testing from: {args.test_model}")
    model.load_state_dict(torch.load(args.test_model, map_location=device))
    print("Model loaded successfully for testing.")

    test_loss = evaluate(model, test_iterator, criterion)
    print(f'| Test Loss: {test_loss:.3f} | Test PPL: {math.exp(test_loss):7.3f} |')
    print("\nCalculating BLEU score on the test set...")
    bleu_score_test = calculate_bleu_score(
        test_dataset,
        model,
        tokenize_en,
        SRC_VOCAB,
        TRG_VOCAB,
        device
    )
    print(f"Corpus BLEU score on Test Set: {bleu_score_test:.2f}")
