# -*- coding: utf-8 -*-
import torch
import torch.nn as nn
from transformers import BertPreTrainedModel, BertModel, BertTokenizerFast, BertConfig
from TorchCRF import CRF
from torch.utils.data import Dataset, DataLoader
from torch.optim import AdamW
from seqeval.metrics import classification_report # 用于评估 (可选)
import os # 用于检查文件

# --- 1. 标签定义与映射 ---
# 确保这里的标签与你的 small-chinese.txt 文件中的标签完全一致
labels = [
    "O",
    "B-name", "I-name",
    "B-address", "I-address",
    "B-org", "I-org",
    "B-time", "I-time",
    "B-date", "I-date",
    "B-loc", "I-loc",
    "B-id", "I-id",
    "B-job", "I-job",
    "B-nationality", "I-nationality",
    "B-edu", "I-edu",
    "B-company", "I-company",
    "B-product", "I-product",
    "B-book", "I-book",
    "B-movie", "I-movie",
    "B-email", "I-email",
    "B-website", "I-website",
    "B-money", "I-money",
    "B-percent", "I-percent",
    "B-law", "I-law",
    "B-ORG","I-ORG"
]

label_map = {label: idx for idx, label in enumerate(labels)}
label_map_inv = {idx: label for label, idx in label_map.items()}
NUM_LABELS = len(labels)

# --- 2. 数据读取函数 ---

def read_bio_file(filepath):
    """
    读取 BIO 格式的文件 (例如 small-chinese.txt)
    文件格式应为:
    字 标签
    字 标签
    ... (句子结束空一行)
    字 标签
    ...
    返回:
        texts: list[list[str]], e.g., [['我', '爱', '中', '国'], ['北', '京', '欢', '迎', '你']]
        labels: list[list[str]], e.g., [['O', 'O', 'B-LOC', 'I-LOC'], ['B-LOC', 'I-LOC', 'O', 'O', 'O']]
    """
    if not os.path.exists(filepath):
        raise FileNotFoundError(f"训练文件未找到: {filepath}")

    texts = []
    labels_list = []
    current_text = []
    current_labels = []
    line_num = 0

    with open(filepath, 'r', encoding='utf-8') as file:
        for line in file:
            line_num += 1
            line = line.strip()
            if line == "":
                if current_text: # 句子结束
                    if len(current_text) != len(current_labels):
                         print(f"警告: 文件 {filepath} 第 {line_num} 行附近句子长度与标签长度不匹配。跳过此句。")
                         # 可以选择抛出错误或记录下来
                    else:
                        texts.append(current_text)
                        labels_list.append(current_labels)
                    current_text = []
                    current_labels = []
            else:
                parts = line.split(maxsplit=1) # 使用 maxsplit=1 避免多余空格导致的问题
                if len(parts) == 2:
                    char, tag = parts
                    # 检查标签是否在预定义中 (可选但推荐)
                    if tag not in label_map:
                        print(f"警告: 文件 {filepath} 第 {line_num} 行发现未知标签 '{tag}'，将视为 'O'.")
                        tag = 'O'
                    current_text.append(char)
                    current_labels.append(tag)
                else:
                    print(f"跳过无效行 {line_num} (格式错误): {line}")

        # 处理文件末尾没有空行的情况
        if current_text:
            if len(current_text) != len(current_labels):
                 print(f"警告: 文件 {filepath} 末尾句子长度与标签长度不匹配。跳过此句。")
            else:
                texts.append(current_text)
                labels_list.append(current_labels)

    if not texts:
        print(f"警告: 未能从 {filepath} 读取到任何有效的训练数据。")

    return texts, labels_list

def read_plain_text_file(filepath):
    """
    读取纯文本文件 (例如 Chinese.txt)，每行一个句子。
    返回:
        texts: list[list[str]], e.g., [['3', '月', '2', ...], ['另', '一', '个', ...]]
    """
    if not os.path.exists(filepath):
        raise FileNotFoundError(f"测试文件未找到: {filepath}")

    texts = []
    with open(filepath, 'r', encoding='utf-8') as f:
        for line in f:
            line = line.strip()
            if line: # 只处理非空行
                texts.append(list(line)) # 将句子字符串转换为字符列表

    if not texts:
        print(f"警告: 未能从 {filepath} 读取到任何有效的测试数据。")
    return texts

