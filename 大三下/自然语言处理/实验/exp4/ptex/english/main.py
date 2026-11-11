import torch
from torch.utils.data import DataLoader
from transformers import BertTokenizerFast, DataCollatorForTokenClassification
from datasets import load_dataset
from seqeval.metrics import classification_report as seqeval_classification_report
from seqeval.scheme import IOB2 # CoNLL-2003 uses IOB2 schema
from tqdm.auto import tqdm # Use tqdm.auto for better notebook compatibility
import json
import os
import re 

from model import BERT_BiLSTM_CRF
from data_loader import tokenize_and_align_labels


BERT_MODEL_NAME = "bert-base-cased"
HIDDEN_DIM_LSTM = 256
BATCH_SIZE = 16 # Adjust based on your GPU memory
EPOCHS = 1 
LEARNING_RATE = 3e-5 
MAX_SEQ_LENGTH = 256 
LABEL_ALL_SUBTOKENS = False # Strategy for labeling sub-tokens (False is common)
OUTPUT_DIR = "./ner_experiment_output" # Directory to save model, logs, etc.
MODEL_FILENAME = "best_ner_model.pth" # Filename for the saved model

device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
print(f"Using device: {device}")
if torch.cuda.is_available():
    torch.backends.cudnn.benchmark = True # If input sizes are consistent

os.makedirs(OUTPUT_DIR, exist_ok=True)
model_save_path = os.path.join(OUTPUT_DIR, MODEL_FILENAME)

print("Loading CoNLL-2003 dataset...")
try:
    raw_datasets = load_dataset("conll2003")
except ValueError as e:
    if "trust_remote_code" in str(e):
        print("Attempting to load conll2003 with trust_remote_code=True due to custom script.")
        raw_datasets = load_dataset("conll2003", trust_remote_code=True)
    else:
        raise e
print("Dataset loaded successfully.")

label_list = raw_datasets["train"].features["ner_tags"].feature.names
tag2id = {tag: i for i, tag in enumerate(label_list)}
id2tag = {i: tag for i, tag in tag2id.items()}
num_labels = len(label_list)

print(f"Number of labels: {num_labels}")
print(f"Label list: {label_list}")

with open(os.path.join(OUTPUT_DIR, "tag2id.json"), "w") as f:
    json.dump(tag2id, f)
with open(os.path.join(OUTPUT_DIR, "id2tag.json"), "w") as f:
    json.dump(id2tag, f)

print(f"Initializing tokenizer: {BERT_MODEL_NAME}")
tokenizer = BertTokenizerFast.from_pretrained(BERT_MODEL_NAME)

print("Tokenizing dataset and aligning labels...")
tokenized_datasets = raw_datasets.map(
    lambda examples_batch: tokenize_and_align_labels(
        examples_batch,
        tokenizer,
        MAX_SEQ_LENGTH,
        LABEL_ALL_SUBTOKENS
    ),
    batched=True,
)
print("Tokenization and alignment complete.")

original_columns_to_remove = list(raw_datasets["train"].column_names)
columns_from_tokenizer_to_check = ["offset_mapping"]
potential_columns_to_remove = original_columns_to_remove + columns_from_tokenizer_to_check

for split in tokenized_datasets.keys():
    current_columns = tokenized_datasets[split].column_names
    actual_columns_to_remove_for_split = [
        col for col in potential_columns_to_remove if col in current_columns
    ]
    if actual_columns_to_remove_for_split:
        print(f"Removing columns from '{split}' split: {actual_columns_to_remove_for_split}")
        tokenized_datasets[split] = tokenized_datasets[split].remove_columns(actual_columns_to_remove_for_split)
    print(f"Final columns in '{split}' split: {tokenized_datasets[split].column_names}")

data_collator = DataCollatorForTokenClassification(tokenizer=tokenizer, padding="longest")

train_dataloader = DataLoader(
    tokenized_datasets["train"],
    shuffle=True,
    collate_fn=data_collator,
    batch_size=BATCH_SIZE,
    num_workers=2 if device.type == 'cuda' else 0, # Use workers if on GPU
    pin_memory=True if device.type == 'cuda' else False
)
validation_dataloader = DataLoader(
    tokenized_datasets["validation"],
    collate_fn=data_collator,
    batch_size=BATCH_SIZE,
    num_workers=2 if device.type == 'cuda' else 0,
    pin_memory=True if device.type == 'cuda' else False
)
test_dataloader = DataLoader(
    tokenized_datasets["test"],
    collate_fn=data_collator,
    batch_size=BATCH_SIZE,
    num_workers=2 if device.type == 'cuda' else 0,
    pin_memory=True if device.type == 'cuda' else False
)
print("DataLoaders created.")

print("Initializing BERT_BiLSTM_CRF model...")
model = BERT_BiLSTM_CRF(
    bert_model_name=BERT_MODEL_NAME,
    hidden_dim=HIDDEN_DIM_LSTM,
    tag2id=tag2id
).to(device)

optimizer = torch.optim.AdamW(model.parameters(), lr=LEARNING_RATE)
print("Model and optimizer initialized.")

print(f"Starting training for {EPOCHS} epochs...")
best_validation_f1 = 0.0

