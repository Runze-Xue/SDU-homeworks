import torch
import torch.optim as optim
from torch.utils.data import DataLoader
from transformers import BertTokenizerFast, get_linear_schedule_with_warmup
from seqeval.metrics import f1_score, precision_score, recall_score, classification_report

from data_loader import load_data, get_tags, NerDataset 
from model import BertBiLSTMCRF 

BERT_MODEL_NAME = 'bert-base-chinese' 
TRAIN_FILE = 'train.txt'
DEV_FILE = 'dev.txt'

MAX_LEN = 128
BATCH_SIZE = 16 
EPOCHS = 5 
LEARNING_RATE = 3e-5 
LSTM_HIDDEN_DIM = 256
LSTM_LAYERS = 1
DROPOUT_RATE = 0.1
WEIGHT_DECAY = 0.01 

DEVICE = torch.device("cuda" if torch.cuda.is_available() else "cpu")
print(f"Using device: {DEVICE}")
tokenizer = BertTokenizerFast.from_pretrained(BERT_MODEL_NAME)

train_sentences = load_data(TRAIN_FILE)
dev_sentences = load_data(DEV_FILE)

all_sentences_for_tags = train_sentences + dev_sentences
tag_to_idx, idx_to_tag, PAD_TAG_ID = get_tags(all_sentences_for_tags)
NUM_TAGS = len(tag_to_idx)

print(f"Number of tags: {NUM_TAGS}")
print("Tag to ID mapping:", tag_to_idx)
print("Pad Tag ID for labels (used for subwords/padding):", PAD_TAG_ID, f"({idx_to_tag[PAD_TAG_ID]})")


train_dataset = NerDataset(train_sentences, tokenizer, tag_to_idx, MAX_LEN, PAD_TAG_ID)
dev_dataset = NerDataset(dev_sentences, tokenizer, tag_to_idx, MAX_LEN, PAD_TAG_ID)

train_dataloader = DataLoader(train_dataset, batch_size=BATCH_SIZE, shuffle=True)
dev_dataloader = DataLoader(dev_dataset, batch_size=BATCH_SIZE)

model = BertBiLSTMCRF(
    bert_model_name=BERT_MODEL_NAME,
    num_tags=NUM_TAGS,
    lstm_hidden_dim=LSTM_HIDDEN_DIM,
    lstm_layers=LSTM_LAYERS,
    dropout_rate=DROPOUT_RATE
)
model.to(DEVICE)

optimizer = optim.AdamW(model.parameters(), lr=LEARNING_RATE, weight_decay=WEIGHT_DECAY)

total_steps = len(train_dataloader) * EPOCHS
scheduler = get_linear_schedule_with_warmup(
    optimizer,
    num_warmup_steps=0, 
    num_training_steps=total_steps
)

def train_epoch(model, data_loader, optimizer, scheduler, device):
    model.train()
    total_loss = 0
    for batch_idx, batch in enumerate(data_loader):
        input_ids = batch['input_ids'].to(device)
        attention_mask = batch['attention_mask'].to(device)
        labels = batch['labels'].to(device)

        optimizer.zero_grad()
        loss = model(input_ids, attention_mask, labels)
        
        loss.backward()
        torch.nn.utils.clip_grad_norm_(model.parameters(), 1.0) 
        optimizer.step()
        scheduler.step()
        
        total_loss += loss.item()
        
        if (batch_idx + 1) % 5 == 0: 
             print(f"  Batch {batch_idx+1}/{len(data_loader)}, Loss: {loss.item():.4f}")

    return total_loss / len(data_loader)

