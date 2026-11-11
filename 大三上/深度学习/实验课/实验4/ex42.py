import torch
from torchtext.datasets import AG_NEWS
from torchtext.data.utils import get_tokenizer
from torchtext.vocab import build_vocab_from_iterator
from torch.utils.data import DataLoader
from torch.nn.utils.rnn import pad_sequence

# 加载数据集
train_iter = AG_NEWS(split='train')
tokenizer = get_tokenizer('basic_english')
# 构建词汇表
def yield_tokens(data_iter):
    for _, text in data_iter:
        yield tokenizer(text)
vocab = build_vocab_from_iterator(yield_tokens(train_iter), specials=["<unk>"])
vocab.set_default_index(vocab["<unk>"])
# 数据处理
def text_pipeline(text):
    return vocab(tokenizer(text))

def label_pipeline(label):
    return int(label) - 1

# DataLoader和批量数据处理
def collate_batch(batch):
    label_list, text_list = [], []
    for (_label, _text) in batch:
         label_list.append(label_pipeline(_label))
         text_list.append(torch.tensor(text_pipeline(_text), dtype=torch.int64))
    return torch.tensor(label_list), pad_sequence(text_list, batch_first=True, padding_value=0)
train_iter, test_iter = AG_NEWS(split=('train', 'test'))
train_dataloader = DataLoader(list(train_iter), batch_size=8, shuffle=True, collate_fn=collate_batch)
test_dataloader = DataLoader(list(test_iter), batch_size=8, shuffle=False, collate_fn=collate_batch)

import torch.nn as nn
# print(torch.__version__)          # 查看 PyTorch 版本
# print(torch.cuda.is_available())  # 检查是否可用 CUDA
device = 'cpu'
class FullyConnectedModel(nn.Module):
    def __init__(self, vocab_size, embed_dim, num_class):
        super(FullyConnectedModel, self).__init__()
        self.embedding = nn.EmbeddingBag(vocab_size, embed_dim, sparse=True)
        self.fc = nn.Linear(embed_dim, num_class)

    def forward(self, text):
        embedded = self.embedding(text)
        return self.fc(embedded)

num_class = len(set([label for (label, _) in train_iter]))
# fc_model = FullyConnectedModel(len(vocab), embed_dim, num_class).to(device)


class CNNModel(nn.Module):
    def __init__(self, vocab_size, embed_dim, num_class, kernel_sizes=[3, 4, 5], num_channels=100):
        super(CNNModel, self).__init__()
        self.embedding = nn.Embedding(vocab_size, embed_dim)
        self.convs = nn.ModuleList([
            nn.Conv2d(1, num_channels, (k, embed_dim)) for k in kernel_sizes
        ])
        self.fc = nn.Linear(num_channels * len(kernel_sizes), num_class)

    def forward(self, text):
        embedded = self.embedding(text).unsqueeze(1)
        conv_outs = [torch.relu(conv(embedded)).squeeze(3) for conv in self.convs]
        pooled_outs = [torch.max(out, 2)[0] for out in conv_outs]
        cat_out = torch.cat(pooled_outs, 1)
        return self.fc(cat_out)

# cnn_model = CNNModel(len(vocab), embed_dim, num_class).to(device)

class RNNModel(nn.Module):
    def __init__(self, vocab_size, embed_dim, hidden_dim, num_class):
        super(RNNModel, self).__init__()
        self.embedding = nn.Embedding(vocab_size, embed_dim)
        self.rnn = nn.LSTM(embed_dim, hidden_dim, bidirectional=True, batch_first=True)
        self.fc = nn.Linear(hidden_dim * 2, num_class)

    def forward(self, text):
        embedded = self.embedding(text)
        rnn_out, _ = self.rnn(embedded)
        final_feature_map = rnn_out[:, -1, :]
        return self.fc(final_feature_map)

# rnn_model = RNNModel(len(vocab), embed_dim, hidden_dim, num_class).to(device)
hidden_dim = 128

import torch.nn.functional as F
import random
def train(model, dataloader, optimizer, criterion, batch_size=128, num_classes=4):
    model.train()
    total_acc, total_count = 0, 0
    
    # 随机选择一批数据的索引
    indices = random.sample(range(len(dataloader.dataset)), batch_size)

    for idx in indices:
        label, text = dataloader.dataset[idx]

        if isinstance(text, str):
            text = text_pipeline(text)
            text = torch.tensor(text).unsqueeze(0)
        else:
            text = torch.tensor(text).unsqueeze(0)
        
        label = torch.tensor(label).unsqueeze(0)

        # 转移到设备
        text = text.to(device)
        label = label.to(device)

        optimizer.zero_grad()
        predicted_label = model(text)

        # 调试信息
        # print(f"Predicted: {predicted_label.argmax(1)}, Target: {label}")

        # 检查 label 是否超出范围
        if label.item() >= num_classes:
            # print(f"Invalid label value: {label.item()} (max class index: {num_classes - 1})")
            continue  # 跳过这次迭代，避免计算损失

        loss = criterion(predicted_label, label)
        loss.backward()
        optimizer.step()
        
        total_acc += (predicted_label.argmax(1) == label).sum().item()
        total_count += label.size(0)
    return total_acc / total_count if total_count > 0 else 0
def evaluate(model, dataloader):
    model.eval()
    total_acc, total_count = 0, 0
    with torch.no_grad():
        for idx, (label, text) in enumerate(dataloader):
            predited_label = model(text.to(device))
            # predited_label = model(text)
            total_acc += (predited_label.argmax(1) == label.to(device)).sum().item()
            total_count += label.size(0)
            # total_acc += (predited_label.argmax(1) == label).sum().item()
            # total_count += label.size(0)
    return total_acc/total_count

import torch.optim as optim

# 假设 vocab 和 train_iter 已定义
vocab_size = len(vocab)

embed_dim = 64
hidden_dim = 128
num_class = len(set([label for (label, _) in train_iter]))

# 创建模型
fc_model = FullyConnectedModel(vocab_size, embed_dim, num_class).to(device)
cnn_model = CNNModel(vocab_size, embed_dim, num_class).to(device)
rnn_model = RNNModel(vocab_size, embed_dim, hidden_dim, num_class).to(device)
model = cnn_model
num_epochs = 30
learning_rate = 0.001
criterion = nn.CrossEntropyLoss()
optimizer = optim.Adam(cnn_model.parameters(), lr=learning_rate)
for epoch in range(num_epochs):
    train_acc = train(model, train_dataloader, optimizer, criterion)
    print(f'Epoch: {epoch}, Train accuracy: {train_acc:.4f}')
# test_acc = evaluate(model,test_dataloader)
test_acc = train(model, test_dataloader)
print(f'Test accuracy: {test_acc:.4f}')