for epoch in range(EPOCHS):
    print(f"\n--- Epoch {epoch + 1}/{EPOCHS} ---")
    model.train()
    total_train_loss = 0
    train_progress_bar = tqdm(train_dataloader, desc="Training", leave=False)
    for batch in train_progress_bar:
        input_ids = batch["input_ids"].to(device)
        attention_mask = batch["attention_mask"].to(device)
        labels = batch["labels"].to(device)

        optimizer.zero_grad()
        loss, _ = model(input_ids, attention_mask, labels)
        loss.backward()
        optimizer.step()

        total_train_loss += loss.item()
        train_progress_bar.set_postfix({'loss': f"{loss.item():.4f}"})
    
    avg_train_loss = total_train_loss / len(train_dataloader)
    print(f"Average Training Loss: {avg_train_loss:.4f}")

    model.eval()
    total_validation_loss = 0
    all_true_validation_sequences = []
    all_predicted_validation_sequences = []
    validation_progress_bar = tqdm(validation_dataloader, desc="Validating", leave=False)

    with torch.no_grad():
        for batch in validation_progress_bar:
            input_ids = batch["input_ids"].to(device)
            attention_mask = batch["attention_mask"].to(device)
            labels_in_batch = batch["labels"].to(device)

            loss, predicted_ids_list = model(input_ids, attention_mask, labels_in_batch)
            total_validation_loss += loss.item()

            for i in range(labels_in_batch.size(0)):
                true_tags_for_sample = []
                predicted_tags_for_sample = []
                current_pred_token_idx = 0
                for j in range(labels_in_batch.size(1)):
                    if attention_mask[i, j].item() == 1:
                        if current_pred_token_idx < len(predicted_ids_list[i]):
                            if labels_in_batch[i, j].item() != -100:
                                true_tags_for_sample.append(id2tag[labels_in_batch[i, j].item()])
                                predicted_tags_for_sample.append(id2tag[predicted_ids_list[i][current_pred_token_idx]])
                            current_pred_token_idx += 1
                        else:
                            break
                if true_tags_for_sample:
                    all_true_validation_sequences.append(true_tags_for_sample)
                    all_predicted_validation_sequences.append(predicted_tags_for_sample)

    avg_validation_loss = total_validation_loss / len(validation_dataloader)
    print(f"Average Validation Loss: {avg_validation_loss:.4f}")

    if all_true_validation_sequences and all_predicted_validation_sequences:
        try:
            validation_report_str = seqeval_classification_report(
                all_true_validation_sequences,
                all_predicted_validation_sequences,
                scheme=IOB2,
                output_dict=False,
                zero_division=0
            )
            validation_report_dict = seqeval_classification_report(
                all_true_validation_sequences,
                all_predicted_validation_sequences,
                scheme=IOB2,
                output_dict=True,
                zero_division=0
            )
            print("Validation Report (seqeval):")
            print(validation_report_str)
            current_micro_f1 = validation_report_dict.get("micro avg", {}).get("f1-score", 0.0)
            print(f"Validation Micro Avg F1-score: {current_micro_f1:.4f}")

            if current_micro_f1 > best_validation_f1:
                best_validation_f1 = current_micro_f1
                print(f"New best validation F1: {best_validation_f1:.4f}. Saving model to {model_save_path}")
                torch.save(model.state_dict(), model_save_path)
        except Exception as e_report:
            print(f"Error generating seqeval report for validation: {e_report}")
            print("True sequences sample:", all_true_validation_sequences[:2])
            print("Predicted sequences sample:", all_predicted_validation_sequences[:2])
    else:
        print("Validation: No valid sequences found to compute metrics.")

print(f"\n--- Training Finished ---")
print(f"Best Validation F1-score (seqeval micro avg): {best_validation_f1:.4f}")


if os.path.exists(model_save_path) and best_validation_f1 > 0:
    print(f"\n--- Evaluating on Test Set with Best Model from {model_save_path} ---")
    model.load_state_dict(torch.load(model_save_path, map_location=device))
    model.to(device) 
    model.eval()

    all_true_test_sequences = []
    all_predicted_test_sequences = []
    test_progress_bar = tqdm(test_dataloader, desc="Testing", leave=False)

    with torch.no_grad():
        for batch in test_progress_bar:
            input_ids = batch["input_ids"].to(device)
            attention_mask = batch["attention_mask"].to(device)
            labels_in_batch = batch["labels"].to(device)
            
            _, predicted_ids_list = model(input_ids, attention_mask, labels_in_batch)

            for i in range(labels_in_batch.size(0)):
                true_tags_for_sample = []
                predicted_tags_for_sample = []
                current_pred_token_idx = 0
                for j in range(labels_in_batch.size(1)):
                    if attention_mask[i, j].item() == 1:
                        if current_pred_token_idx < len(predicted_ids_list[i]):
                            if labels_in_batch[i, j].item() != -100:
                                true_tags_for_sample.append(id2tag[labels_in_batch[i, j].item()])
                                predicted_tags_for_sample.append(id2tag[predicted_ids_list[i][current_pred_token_idx]])
                            current_pred_token_idx += 1
                        else:
                            break
                if true_tags_for_sample:
                    all_true_test_sequences.append(true_tags_for_sample)
                    all_predicted_test_sequences.append(predicted_tags_for_sample)
    
    if all_true_test_sequences and all_predicted_test_sequences:
        try:
            test_report_str = seqeval_classification_report(
                all_true_test_sequences,
                all_predicted_test_sequences,
                scheme=IOB2,
                output_dict=False,
                zero_division=0
            )
            print("Test Set Report (seqeval):")
            print(test_report_str)
        except Exception as e_report_test:
            print(f"Error generating seqeval report for test: {e_report_test}")
            print("True test sequences sample:", all_true_test_sequences[:2])
            print("Predicted test sequences sample:", all_predicted_test_sequences[:2])

    else:
        print("Test Set: No valid sequences found to compute metrics.")
