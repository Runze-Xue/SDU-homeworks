import torch
import torch.optim as optim
from torch.utils.data import DataLoader
from transformers import BertTokenizerFast, get_linear_schedule_with_warmup
from seqeval.metrics import f1_score, precision_score, recall_score, classification_report

from en_data_loader import load_data, get_tags, NerDataset  # 你应确保适配CoNLL格式的load_data
from model import BertBiLSTMCRF  # 模型结构无变化

# --- 配置参数 ---
BERT_MODEL_NAME = 'bert-base-cased'
TRAIN_FILE = 'conll2003_train.txt'
DEV_FILE = 'conll2003_dev.txt'

MAX_LEN = 128
BATCH_SIZE = 16
EPOCHS = 5
LEARNING_RATE = 3e-5
LSTM_HIDDEN_DIM = 256
LSTM_LAYERS = 1
DROPOUT_RATE = 0.1
WEIGHT_DECAY = 0.01
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
        torch.nn.utils.clip_grad_norm_(model.parameters(), 1.0) #梯度裁剪
        optimizer.step()
        scheduler.step()
        
        total_loss += loss.item()
        
        if (batch_idx + 1) % 5 == 0: # 每 N 个 batch 打印一次
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
            labels = batch['labels'] # labels保持在CPU上，并且是原始的，用于比较

            predictions = model(input_ids, attention_mask) # List of lists of tag_ids

            for i in range(len(predictions)): # 遍历batch中的每个样本
                pred_tag_ids = predictions[i] # 单个样本的预测tag id列表
                true_label_ids = labels[i].tolist() # 单个样本的真实tag id列表
                
                # `attention_mask` 用于确定实际长度 (不包括 [PAD] tokens)
                # `word_ids` (在Dataset中生成，但这里没有直接传过来) 用于对齐subword
                # 我们需要基于原始的tokenization重建标签，只考虑第一个subword的标签
                
                # 获取原始tokens和对齐后的真实标签 (只考虑有效部分)
                # 注意：这里的对齐逻辑是为了seqeval评估，它期望每个原始token一个标签
                # 我们需要重新从 input_ids 和原始的 labels (经过subword处理的) 构建
                
                # 简化：我们直接比较 CRF 输出的标签 和 Dataset 中已经对齐好的 labels
                # 只不过 CRF 的输出不包含 [CLS] [SEP] 的标签，而 labels 包含了
                # 并且 CRF 输出的长度是实际句子长度 (由 mask 决定)，而 labels 是 max_len
                
                # 修正对齐逻辑以匹配seqeval的要求：
                # seqeval 需要 List[List[str]], 例如:
                # y_true = [['O', 'O', 'B-PER', 'I-PER', 'O'], ['B-LOC', 'I-LOC']]
                # y_pred = [['O', 'O', 'B-PER', 'I-PER', 'O'], ['B-LOC', 'O']]
                
                # 当前 batch['labels'] 已经处理了 subword，将非首个 subword 标为 PAD_TAG_ID
                # CRF 的 predictions 是针对每个 token 的，我们需要过滤掉 [CLS], [SEP] 和 subword 标记
                
                # 为了准确评估，我们需要知道哪些是原始词的第一个token
                # 让我们重新分词以获得word_ids，这有点冗余，更好的方式是在Dataset中也返回word_ids
                
                # 简便但稍不精确的做法（假设CRF输出长度对应于移除了[CLS]和[SEP]的token序列）：
                # 对于评估，我们只关心非 PAD_TAG_ID 的标签
                
                pred_tags_str_sample = []
                true_tags_str_sample = []

                # 我们需要从原始的tokenized_inputs中恢复word_ids来正确对齐
                # 这是一个常见痛点。更稳妥的做法是 Dataset 也输出 word_ids
                # 或者，在评估时，我们只关心那些标签不是 pad_tag_id 的位置
                
                # 假设模型输出不包含 [CLS] 和 [SEP] 的标签预测
                # 实际上，BERT的输出会包含[CLS]和[SEP]的表示，CRF层也是基于此
                # 因此，predictions的长度应该和labels在有效token上的长度一致
                # pytorch-crf的decode输出列表，每个列表是解码的标签序列，长度由mask决定

                # 真实标签处理：只取有效部分，并把 PAD_TAG_ID 变回 'O' (或其他)
                # 预测标签处理：同上
                
                # 改用更严谨的方式：
                # 1. 从 input_ids 获得 tokens
                # 2. 从 labels 获得真实标签，并根据 word_ids 对齐
                # 3. 从 predictions 获得预测标签，并根据 word_ids 对齐
                
                current_input_ids = batch['input_ids'][i].cpu().tolist()
                current_labels = batch['labels'][i].cpu().tolist()
                # `predictions[i]` 是模型对当前样本的预测标签ID列表
                # 其长度是根据 attention_mask 确定的有效token数 (可能不包括 [CLS], [SEP] 的标签)
                # pytorch-crf 的 decode 返回的序列长度是有效的 token 数，不包括首尾的特殊 token 和 padding
                # 我们需要确保比较的序列长度一致且对齐
                
                # 获取 word_ids 以正确处理 subword
                # (这是个简化，实际应该从Dataset获取)
                raw_tokens = tokenizer.convert_ids_to_tokens(current_input_ids, skip_special_tokens=False)
                word_ids_list = []
                # 模拟 tokenizer(tokens, is_split_into_words=True).word_ids()
                # 这个模拟比较复杂，不如直接在Dataset中返回原始tokens和对齐好的标签
                # 我们先用一种近似的方式，假设CRF输出对齐了第一个subword
                
                # 取巧：只比较非 PAD_TAG_ID 的标签
                # 并且忽略 [CLS] 和 [SEP]
                
                # 假设 `predictions[i]` 的长度已经对应了有效token序列（不含[CLS],[SEP]）
                # `labels[i]` 是 MAX_LEN 长度，包含 [CLS], [SEP] 和 padding
                # attention_mask[i] 可以告诉我们哪些是真实token
                
                # 重新思考对齐：
                # BertTokenizer 产生的 token 序列通常是：[CLS] t1 t2 ... tn [SEP] [PAD] ...
                # 对应的 label_ids 也是这样对齐的。
                # CRF 的 emission scores 也是 (batch, seq_len, num_tags)，seq_len 是 MAX_LEN
                # CRF 的 mask (来自 attention_mask) 也是 (batch, seq_len)
                # CRF 的 decode 输出的 list of lists，每个 list 的长度是该样本的有效 token 数量
                # 这个有效 token 数量应该是指 attention_mask 中为 1 的部分，包括 [CLS] 和 [SEP]
                
                # 所以，pred_tag_ids 是包含 [CLS] 和 [SEP] 的标签
                # 我们在评估时通常跳过 [CLS] 和 [SEP]
                # 并且只考虑原始词的第一个subword的标签

                true_sample_tags_str = []
                pred_sample_tags_str = []
                
                # Re-tokenize to get word_ids for alignment (this is inefficient here, best done in Dataset)
                original_words = []
                original_tags_for_words = [] # Placeholder, not easily reconstructed here
                                             # The NerDataset's label alignment logic is key

                # Let's use a simplified approach for now:
                # Assume predictions[i] and labels[i] are aligned for non-pad tokens
                # And we need to filter out sub-words and special tokens for seqeval
                
                # This part is tricky without proper word_ids propagation
                # Let's try to align based on `batch['labels']` which has PAD_TAG_ID for subwords
                
                valid_indices = batch['attention_mask'][i].cpu().tolist()
                
                pred_tag_ids_for_sample = predictions[i] # This is for tokens where mask was True
                true_label_ids_for_sample = batch['labels'][i].cpu().tolist()

                # `pred_tag_ids_for_sample` length is sum(attention_mask[i])
                # `true_label_ids_for_sample` length is MAX_LEN
                
                # We need to align them.
                # The CRF decode output length corresponds to the number of 1s in the mask.
                # So, `pred_tag_ids_for_sample` matches `true_label_ids_for_sample` where `attention_mask[i]` is 1.
                
                idx = 0
                for k in range(MAX_LEN):
                    if valid_indices[k] == 1: # If it's not a PAD token
                        # Skip [CLS] and [SEP] for seqeval evaluation
                        # BERT tokenizer.cls_token_id, sep_token_id
                        if batch['input_ids'][i, k].item() == tokenizer.cls_token_id or \
                           batch['input_ids'][i, k].item() == tokenizer.sep_token_id:
                            idx +=1 # Consume prediction for CLS/SEP but don't add to seqeval lists
                            continue

                        # Only consider labels that are not PAD_TAG_ID (i.e., first subword of an original word)
                        if true_label_ids_for_sample[k] != pad_tag_id:
                            true_tags_str_sample.append(idx_to_tag[true_label_ids_for_sample[k]])
                            # pred_tag_ids_for_sample[idx] corresponds to this k-th token
                            if idx < len(pred_tag_ids_for_sample): # Make sure idx is within bounds
                                pred_sample_tags_str.append(idx_to_tag[pred_tag_ids_for_sample[idx]])
                            else:
                                # This case should ideally not happen if lengths match as expected
                                pred_sample_tags_str.append(idx_to_tag[pad_tag_id]) # or 'O'
                        idx += 1
                    # If valid_indices[k] == 0, it's a PAD token, true_label_ids_for_sample[k] is pad_tag_id
                    # and it's not in pred_tag_ids_for_sample

                if true_tags_str_sample: # Only add if there are actual tags
                    all_labels_str.append(true_tags_str_sample)
                    all_preds_str.append(pred_sample_tags_str)

    if not all_labels_str: # handle empty dev set or all padding
        print("Warning: No valid labels found for evaluation.")
        return 0.0, 0.0, 0.0, ""

    precision = precision_score(all_labels_str, all_preds_str)
    recall = recall_score(all_labels_str, all_preds_str)
    f1 = f1_score(all_labels_str, all_preds_str)
    report = classification_report(all_labels_str, all_preds_str, digits=4)
    
    return precision, recall, f1, report
