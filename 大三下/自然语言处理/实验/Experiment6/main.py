import os
import math
import time
import torch
import torch.nn as nn
import torch.optim as optim
import random
import spacy
import jieba
import csv # For reading TSV files
import argparse

from torch.utils.data import Dataset, DataLoader
from torchtext.vocab import build_vocab_from_iterator
from torch.nn.utils.rnn import pad_sequence

from constrains import (N_EPOCHS, ENC_EMB_DIM, ENC_HID_DIM, DEC_HID_DIM,
                        ENC_DROPOUT, DEC_EMB_DIM, DEC_DROPOUT, device, SEED,
                        ORIGINAL_TRAIN_FILE, PROCESSED_TRAIN_FILE, ORIGINAL_VALID_FILE, PROCESSED_VALID_FILE,
                        ORIGINAL_TEST_FILE, PROCESSED_TEST_FILE, BATCH_SIZE, CLIP
                        )
from train import train, evaluate, epoch_time # Assuming train(model, iterator, optimizer, criterion, clip)
from model import Seq2Seq, Attention, Encoder, Decoder # Assuming these classes are defined
from data_process import (preprocess_to_tsv) # Assuming this function is defined
# from test_tools import translate_sentence, calculate_bleu # We'll need to adapt this later


parser = argparse.ArgumentParser()
parser.add_argument('-v', '--version', type=str, default='train')
args = parser.parse_args()

os.environ["TMP"] = r"temp" # These might not be necessary depending on your system/spacy needs
os.environ["TEMP"] = r"temp"

random.seed(SEED)
torch.manual_seed(SEED)
if torch.cuda.is_available():
    torch.cuda.manual_seed(SEED)
    torch.backends.cudnn.deterministic = True
    torch.backends.cudnn.benchmark = False # For full determinism

print(f"Using device: {device}")

# --- Data Preprocessing (if not already done) ---
print('Data process: ')
preprocess_to_tsv(ORIGINAL_TRAIN_FILE, PROCESSED_TRAIN_FILE)
preprocess_to_tsv(ORIGINAL_VALID_FILE, PROCESSED_VALID_FILE)
preprocess_to_tsv(ORIGINAL_TEST_FILE, PROCESSED_TEST_FILE)

# Assuming PROCESSED_TEST_FILE is also created by a similar call if it's from an original file.
# --- Tokenizers ---
spacy_en = spacy.load('en_core_web_sm')

def tokenize_en(text):
    """Tokenizes English text, lowercases, and reverses it."""
    return [tok.text.lower() for tok in spacy_en.tokenizer(text)][::-1]

def tokenize_zh(text):
    """Tokenizes Chinese text and lowercases it."""
    return [tok.lower() for tok in jieba.cut(text)]

# --- Vocabulary Building ---
UNK_TOKEN, PAD_TOKEN, SOS_TOKEN, EOS_TOKEN = '<unk>', '<pad>', '<sos>', '<eos>'
special_symbols = [UNK_TOKEN, PAD_TOKEN, SOS_TOKEN, EOS_TOKEN]

# Helper to yield tokens from TSV file for vocabulary building
def yield_tokens_from_tsv(file_path, tokenizer_fn, column_index):
    with open(file_path, 'r', encoding='utf-8') as f:
        reader = csv.reader(f, delimiter='\t')
        for row in reader:
            if row: # Ensure row is not empty
                yield tokenizer_fn(row[column_index])