def evaluate(model, data_loader, device, idx_to_tag, pad_tag_id):
    model.eval()
    all_preds_str = []
    all_labels_str = []
    
    with torch.no_grad():
        for batch in data_loader:
            input_ids = batch['input_ids'].to(device)
            attention_mask = batch['attention_mask'].to(device)

            predictions = model(input_ids, attention_mask) 

            for i in range(len(predictions)): 
                true_tags_str_sample = []
                pred_sample_tags_str = []
                
                valid_indices = batch['attention_mask'][i].cpu().tolist()
                
                pred_tag_ids_for_sample = predictions[i]
                true_label_ids_for_sample = batch['labels'][i].cpu().tolist()
                
                idx = 0
                for k in range(MAX_LEN):
                    if valid_indices[k] == 1: 
                        if batch['input_ids'][i, k].item() == tokenizer.cls_token_id or \
                           batch['input_ids'][i, k].item() == tokenizer.sep_token_id:
                            idx +=1 
                            continue

                        if true_label_ids_for_sample[k] != pad_tag_id:
                            true_tags_str_sample.append(idx_to_tag[true_label_ids_for_sample[k]])
                            if idx < len(pred_tag_ids_for_sample):
                                pred_sample_tags_str.append(idx_to_tag[pred_tag_ids_for_sample[idx]])
                            else:
                                pred_sample_tags_str.append(idx_to_tag[pad_tag_id]) 
                        idx += 1
                if true_tags_str_sample:
                    all_labels_str.append(true_tags_str_sample)
                    all_preds_str.append(pred_sample_tags_str)

    if not all_labels_str: 
        print("Warning: No valid labels found for evaluation.")
        return 0.0, 0.0, 0.0, ""

    precision = precision_score(all_labels_str, all_preds_str)
    recall = recall_score(all_labels_str, all_preds_str)
    f1 = f1_score(all_labels_str, all_preds_str)
    report = classification_report(all_labels_str, all_preds_str, digits=4)
    
    return precision, recall, f1, report
best_f1 = 0.0
for epoch in range(EPOCHS):
    print(f"--- Epoch {epoch+1}/{EPOCHS} ---")
    
    avg_train_loss = train_epoch(model, train_dataloader, optimizer, scheduler, DEVICE)
    print(f"Epoch {epoch+1} Training Loss: {avg_train_loss:.4f}")
    
    precision, recall, f1, report = evaluate(model, dev_dataloader, DEVICE, idx_to_tag, PAD_TAG_ID)
    print(f"Epoch {epoch+1} Validation: Precision: {precision:.4f}, Recall: {recall:.4f}, F1: {f1:.4f}")
    print("Validation Classification Report:\n", report)
    
    if f1 > best_f1:
        best_f1 = f1
        print(f"New best F1: {best_f1:.4f}. Model saved.")

print("Training complete.")
print(f"Best F1 score on Dev set: {best_f1:.4f}")

def predict_sentence(sentence_text, model, tokenizer, idx_to_tag, device, max_len, pad_tag_id):
    model.eval()
    tokens = list(sentence_text) 

    tokenized_inputs = tokenizer(
        tokens,
        is_split_into_words=True, 
        return_offsets_mapping=False, 
        padding='max_length',
        truncation=True,
        max_length=max_len,
        return_tensors="pt"
    )
    
    input_ids = tokenized_inputs["input_ids"].to(device)
    attention_mask = tokenized_inputs["attention_mask"].to(device)

    with torch.no_grad():
        predictions_ids_list = model(input_ids, attention_mask) # list of lists
    
    if not predictions_ids_list:
        return []
        
    pred_tag_ids = predictions_ids_list[0] 
    actual_tokens_with_special = tokenizer.convert_ids_to_tokens(input_ids[0].cpu().tolist())
    
    results = []
    word_idx_in_original_sentence = 0
    current_pred_idx = 0
    if actual_tokens_with_special[current_pred_idx] == tokenizer.cls_token:
        current_pred_idx +=1

    while current_pred_idx < len(pred_tag_ids) and word_idx_in_original_sentence < len(tokens):
        bert_token = actual_tokens_with_special[current_pred_idx]
        if bert_token == tokenizer.sep_token:
            break 
        original_token = tokens[word_idx_in_original_sentence]
        if bert_token.startswith("##"): 
            current_pred_idx += 1
            continue 
        tag_id = pred_tag_ids[current_pred_idx]
        tag_name = idx_to_tag[tag_id]
        results.append((original_token, tag_name))
        
        word_idx_in_original_sentence += 1
        current_pred_idx += 1
    return results
test_sentence = "3月23日下午，“青年报杯赛”U19邀请赛在越南芽庄进行，前国脚曲波挂帅的中国U19B队迎战泰国U19，上半场国青队的门户大开，泰国在第11分钟和第17分钟连进2球，半场国青0射门0角球，几乎被完全压制。下半场，国青的进攻一度有所起色，并由马辅渔利用远射扳回一球，但最终未能扳平比分。全场比赛结束，国青1-2输球，继中国杯国足0-1输给泰国之后，3天内遭遇泰国足球双杀。"
predicted_tags = predict_sentence(test_sentence, model, tokenizer, idx_to_tag, DEVICE, MAX_LEN, PAD_TAG_ID)
print(f"\n--- Prediction for: '{test_sentence}' ---")
for token, tag in predicted_tags:
    print(f"{token}: {tag}")