# --- 3. 模型定义 ---

class BertBiLSTMCRF(BertPreTrainedModel):
    def __init__(self, config, num_labels):
        super(BertBiLSTMCRF, self).__init__(config)
        self.num_labels = num_labels
        self.bert = BertModel(config, add_pooling_layer=False) # 不需要 pooler output
        classifier_dropout = (
            config.classifier_dropout if config.classifier_dropout is not None else config.hidden_dropout_prob
        )
        self.dropout = nn.Dropout(classifier_dropout)

        # LSTM 层: 输入大小为 BERT 输出大小, 输出大小可以自定义或与BERT相同
        # 将 LSTM 隐藏层大小设为 BERT 的一半比较常见
        lstm_hidden_size = config.hidden_size // 2
        self.lstm = nn.LSTM(input_size=config.hidden_size,
                            hidden_size=lstm_hidden_size,
                            num_layers=1, # 可以尝试增加层数
                            bidirectional=True,
                            batch_first=True) # 输出形状 (batch, seq_len, hidden_size*2)

        # 全连接层: 输入大小为 LSTM 输出大小 (双向*隐藏层大小)
        self.fc = nn.Linear(lstm_hidden_size * 2, num_labels)

        # CRF 层
        self.crf = CRF(num_labels, batch_first=True)

        # 初始化权重 (BertPreTrainedModel 会处理 BERT 部分的加载)
        # self.init_weights() # 通常不需要手动调用，除非要自定义 LSTM/FC 的初始化

    def forward(self, input_ids, attention_mask=None, token_type_ids=None, labels=None, word_ids=None):
        """
        Args:
            input_ids (torch.Tensor): shape (batch_size, seq_length)
            attention_mask (torch.Tensor): shape (batch_size, seq_length)
            token_type_ids (torch.Tensor, optional): shape (batch_size, seq_length)
            labels (torch.Tensor, optional): shape (batch_size, seq_length). 训练时需要提供.
            word_ids (torch.Tensor, optional): shape (batch_size, seq_length). 预测时用于对齐.

        Returns:
            loss (torch.Tensor): 如果提供了 labels，返回 CRF 损失 (负对数似然).
            predictions (list[list[int]]): 如果未提供 labels，返回每个样本的预测标签 ID 序列.
        """
        bert_outputs = self.bert(
            input_ids,
            attention_mask=attention_mask,
            token_type_ids=token_type_ids,
            return_dict=True # 建议使用 return_dict=True
        )
        # 获取 BERT 最后一层的隐藏状态
        sequence_output = bert_outputs.last_hidden_state # shape: (batch_size, seq_length, hidden_size)
        sequence_output = self.dropout(sequence_output)

        # 通过 LSTM
        lstm_output, (hidden, cell) = self.lstm(sequence_output) # shape: (batch_size, seq_length, lstm_hidden_size * 2)

        # 通过全连接层得到发射分数 (emissions)
        emissions = self.fc(lstm_output) # shape: (batch_size, seq_length, num_labels)

        # 准备 CRF 需要的 mask (将 attention_mask 转为 bool 类型)
        # 注意：CRF 期望 mask 中 True 的位置是有效 token，False 是 padding
        mask = attention_mask.bool() if attention_mask is not None else None

        if labels is not None:
            # --- 训练模式 ---
            if mask is None:
                 # 如果没有提供 attention_mask，假设所有输入都是有效的 (可能不安全)
                 mask = torch.ones_like(labels, dtype=torch.bool)

            # 计算 CRF 损失 (负对数似然)
            # labels 中 padding 的位置应该有特殊值 (例如 -100)，CRF 层内部或外部需要处理
            # TorchCRF 的 mask 参数可以处理 padding
            loss = -self.crf(emissions, labels, mask=mask, reduction='mean') # reduction='token_mean' 也可以考虑
            return loss
        else:
            # --- 预测模式 ---
            if mask is None:
                 mask = torch.ones(emissions.shape[:2], dtype=torch.bool, device=emissions.device)

            # 使用 CRF 解码得到最优路径 (预测标签序列)
            # 返回的是一个 list[list[int]], 每个内层 list 对应一个样本的预测结果
            # 其长度等于对应样本 mask 中 True 的数量
            predictions = self.crf.decode(emissions, mask=mask)
            return predictions


