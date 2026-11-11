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
import re # For parsing epoch from filename

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
from data_process import (preprocess_to_tsv, load_sentences) # Assuming this function is defined

parser = argparse.ArgumentParser()
parser.add_argument('-v', '--version', type=str, default='train', choices=['train', 'test'],
                    help="Mode: 'train' or 'test'")
parser.add_argument('-m', '--model_path', type=str, default=None, # Renamed for clarity
                    help="Path to a pre-trained model.pth file to continue training or for testing.")
parser.add_argument('-o', '--optimizer_path', type=str, default=None, # Optional: path to optimizer state
                    help="Path to a pre-trained optimizer.pth file to continue training.")
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

# --- Data Preprocessing ---
print('Data process: ')
preprocess_to_tsv(ORIGINAL_TRAIN_FILE, PROCESSED_TRAIN_FILE)
preprocess_to_tsv(ORIGINAL_VALID_FILE, PROCESSED_VALID_FILE)
preprocess_to_tsv(ORIGINAL_TEST_FILE, PROCESSED_TEST_FILE)

# --- Tokenizers ---
spacy_en = spacy.load('en_core_web_sm')

def tokenize_en(text):
    return [tok.text.lower() for tok in spacy_en.tokenizer(text)][::-1]

def tokenize_zh(text):
    return [tok.lower() for tok in jieba.cut(text)]

# --- Vocabulary Building ---
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
    min_freq=1, specials=special_symbols, special_first=True)
SRC_VOCAB.set_default_index(SRC_VOCAB[UNK_TOKEN])
SRC_PAD_IDX = SRC_VOCAB[PAD_TOKEN]
SRC_SOS_IDX = SRC_VOCAB[SOS_TOKEN]
SRC_EOS_IDX = SRC_VOCAB[EOS_TOKEN]

print("Building target vocabulary...")
TRG_VOCAB = build_vocab_from_iterator(
    yield_tokens_from_tsv(PROCESSED_TRAIN_FILE, tokenize_zh, 1),
    min_freq=1, specials=special_symbols, special_first=True)
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

# --- Collate Function ---
def collate_batch(batch):
    src_list, trg_list = [], []
    for (src_item, trg_item) in batch:
        src_list.append(src_item)
        trg_list.append(trg_item)
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

train_iterator = DataLoader(train_dataset, batch_size=BATCH_SIZE, shuffle=True, collate_fn=collate_batch)
valid_iterator = DataLoader(valid_dataset, batch_size=BATCH_SIZE, shuffle=False, collate_fn=collate_batch)
test_iterator  = DataLoader(test_dataset, batch_size=BATCH_SIZE, shuffle=False, collate_fn=collate_batch)

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

# --- Model Initialization and Loading ---
start_epoch = 0
best_valid_loss = float('inf')

if args.model_path: # If a model path is provided
    print(f"Loading model from: {args.model_path}")
    try:
        # --- Option 1: Load model and optimizer state from a single checkpoint dictionary ---
        # checkpoint = torch.load(args.model_path, map_location=device)
        # model.load_state_dict(checkpoint['model_state_dict'])
        # optimizer.load_state_dict(checkpoint['optimizer_state_dict']) # Optimizer must be defined before this
        # start_epoch = checkpoint.get('epoch', 0) # .get() to avoid KeyError if 'epoch' is not saved
        # best_valid_loss = checkpoint.get('best_valid_loss', float('inf'))
        # print(f"Resuming training from epoch {start_epoch + 1}, best_valid_loss: {best_valid_loss:.3f}")

        # --- Option 2: Load only model state_dict (as in your original code) ---
        model.load_state_dict(torch.load(args.model_path, map_location=device))
        print("Model state_dict loaded successfully.")
        # Try to infer start_epoch and best_valid_loss if they were part of the filename
        # This is a bit fragile and depends on your naming convention
        filename = os.path.basename(args.model_path)
        epoch_match = re.search(r'epoch-(\d+)', filename)
        if epoch_match:
            start_epoch = int(epoch_match.group(1)) # Start from the next epoch
            print(f"Inferred start epoch from filename: {start_epoch}. Will continue from epoch {start_epoch + 1}.")
        else:
            print("Could not infer start epoch from filename. Starting from epoch 0 for loaded model.")
            start_epoch = 0 # Or you might want to prompt the user or set it differently

        # If you saved best_valid_loss separately or can infer it, load it here.
        # For simplicity, if loading a model, we might assume we want to find a new best loss
        # or you'd need a more robust way to save/load `best_valid_loss`.
        # If you have a separate file for best_valid_loss or it's in a checkpoint dict, load it.
        # For now, we'll keep best_valid_loss as inf, or you can load a previous one.
        # If you load a model, you might want to re-evaluate it to get its valid_loss.
        # print("Evaluating loaded model on validation set to get current best_valid_loss...")
        # loaded_model_valid_loss = evaluate(model, valid_iterator, criterion)
        # best_valid_loss = loaded_model_valid_loss # Or min(best_valid_loss, loaded_model_valid_loss)
        # print(f"Validation loss of loaded model: {best_valid_loss:.3f}")

    except FileNotFoundError:
        print(f"Error: Model file not found at {args.model_path}. Starting from scratch.")
        model.apply(init_weights) # Initialize if file not found
    except Exception as e:
        print(f"Error loading model: {e}. Starting from scratch.")
        model.apply(init_weights) # Initialize on other errors
