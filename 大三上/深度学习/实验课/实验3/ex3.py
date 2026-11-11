import os 
os.chdir ('D:/大学/大三上/深度学习/实验课/实验3')
import torch
import torch.nn as nn
import torch.nn.functional as F
import numpy
from torchvision  import datasets as dsets, transforms
import matplotlib.pyplot as plt
train_data = dsets.MNIST(root = 'D:/大学/大三上/深度学习/实验课/实验3', train = True,
                        transform = transforms.ToTensor(), download = True)

test_data = dsets.MNIST(root = 'D:/大学/大三上/深度学习/实验课/实验3', train = False,
                       transform = transforms.ToTensor())
batch_size = 10
train_gen = torch.utils.data.DataLoader(dataset = train_data,
                                             batch_size = batch_size,
                                             shuffle = True)

test_gen = torch.utils.data.DataLoader(dataset = test_data,
                                      batch_size = batch_size, 
                                      shuffle = False)

# # 从训练数据集中获取一批数据
# data_iter = iter(train_gen)
# images, labels = data_iter.next()
# # 绘制前10个手写字符
# fig, axes = plt.subplots(1, 10, figsize=(15, 1.5))
# for i in range(10):
#     ax = axes[i]
#     ax.imshow(images[i].numpy().squeeze(), cmap='gray')
#     ax.set_title(f'Label: {labels[i].item()}')
#     ax.axis('off')

# plt.show()

class FullyConnectedNN(nn.Module):
    def __init__(self, input_size, hidden_size1, hidden_size2, output_size):
    # def __init__(self, input_size, output_size):
        super(FullyConnectedNN, self).__init__()
        self.fc1 = nn.Linear(input_size, hidden_size1)  # 输入层到第一个隐藏层
        self.fc2 = nn.Linear(hidden_size1, hidden_size2)  # 第一个隐藏层到第二个隐藏层
        self.fc3 = nn.Linear(hidden_size2, output_size)  # 第二个隐藏层到输出层
        # self.fc = nn.Linear(input_size, output_size)
    def forward(self, x):
        x = F.relu(self.fc1(x))  # 使用ReLU激活函数
        x = F.relu(self.fc2(x))
        x = self.fc3(x)  # 输出层不使用激活函数
        # x = F.relu(self.fc(x))
        return x

# 设置模型参数
input_size = 28 * 28  # MNIST图像大小为28x28
hidden_size1 = 128
hidden_size2 = 64
output_size = 10  # MNIST有10个类别

# 实例化模型
model = FullyConnectedNN(input_size, hidden_size1, hidden_size2, output_size)
# model = FullyConnectedNN(input_size, output_size)
# print(model)
device = 'cpu'
# if torch.cuda.is_available():
#     model.cuda()
#     device = 'cuda:0'

criterion = nn.CrossEntropyLoss()
optimizer = torch.optim.Adam(model.parameters(), lr=0.001)

num_epochs = 5

for epoch in range(num_epochs):
    for i, (images, labels) in enumerate(train_gen):
        images = images.view(-1, 28*28).to(device)  # 将图像和标签移动到设备
        labels = labels.to(device)
        
        outputs = model(images)
        loss = criterion(outputs, labels)
        optimizer.zero_grad()
        loss.backward()
        optimizer.step()
        
        if (i+1) % 100 == 0:
            print(f'Epoch [{epoch+1}/{num_epochs}], Step [{i+1}/{len(train_gen)}], Loss: {loss.item():.4f}')

model.eval()
with torch.no_grad():
    correct = 0
    total = 0
    for images, labels in test_gen:
        images = images.view(-1, 28*28).to(device)
        labels = labels.to(device)
        outputs = model(images)
        _, predicted = torch.max(outputs.data, 1)
        correct += (predicted == labels).sum().item()
        total += labels.size(0)
    
    print(f'Accuracy of the model on the 10000 test images: {100 * correct / total:.2f}%')

# correct = 0
# total = 0
# for images, labels in test_gen:
#     images = torch.tensor(images.view(-1, 28*28)).to(device)
#     labels = labels.to(device)
    
#     output = model(images)  # 使用 model 而不是 net
#     _, predicted = torch.max(output, 1)
#     correct += (predicted == labels).sum().item()
#     total += labels.size(0)

# print('Accuracy of the model: %.3f %%' % (100 * correct / total))
