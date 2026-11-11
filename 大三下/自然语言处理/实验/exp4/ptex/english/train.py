from datasets import load_dataset, load_metric
from transformers import BertTokenizerFast, DataCollatorForTokenClassification
from torch.utils.data import DataLoader
from model.model import BERT_BiLSTM_CRF
from utils.data_loader import tokenize_and_align_labels
import torch
from tqdm import tqdm

# 超参数
model_name = "bert-base-cased"
hidden_dim = 256
batch_size = 16
epochs = 3
device = torch.device("cuda" if torch.cuda.is_available() else "cpu")

# 加载数据与标签
dataset = load_dataset("conll2003")
label_list = dataset["train"].features["ner_tags"].feature.names
tag2id = {tag: i for i, tag in enumerate(label_list)}
id2tag = {i: tag for tag, i in tag2id.items()}

# 分词与标签对齐
tokenizer = BertTokenizerFast.from_pretrained(model_name)
encoded_dataset = dataset.map(lambda x: tokenize_and_align_labels(x, tokenizer),
                              batched=True)

# 创建 dataloader
collator = DataCollatorForTokenClassification(tokenizer)
train_loader = DataLoader(encoded_dataset["train"], batch_size=batch_size, shuffle=True, collate_fn=collator)
valid_loader = DataLoader(encoded_dataset["validation"], batch_size=batch_size, collate_fn=collator)

# 初始化模型
model = BERT_BiLSTM_CRF(model_name, hidden_dim, tag2id).to(device)
optimizer = torch.optim.AdamW(model.parameters(), lr=5e-5)

# 训练循环
for epoch in range(epochs):
    model.train()
    total_loss = 0
    for batch in tqdm(train_loader):
        input_ids = batch["input_ids"].to(device)
        attention_mask = batch["attention_mask"].to(device)
        labels = batch["labels"].to(device)

        loss = model(input_ids, attention_mask, labels)
        optimizer.zero_grad()
        loss.backward()
        optimizer.step()
        total_loss += loss.item()
    print(f"Epoch {epoch + 1} Loss: {total_loss:.4f}")
from datasets import load_dataset, load_metric
from transformers import BertTokenizerFast, DataCollatorForTokenClassification
from torch.utils.data import DataLoader
from model.model import BERT_BiLSTM_CRF
from utils.data_loader import tokenize_and_align_labels
import torch
from tqdm import tqdm

# 超参数
model_name = "bert-base-cased"
hidden_dim = 256
batch_size = 16
epochs = 3
device = torch.device("cuda" if torch.cuda.is_available() else "cpu")

# 加载数据与标签
dataset = load_dataset("conll2003")
label_list = dataset["train"].features["ner_tags"].feature.names
tag2id = {tag: i for i, tag in enumerate(label_list)}
id2tag = {i: tag for tag, i in tag2id.items()}

# 分词与标签对齐
tokenizer = BertTokenizerFast.from_pretrained(model_name)
encoded_dataset = dataset.map(lambda x: tokenize_and_align_labels(x, tokenizer),
                              batched=True)

# 创建 dataloader
collator = DataCollatorForTokenClassification(tokenizer)
train_loader = DataLoader(encoded_dataset["train"], batch_size=batch_size, shuffle=True, collate_fn=collator)
valid_loader = DataLoader(encoded_dataset["validation"], batch_size=batch_size, collate_fn=collator)

# 初始化模型
model = BERT_BiLSTM_CRF(model_name, hidden_dim, tag2id).to(device)
optimizer = torch.optim.AdamW(model.parameters(), lr=5e-5)

# 训练循环
for epoch in range(epochs):
    model.train()
    total_loss = 0
    for batch in tqdm(train_loader):
        input_ids = batch["input_ids"].to(device)
        attention_mask = batch["attention_mask"].to(device)
        labels = batch["labels"].to(device)

        loss = model(input_ids, attention_mask, labels)
        optimizer.zero_grad()
        loss.backward()
        optimizer.step()
        total_loss += loss.item()
    print(f"Epoch {epoch + 1} Loss: {total_loss:.4f}")