# --- 4. Dataset 类 ---

class NERDataset(Dataset):
    """用于训练和验证的 Dataset"""
    def __init__(self, texts, labels_list, tokenizer, max_len, label_map):
        self.texts = texts # list[list[str]]
        self.labels_list = labels_list # list[list[str]]
        self.tokenizer = tokenizer
        self.max_len = max_len
        self.label_map = label_map
        # 使用 label_map['O'] 作为特殊 token 和 subword 的默认标签 ID
        self.default_label_id = self.label_map.get('O', 0)

    def __len__(self):
        return len(self.texts)

    def __getitem__(self, item):
        text = self.texts[item]
        labels = self.labels_list[item]

        # Tokenize: is_split_into_words=True 因为输入已经是字符列表
        encoding = self.tokenizer(
            text,
            is_split_into_words=True,
            return_offsets_mapping=False, # 暂时不需要 offset mapping
            padding='max_length',
            truncation=True,
            max_length=self.max_len,
            return_tensors='pt' # 返回 PyTorch 张量
        )

        # 获取 token 对应的原始 word/char 索引
        word_ids = encoding.word_ids(batch_index=0)

        # --- 关键：对齐标签 ---
        label_ids = []
        previous_word_idx = None
        for word_idx in word_ids:
            if word_idx is None:
                # 特殊 token ([CLS], [SEP], [PAD])
                # 通常设为 'O' 或者一个可以被损失函数忽略的值 (如 -100)
                # 这里我们统一设为 'O' 的 ID，因为 CRF 需要有效的标签 ID
                label_ids.append(self.default_label_id)
            elif word_idx != previous_word_idx:
                # 一个新词/字的第一个 token
                # 从原始 labels_list 中获取该词/字的标签
                label_str = labels[word_idx]
                label_ids.append(self.label_map.get(label_str, self.default_label_id))
            else:
                # 同一个词/字的后续 token (subword)
                # 策略：可以设为 'O'，或者设为对应的 'I-' 标签，或者设为与第一个 token 相同的标签
                # 这里简单处理，也设为与第一个 token 相同的标签
                label_str = labels[word_idx]
                label_ids.append(self.label_map.get(label_str, self.default_label_id))
                # 或者，如果想严格遵循 BIO，如果第一个 token 是 B-tag，这里可以是 I-tag
                # label_str = labels[word_idx]
                # tag = label_str[2:] if label_str.startswith(('B-', 'I-')) else None
                # if tag and label_str.startswith('B-'):
                #      label_ids.append(self.label_map.get(f"I-{tag}", self.default_label_id))
                # else:
                #      label_ids.append(self.label_map.get(label_str, self.default_label_id))

            previous_word_idx = word_idx

        # input_ids, attention_mask 已经 padding 到 max_len
        # label_ids 也需要是 max_len
        # 由于我们为 [PAD] token 也分配了 default_label_id，长度应该已经匹配

        # .squeeze(0) 移除 tokenizer 添加的 batch 维度
        return {
            'input_ids': encoding['input_ids'].squeeze(0),
            'attention_mask': encoding['attention_mask'].squeeze(0),
            'labels': torch.tensor(label_ids, dtype=torch.long) # 确保是 Long 类型
        }