DEVICE = torch.device("cuda" if torch.cuda.is_available() else "cpu")
print(f"Using device: {DEVICE}")

# --- 1. 加载数据 ---
tokenizer = BertTokenizerFast.from_pretrained(BERT_MODEL_NAME)

train_sentences = load_data(TRAIN_FILE)  # 结构: [[(word1, tag1), (word2, tag2), ...], ...]
dev_sentences = load_data(DEV_FILE)

tag_to_idx, idx_to_tag, PAD_TAG_ID = get_tags(train_sentences + dev_sentences)
NUM_TAGS = len(tag_to_idx)

train_dataset = NerDataset(train_sentences, tokenizer, tag_to_idx, MAX_LEN, PAD_TAG_ID)
dev_dataset = NerDataset(dev_sentences, tokenizer, tag_to_idx, MAX_LEN, PAD_TAG_ID)

train_dataloader = DataLoader(train_dataset, batch_size=BATCH_SIZE, shuffle=True)
dev_dataloader = DataLoader(dev_dataset, batch_size=BATCH_SIZE)

# --- 2. 模型初始化 ---
model = BertBiLSTMCRF(
    bert_model_name=BERT_MODEL_NAME,
    num_tags=NUM_TAGS,
    lstm_hidden_dim=LSTM_HIDDEN_DIM,
    lstm_layers=LSTM_LAYERS,
    dropout_rate=DROPOUT_RATE
)
model.to(DEVICE)