else: # No model_path provided, initialize weights
    print("No pre-trained model specified. Initializing weights randomly.")
    model.apply(init_weights)

print(f'The model has {sum(p.numel() for p in model.parameters() if p.requires_grad):,} trainable parameters')

optimizer = optim.Adam(model.parameters())
criterion = nn.CrossEntropyLoss(ignore_index=TRG_PAD_IDX)

# --- Optional: Load optimizer state if provided and we loaded a model ---
if args.model_path and args.optimizer_path: # Only if model was loaded and optimizer path is given
    print(f"Loading optimizer state from: {args.optimizer_path}")
    try:
        optimizer.load_state_dict(torch.load(args.optimizer_path, map_location=device))
        print("Optimizer state loaded successfully.")
        # If you've loaded an optimizer, you might need to move its state to the correct device
        # This is often handled automatically if optimizer was created after model.to(device)
        # but good to be aware:
        for state in optimizer.state.values():
            for k, v in state.items():
                if isinstance(v, torch.Tensor):
                    state[k] = v.to(device)
    except FileNotFoundError:
        print(f"Warning: Optimizer file not found at {args.optimizer_path}. Using new optimizer.")
    except Exception as e:
        print(f"Warning: Error loading optimizer state: {e}. Using new optimizer.")


if args.version == 'train':
    print(f"Starting training from epoch {start_epoch + 1}...")
    for epoch in range(start_epoch, N_EPOCHS): # Adjust loop to start from `start_epoch`
        current_epoch_display = epoch + 1 # For display purposes
        start_time = time.time()

        train_loss = train(model, train_iterator, optimizer, criterion, CLIP)
        valid_loss = evaluate(model, valid_iterator, criterion)

        end_time = time.time()
        epoch_mins, epoch_secs = epoch_time(start_time, end_time)

        if valid_loss < best_valid_loss:
            best_valid_loss = valid_loss
            # Save model and optimizer state together for easier resumption
            checkpoint = {
                'epoch': current_epoch_display, # Save the epoch number that just finished
                'model_state_dict': model.state_dict(),
                'optimizer_state_dict': optimizer.state_dict(),
                'best_valid_loss': best_valid_loss,
                # You can add SRC_VOCAB, TRG_VOCAB if they are not rebuilt every time
                # 'src_vocab': SRC_VOCAB,
                # 'trg_vocab': TRG_VOCAB,
            }
            model_save_path = f'best-model-epoch-{current_epoch_display}.pt'
            torch.save(checkpoint, model_save_path)
            print(f"\tBest validation loss: {best_valid_loss:.3f}. Model checkpoint saved to {model_save_path}")
        else:
            # Optionally save a checkpoint नागरिक every N epochs even if not best
            if (epoch + 1) % 5 == 0: # Save every 5 epochs for example
                checkpoint = {
                    'epoch': current_epoch_display,
                    'model_state_dict': model.state_dict(),
                    'optimizer_state_dict': optimizer.state_dict(),
                    'best_valid_loss': best_valid_loss, # Still save the current best
                }
                torch.save(checkpoint, f'checkpoint-epoch-{current_epoch_display}.pt')
                print(f"Checkpoint saved for epoch {current_epoch_display}")


        print(f'Epoch: {current_epoch_display:02} | Time: {epoch_mins}m {epoch_secs}s')
        print(f'\tTrain Loss: {train_loss:.3f} | Train PPL: {math.exp(train_loss):7.3f}')
        print(f'\t Val. Loss: {valid_loss:.3f} |  Val. PPL: {math.exp(valid_loss):7.3f}')