class TestDataset(Dataset):
    """用于预测的 Dataset (无标签)"""
    def __init__(self, texts, tokenizer, max_len):
        self.texts = texts # list[list[str]]
        self.tokenizer = tokenizer
        self.max_len = max_len

    def __len__(self):
        return len(self.texts)

    def __getitem__(self, item):
        text = self.texts[item] # text 是一个字符列表

        encoding = self.tokenizer(
            text,
            is_split_into_words=True,
            return_offsets_mapping=False,
            padding='max_length',
            truncation=True,
            max_length=self.max_len,
            return_tensors='pt'
        )

        # 获取 word_ids 用于后续对齐
        word_ids = encoding.word_ids(batch_index=0)
        # word_ids 可能包含 None，需要处理

        # --- 修改开始 (简洁版) ---
        processed_word_ids = [idx if idx is not None else -1 for idx in word_ids] if word_ids is not None else []
        # --- 修改结束 ---

        return {
            'input_ids': encoding['input_ids'].squeeze(0),
            'attention_mask': encoding['attention_mask'].squeeze(0),
            # 使用处理过的列表创建 Tensor
            'word_ids': torch.tensor(processed_word_ids, dtype=torch.long), # <--- 修改后
            'original_text': text # 传递原始字符列表用于最终输出
        }

# --- 5. 训练函数 ---

def train_epoch(model, dataloader, optimizer, device):
    model.train() # 设置为训练模式
    total_loss = 0
    for batch in dataloader:
        # 清零梯度
        optimizer.zero_grad()

        # 将数据移动到指定设备
        input_ids = batch['input_ids'].to(device)
        attention_mask = batch['attention_mask'].to(device)
        labels = batch['labels'].to(device)

        # 前向传播计算损失
        loss = model(input_ids=input_ids, attention_mask=attention_mask, labels=labels)

        # 处理可能的 NaN 损失
        if torch.isnan(loss):
            print("警告: 检测到 NaN 损失，跳过此批次。")
            continue

        # 反向传播
        loss.backward()

        # 可选：梯度裁剪，防止梯度爆炸
        # torch.nn.utils.clip_grad_norm_(model.parameters(), max_norm=1.0)

        # 更新参数
        optimizer.step()

        total_loss += loss.item()

    avg_loss = total_loss / len(dataloader) if len(dataloader) > 0 else 0
    return avg_loss

# --- 6. 主执行流程 ---

