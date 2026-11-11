import torch
from torchtext.datasets import AG_NEWS
from torchtext.data.utils import get_tokenizer
from torchtext.vocab import build_vocab_from_iterator
from torch.utils.data import DataLoader
from torch.nn.utils.rnn import pad_sequence

# 加载数据集
train_iter, test_iter = AG_NEWS(split=('train', 'test'))
tokenizer = get_tokenizer('basic_english')

# 构建词汇表
def yield_tokens(data_iter):
    for _, text in data_iter:
        yield tokenizer(text)

vocab = build_vocab_from_iterator(yield_tokens(train_iter), specials=["<unk>"])
vocab.set_default_index(vocab["<unk>"])

# 数据处理
def text_pipeline(text):
    return torch.tensor([vocab[token] for token in tokenizer(text)], dtype=torch.int64)

def label_pipeline(label):
    return int(label) - 1

# DataLoader和批量数据处理
def collate_batch(batch):
    label_list, text_list = [], []
    for (_label, _text) in batch:
         label_list.append(label_pipeline(_label))
         text_list.append(text_pipeline(_text))
    return torch.tensor(label_list), pad_sequence(text_list, batch_first=True, padding_value=vocab["<unk>"])

train_dataloader = DataLoader(list(train_iter), batch_size=8, shuffle=True, collate_fn=collate_batch)
test_dataloader = DataLoader(list(test_iter), batch_size=8, shuffle=False, collate_fn=collate_batch)

import torch.nn as nn

device = 'cpu'

class FullyConnectedModel(nn.Module):
    def __init__(self, vocab_size, embed_dim, num_class):
        super(FullyConnectedModel, self).__init__()
        self.embedding = nn.EmbeddingBag(vocab_size, embed_dim, sparse=True)
        self.fc = nn.Linear(embed_dim, num_class)

    def forward(self, text):
        embedded = self.embedding(text)
        return self.fc(embedded)

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

embed_dim = 64
hidden_dim = 128
num_class = 4  # AG_NEWS 有4个类别

vocab_size = len(vocab)
fc_model = FullyConnectedModel(vocab_size, embed_dim, num_class).to(device)
cnn_model = CNNModel(vocab_size, embed_dim, num_class).to(device)
rnn_model = RNNModel(vocab_size, embed_dim, hidden_dim, num_class).to(device)

import torch.nn.functional as F
import torch.optim as optim

model = cnn_model
num_epochs = 10
learning_rate = 0.001
criterion = nn.CrossEntropyLoss()
optimizer = optim.Adam(model.parameters(), lr=learning_rate)

def train(model, dataloader, optimizer, criterion, num_classes):
    model.train()
    total_acc, total_count = 0, 0
    for label, text in dataloader:
        label = label.to(device)
        text = text.to(device)
        
        optimizer.zero_grad()
        predicted_label = model(text)
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
        for label, text in dataloader:
            label = label.to(device)
            text = text.to(device)
            predited_label = model(text)
            total_acc += (predited_label.argmax(1) == label).sum().item()
            total_count += label.size(0)
    return total_acc / total_count if total_count > 0 else 0

for epoch in range(num_epochs):
    train_acc = train(model, train_dataloader, optimizer, criterion, num_classes=num_class)
    print(f'Epoch: {epoch}, Train accuracy: {train_acc:.4f}')

test_acc = evaluate(model, test_dataloader)
print(f'Test accuracy: {test_acc:.4f}')