print("Building source vocabulary...")
SRC_VOCAB = build_vocab_from_iterator(
    yield_tokens_from_tsv(PROCESSED_TRAIN_FILE, tokenize_en, 0), # Assuming src is column 0
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

# --- Custom Dataset ---
class TranslationDataset(Dataset):
    def __init__(self, tsv_file_path, src_tokenizer, trg_tokenizer, src_vocab, trg_vocab):
        self.src_tokenizer = src_tokenizer
        self.trg_tokenizer = trg_tokenizer
        self.src_vocab = src_vocab
        self.trg_vocab = trg_vocab
        self.data = []
        with open(tsv_file_path, 'r', encoding='utf-8') as f:
            reader = csv.reader(f, delimiter='\t')
            for row in reader:
                if len(row) == 2:
                    self.data.append((row[0], row[1]))

    def __len__(self):
        return len(self.data)
    def __getitem__(self, idx):
        src_text, trg_text = self.data[idx]
        src_tokens = [SRC_SOS_IDX] + [self.src_vocab[token] for token in self.src_tokenizer(src_text)] + [SRC_EOS_IDX]
        trg_tokens = [TRG_SOS_IDX] + [self.trg_vocab[token] for token in self.trg_tokenizer(trg_text)] + [TRG_EOS_IDX]
        return torch.tensor(src_tokens, dtype=torch.long), torch.tensor(trg_tokens, dtype=torch.long)

# --- Collate Function for DataLoader ---
def collate_batch(batch):
    src_list, trg_list = [], []
    for (src_item, trg_item) in batch:
        src_list.append(src_item)
        trg_list.append(trg_item)

    # pad_sequence expects a list of Tensors
    # batch_first=True makes the output (batch_size, seq_len)
    src_padded = pad_sequence(src_list, batch_first=True, padding_value=SRC_PAD_IDX)
    trg_padded = pad_sequence(trg_list, batch_first=True, padding_value=TRG_PAD_IDX)

    return src_padded.to(device), trg_padded.to(device)


# --- Create Datasets and DataLoaders ---
train_dataset = TranslationDataset(PROCESSED_TRAIN_FILE, tokenize_en, tokenize_zh, SRC_VOCAB, TRG_VOCAB)
valid_dataset = TranslationDataset(PROCESSED_VALID_FILE, tokenize_en, tokenize_zh, SRC_VOCAB, TRG_VOCAB)
test_dataset  = TranslationDataset(PROCESSED_TEST_FILE, tokenize_en, tokenize_zh, SRC_VOCAB, TRG_VOCAB)

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

attn = Attention(ENC_HID_DIM, DEC_HID_DIM) # Assuming Attention doesn't need dropout here
enc = Encoder(INPUT_DIM, ENC_EMB_DIM, ENC_HID_DIM, DEC_HID_DIM, ENC_DROPOUT)
dec = Decoder(OUTPUT_DIM, DEC_EMB_DIM, ENC_HID_DIM, DEC_HID_DIM, DEC_DROPOUT, attn)

model = Seq2Seq(enc, dec, SRC_PAD_IDX, device).to(device)

def init_weights(m):
    for name, param in m.named_parameters():
        if 'weight' in name:
            nn.init.normal_(param.data, mean=0, std=0.01)
        else:
            nn.init.constant_(param.data, 0)

model.apply(init_weights)
print(f'The model has {sum(p.numel() for p in model.parameters() if p.requires_grad):,} trainable parameters')

optimizer = optim.Adam(model.parameters())
criterion = nn.CrossEntropyLoss(ignore_index=TRG_PAD_IDX)


best_valid_loss = float('inf')
if args.version == 'train':
    print("Starting training...")
    for epoch in range(N_EPOCHS):
        start_time = time.time()

        train_loss = train(model, train_iterator, optimizer, criterion, CLIP)
        valid_loss = evaluate(model, valid_iterator, criterion)

        end_time = time.time()
        epoch_mins, epoch_secs = epoch_time(start_time, end_time)

        if valid_loss < best_valid_loss:
            best_valid_loss = valid_loss
            torch.save(model.state_dict(), f'best-model-epoch-{epoch+1}.pt')
            print(f"\tBest validation loss: {best_valid_loss:.3f}. Model saved.")

        print(f'Epoch: {epoch+1:02} | Time: {epoch_mins}m {epoch_secs}s')
        print(f'\tTrain Loss: {train_loss:.3f} | Train PPL: {math.exp(train_loss):7.3f}')
        print(f'\t Val. Loss: {valid_loss:.3f} |  Val. PPL: {math.exp(valid_loss):7.3f}')

# --- Testing / Inference ---
# You'll need to adapt `translate_sentence` and `calculate_bleu` as well.
# `translate_sentence` will now need SRC_VOCAB, TRG_VOCAB, tokenizers, and special token indices.

# Example: Load best model and evaluate on test set
def translate_sentence_new(sentence_str, src_tokenizer, src_vocab, trg_vocab, model, device, max_len=50):
    model.eval()
    if isinstance(sentence_str, str):
        tokens = src_tokenizer(sentence_str)
    else: # Assuming it's already a list of tokens
        tokens = [token.lower() for token in sentence_str] # Apply lower if not done by tokenizer
        tokens = tokens[::-1] # Reverse if it was part of original tokenize_en

    src_indexes = [SRC_SOS_IDX] + [src_vocab[token] for token in tokens] + [SRC_EOS_IDX]
    src_tensor = torch.LongTensor(src_indexes).unsqueeze(0).to(device) # Add batch dimension

    with torch.no_grad():
        encoder_outputs, hidden = model.encoder(src_tensor) # Assuming model.encoder exists

    # For decoder, start with <sos> token
    trg_indexes = [TRG_SOS_IDX]
    attentions = torch.zeros(max_len, 1, len(src_indexes)) # For storing attention weights

    for i in range(max_len):
        trg_tensor = torch.LongTensor([trg_indexes[-1]]).to(device) # Last predicted token

        with torch.no_grad():
            # Decoder might need hidden and cell from encoder, or just hidden
            # And encoder_outputs for attention
            # This depends on your exact model.decoder interface
            src_mask_translate = (src_tensor != SRC_PAD_IDX) # SRC_PAD_IDX 也需要可访问
            output, hidden = model.decoder(trg_tensor, hidden, encoder_outputs, src_mask_translate)
            # attentions[i] = attention_weights # Store attention

        pred_token_idx = output.argmax(1).item()
        trg_indexes.append(pred_token_idx)

        if pred_token_idx == TRG_EOS_IDX:
            break

    trg_tokens = [trg_vocab.get_itos()[i] for i in trg_indexes]
    # Remove <sos> and <eos> if present at ends
    return trg_tokens[1:-1] if trg_tokens[-1] == EOS_TOKEN else trg_tokens[1:]

if args.version == 'test':
    model.load_state_dict(torch.load('best-model-epoch-1.pt')) # Load your best model
    test_loss = evaluate(model, test_iterator, criterion)
    # print(f'| Test Loss: {test_loss:.3f} | Test PPL: {math.exp(test_loss):7.3f} |')

    # Example of how translate_sentence might need to change (conceptual):

    # After training, you can try translating a sentence:
    model.load_state_dict(torch.load('best-model-epoch-1.pt')) # Load your best saved model
    example_sentence = "a cat sat on the mat ."
    translation = translate_sentence_new(example_sentence, tokenize_en, SRC_VOCAB, TRG_VOCAB, model, device)
    print(f"'{example_sentence}' -> '{' '.join(translation)}'")