if __name__ == "__main__":
    # --- 配置参数 ---
    TRAIN_FILE = "small-chinese.txt" # 训练数据路径
    TEST_FILE = "Chinese.txt"       # 测试数据路径
    PRETRAINED_MODEL_NAME = 'bert-base-chinese' # 使用的预训练模型
    MAX_LEN = 128                   # 最大序列长度
    BATCH_SIZE = 8                 # 批处理大小 (根据 GPU 显存调整)
    EPOCHS = 5                      # 训练轮数 (可能需要更多)
    LEARNING_RATE = 2e-5            # 学习率 (BERT 微调常用值)

    # --- 检查文件是否存在 ---
    if not os.path.exists(TRAIN_FILE):
        print(f"错误: 训练文件 '{TRAIN_FILE}' 未找到。请确保文件存在于脚本所在目录或提供正确路径。")
        exit()
    if not os.path.exists(TEST_FILE):
        print(f"错误: 测试文件 '{TEST_FILE}' 未找到。请确保文件存在于脚本所在目录或提供正确路径。")
        exit()

    # --- 加载数据 ---
    print("正在加载数据...")
    train_texts, train_labels = read_bio_file(TRAIN_FILE)
    test_texts = read_plain_text_file(TEST_FILE)

    if not train_texts:
        print(f"错误: 未能从 '{TRAIN_FILE}' 加载任何训练数据。请检查文件内容和格式。")
        exit()
    if not test_texts:
        print(f"警告: 未能从 '{TEST_FILE}' 加载任何测试数据。预测将为空。")

    print(f"加载了 {len(train_texts)} 条训练样本。")
    print(f"加载了 {len(test_texts)} 条测试样本。")
    if test_texts:
        print(f"测试样本示例 (前 50 字符): {''.join(test_texts[0][:50])}...")

    # --- 初始化 Tokenizer ---
    print("正在初始化 Tokenizer...")
    tokenizer = BertTokenizerFast.from_pretrained(PRETRAINED_MODEL_NAME)

    # --- 创建 Dataset 和 DataLoader ---
    print("正在创建 Dataloaders...")
    train_dataset = NERDataset(train_texts, train_labels, tokenizer, MAX_LEN, label_map)
    test_dataset = TestDataset(test_texts, tokenizer, MAX_LEN)

    train_dataloader = DataLoader(train_dataset, batch_size=BATCH_SIZE, shuffle=True)
    # 测试时 batch_size=1 更容易处理对齐，但效率较低。也可以用更大 batch size，但预测循环需要相应修改。
    test_dataloader = DataLoader(test_dataset, batch_size=1)

    # --- 设置设备 ---
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    print(f"使用设备: {device}")

    # --- 初始化模型 ---
    print("正在初始化模型...")
    config = BertConfig.from_pretrained(PRETRAINED_MODEL_NAME, num_labels=NUM_LABELS)
    model = BertBiLSTMCRF.from_pretrained(PRETRAINED_MODEL_NAME, config=config, num_labels=NUM_LABELS)
    model.to(device)

    # --- 初始化优化器 ---
    optimizer = AdamW(model.parameters(), lr=LEARNING_RATE)

    # --- 训练模型 ---
    print("开始训练...")
    for epoch in range(EPOCHS):
        avg_train_loss = train_epoch(model, train_dataloader, optimizer, device)
        print(f'Epoch {epoch + 1}/{EPOCHS}, 平均训练损失: {avg_train_loss:.4f}')
    print("训练完成。")

    # --- 进行预测 ---
    print("开始在测试集上预测...")
    model.eval() # 设置为评估模式
    final_results = [] # 存储最终 (字符, 标签) 对

    with torch.no_grad():
        for batch in test_dataloader:
            # print("batch size, ",  batch)
            input_ids = batch['input_ids'].to(device)
            attention_mask = batch['attention_mask'].to(device)
            word_ids = batch['word_ids'].cpu().tolist() # shape (batch_size, seq_len)
            original_texts = batch['original_text'] # list of list of chars, batch_size=1 -> [ ['char1', 'char2', ...] ]

            predictions_ids_list = model(input_ids=input_ids, attention_mask=attention_mask) # list[list[int]]
            current_pred_ids = predictions_ids_list[0] # list[int], 长度为有效 token 数
            current_word_ids = word_ids[0] # list[int or None]
            current_original_text = original_texts[0] # list[str]
            predicted_labels = [label_map_inv.get(p_id, 'O') for p_id in current_pred_ids]
            char_labels = ['O'] * len(current_original_text)

            # 使用 word_ids 将 token 级别的预测映射回字符级别
            token_idx_in_preds = 0 # 指向 predicted_labels 的指针
            last_word_idx = -1

            for i, word_idx in enumerate(current_word_ids):
                if word_idx is None: # 跳过特殊 token [CLS], [SEP], [PAD]
                    continue

                # 检查 attention_mask，确保这是个有效 token (非 padding)
                # 并且确保我们还有对应的预测标签没用完
                if attention_mask[0, i] == 1 and token_idx_in_preds < len(predicted_labels):
                    # 如果是这个原始字符对应的第一个 token
                    if word_idx != last_word_idx:
                         # 检查 word_idx 是否在原始文本范围内 (防止意外)
                        if word_idx < len(char_labels):
                             # 将当前 token 的预测标签赋给对应的原始字符
                             char_labels[word_idx] = predicted_labels[token_idx_in_preds]

                    # 指向下一个预测标签 (无论是否是新词的第一个 token，只要是有效 token 都要消耗一个预测标签)
                    token_idx_in_preds += 1
                    last_word_idx = word_idx

                elif attention_mask[0, i] == 0:
                    # 遇到 padding，可以提前结束内层循环
                    break

            # 检查是否所有预测标签都被用上了 (可选调试)
            if token_idx_in_preds != len(predicted_labels):
                 print(f"警告: 并非所有预测标签都被对齐。已用: {token_idx_in_preds}, 总共: {len(predicted_labels)}")

            # 将原始字符和最终确定的标签配对
            aligned_result = list(zip(current_original_text, char_labels))
            final_results.append(aligned_result)

    print("预测完成。")

    # --- 打印预测结果 ---
    print("\n--- 测试集预测结果 ---")    
    if final_results:
        for i, sentence_result in enumerate(final_results):
            print(f"句子 {i+1}:")
            for char, tag in sentence_result:
                print(f"{char}\t{tag}")
            print("-" * 30)
    else:
        print("没有生成任何预测结果。")