else:
    print("\nNo best model was saved during training (or best_validation_f1 was 0). Skipping test set evaluation.")


print("\n--- Example Prediction on Custom Text ---")

model_loaded_for_prediction = False
if os.path.exists(model_save_path):
    print(f"Loading model from {model_save_path} for custom prediction.")
    try:
        model.load_state_dict(torch.load(model_save_path, map_location=device))
        model.to(device)
        model_loaded_for_prediction = True
    except Exception as e_load:
        print(f"Error loading saved model: {e_load}. Will use model from last training epoch if available.")
elif best_validation_f1 == 0 and 'model' in locals() and model is not None:
     print("No best model saved from training. Using model from the last epoch for prediction.")
     model_loaded_for_prediction = True 
else:
    print(f"No saved model found at {model_save_path} and no model in memory from training. Cannot perform custom prediction.")

if model_loaded_for_prediction:
    model.eval() 

    custom_text_long = (
        "Xi Jinping, male, Han ethnicity, was born in June 1953 and is from Fuping, Shaanxi Province. "
        "He began his first job in January 1969 and joined the Communist Party of China (CPC) in January 1974. "
        "Xi graduated from School of Humanities and Social Sciences, Tsinghua University where he completed an "
        "in-service graduate program in Marxist theory and ideological and political education. He holds a Doctor "
        "of Law degree. Xi is currently General Secretary of the CPC Central Committee, Chairman of the CPC Central "
        "Military Commission, President of the People's Republic of China (PRC), and Chairman of the PRC Central Military Commission."
    )

    def simple_tokenizer_for_custom_text(text):
        text = re.sub(r'([,.()!?"\'])', r' \1 ', text)
        text = re.sub(r'\s{2,}', ' ', text).strip()
        tokens = text.split(' ')
        return [token for token in tokens if token]

    custom_sentence_tokens_long = simple_tokenizer_for_custom_text(custom_text_long)

    def predict_sentence_tags(tokens_list, ner_model, ner_tokenizer, ner_id2tag, ner_device, max_seq_len):
        ner_model.eval()
        inputs = ner_tokenizer(
            [tokens_list], 
            is_split_into_words=True,
            return_tensors="pt",
            max_length=max_seq_len,
            padding="max_length",
            truncation=True, 
            return_word_ids=True
        )
        
        input_ids = inputs["input_ids"].to(ner_device)
        attention_mask = inputs["attention_mask"].to(ner_device)
        word_ids = inputs.word_ids(batch_index=0)

        with torch.no_grad():
            predicted_tag_ids_batch = ner_model(input_ids, attention_mask, labels=None)
            
        if not predicted_tag_ids_batch or not predicted_tag_ids_batch[0]:
            print("Prediction returned empty or failed.")
            return []

        predictions_for_active_tokens = predicted_tag_ids_batch[0]
        
        word_level_predictions = []
        previous_word_idx_from_tokenizer = None
        current_active_token_pred_idx = 0

        for i in range(input_ids.size(1)):
            if attention_mask[0, i].item() == 0:
                continue
            
            if current_active_token_pred_idx >= len(predictions_for_active_tokens):
                break
                
            original_word_idx = word_ids[i]

            if original_word_idx is not None:
                if original_word_idx != previous_word_idx_from_tokenizer:
                    if original_word_idx < len(tokens_list): 
                        predicted_tag_id = predictions_for_active_tokens[current_active_token_pred_idx]
                        word_level_predictions.append(
                            (tokens_list[original_word_idx], ner_id2tag.get(predicted_tag_id, "UNK_TAG"))
                        )
            
            current_active_token_pred_idx += 1
            previous_word_idx_from_tokenizer = original_word_idx
            
        return word_level_predictions

    print(f"\n--- Predicting tags for the long custom text (up to first {MAX_SEQ_LENGTH} BERT tokens) ---")
    predicted_tags_long = predict_sentence_tags(
        custom_sentence_tokens_long, model, tokenizer, id2tag, device, MAX_SEQ_LENGTH
    )

    print("\nCustom Long Sentence Prediction Results:")
    if predicted_tags_long:
        for token, tag in predicted_tags_long:
            print(f"{token:<20} -> {tag}")
    else:
        print("No predictions were generated for the custom long sentence.")

print("\n--- Script Finished ---")