# --- 3. 优化器和学习率调度器 ---
optimizer = optim.AdamW(model.parameters(), lr=LEARNING_RATE, weight_decay=WEIGHT_DECAY)
total_steps = len(train_dataloader) * EPOCHS
scheduler = get_linear_schedule_with_warmup(optimizer, num_warmup_steps=0, num_training_steps=total_steps)

# --- 4. 训练和评估函数（同你原来的代码） ---
# train_epoch(...) 和 evaluate(...) 函数可原样复用

# --- 5. 主训练循环 ---
best_f1 = 0.0
for epoch in range(EPOCHS):
    print(f"\n--- Epoch {epoch+1}/{EPOCHS} ---")
    train_loss = train_epoch(model, train_dataloader, optimizer, scheduler, DEVICE)
    print(f"Train Loss: {train_loss:.4f}")
    
    precision, recall, f1, report = evaluate(model, dev_dataloader, DEVICE, idx_to_tag, PAD_TAG_ID)
    print(f"Eval - Precision: {precision:.4f}, Recall: {recall:.4f}, F1: {f1:.4f}")
    print("Classification Report:\n", report)

    if f1 > best_f1:
        best_f1 = f1
        torch.save(model.state_dict(), "best_model.pth")
        print("Saved new best model.")

print(f"\nBest F1 on Dev: {best_f1:.4f}")