# --- Testing / Inference ---
def translate_sentence_new(sentence_str, src_tokenizer, src_vocab, trg_vocab, model, device, max_len=50):
    # ... (translate_sentence_new implementation as before)
    model.eval()
    if isinstance(sentence_str, str):
        tokens = src_tokenizer(sentence_str)
    else: # Assuming it's already a list of tokens
        tokens = [token.lower() for token in sentence_str] # Apply lower if not done by tokenizer
        # tokens = tokens[::-1] # Reverse if it was part of original tokenize_en -> handled by tokenize_en

    # Ensure SRC_SOS_IDX, SRC_EOS_IDX, TRG_SOS_IDX, TRG_EOS_IDX, SRC_PAD_IDX are accessible
    # (defined globally or passed as params)
    src_indexes = [SRC_SOS_IDX] + [src_vocab.get_stoi().get(token, src_vocab.get_stoi()[UNK_TOKEN]) for token in tokens] + [SRC_EOS_IDX]
    src_tensor = torch.LongTensor(src_indexes).unsqueeze(0).to(device)

    with torch.no_grad():
        encoder_outputs, hidden = model.encoder(src_tensor)

    trg_indexes = [TRG_SOS_IDX]
    # attentions = torch.zeros(max_len, 1, len(src_indexes))

    for i in range(max_len):
        trg_tensor = torch.LongTensor([trg_indexes[-1]]).to(device)
        src_mask_translate = (src_tensor != SRC_PAD_IDX)
        output, hidden = model.decoder(trg_tensor, hidden, encoder_outputs, src_mask_translate)
        pred_token_idx = output.argmax(1).item()
        trg_indexes.append(pred_token_idx)
        if pred_token_idx == TRG_EOS_IDX:
            break
    # Ensure trg_vocab.get_itos() is the correct way to access index-to-string
    trg_tokens = [trg_vocab.get_itos()[i] for i in trg_indexes]
    return trg_tokens[1:-1] if len(trg_tokens) > 1 and trg_tokens[-1] == trg_vocab.get_itos()[TRG_EOS_IDX] else trg_tokens[1:]


if args.version == 'test':
    if args.model_path:
        print(f"Loading model for testing from: {args.model_path}")
        try:
            # If saved as checkpoint dictionary
            checkpoint = torch.load(args.model_path, map_location=device)
            model.load_state_dict(checkpoint['model_state_dict'])
            # If just model.state_dict() was saved:
            # model.load_state_dict(torch.load(args.model_path, map_location=device))
            print("Model loaded successfully for testing.")
        except FileNotFoundError:
            print(f"Error: Model file not found at {args.model_path}. Cannot perform test.")
            exit()
        except Exception as e:
            print(f"Error loading model for testing: {e}. Cannot perform test.")
            exit()
    else:
        print("Error: No model path specified for testing. Use -m <path_to_model.pt>")
        exit()

    test_loss = evaluate(model, test_iterator, criterion)
    print(f'| Test Loss: {test_loss:.3f} | Test PPL: {math.exp(test_loss):7.3f} |')

    test_sentence_list = load_sentences(PROCESSED_TEST_FILE) # Assuming load_sentences reads from processed TSV
    # Or if load_sentences expects raw txt: load_sentences(ORIGINAL_TEST_FILE)
    # For now, let's assume it reads the source sentences from the processed TSV
    source_sentences_for_translation = []
    with open(PROCESSED_TEST_FILE, 'r', encoding='utf-8') as f:
        reader = csv.reader(f, delimiter='\t')
        for row in reader:
            if len(row) >= 1:
                source_sentences_for_translation.append(row[0]) # Assuming source is column 0

    print("\n--- Example Translations ---")
    for i, example_sentence_str in enumerate(source_sentences_for_translation[:5]): # Translate first 5 test sentences
        translation = translate_sentence_new(example_sentence_str, tokenize_en, SRC_VOCAB, TRG_VOCAB, model, device)
        print(f"Original ({i+1}): '{example_sentence_str}'")
        print(f"Translated ({i+1}): '{' '.join(translation)}'\n")