import torch
#随机构造线性数据
import math
import matplotlib.pyplot as plt
from torch import nn
from torch.utils.data import DataLoader, TensorDataset
class LinearRegression(nn.Module):
    def __init__(self):
        super(LinearRegression, self).__init__()
        self.linear = nn.Linear(1, 1)

    def forward(self, x):
        return self.linear(x)

#清空模型，初始化模型参数
def reset_model_parameters(model):
    for layer in model.children():
        if hasattr(layer, 'reset_parameters'):
            layer.reset_parameters()

# def draw_picture(model, x):
#     y_pre = model(x)
#     plt.scatter(x.numpy(), y.numpy(), label = 'Original Data')
#     plt.plot(x.numpy(), y_pre.detach().numpy(), label = 'Fitted Line', color = 'red')
#     plt.legend()
#     plt.show()
nums_data = 30
x = torch.linspace(0, 1,nums_data)
x = torch.unsqueeze(x,dim=1)
k = 2
y = k * x + torch.rand(x.size())
criterion =  nn.MSELoss()
model = LinearRegression()
learning_rate = 0.01
optimizer = torch.optim.SGD(model.parameters(), lr=learning_rate)#优化函数

dataset = TensorDataset(x, y)
batch_size = 10
dataloader = DataLoader(dataset, batch_size = batch_size, shuffle = True)

epoch_num = 1
step_num = math.ceil(nums_data / batch_size)
x = torch.linspace(0, 1,nums_data)
plt.scatter(x.numpy(), y.numpy(), label = 'Original Data')

for epoch in range(epoch_num):
    for step in range(step_num):
        for batch_x, batch_y in dataloader:
            y_pre = model(batch_x)
            loss = criterion(y_pre, batch_y)
            optimizer.zero_grad()
            loss.backward()
            optimizer.step()

plt.plot(batch_x.numpy(), y_pre.detach().numpy(), label = 'Fitted Line1', color = 'red')

reset_model_parameters(model)
epoch_num = 10
step_num = math.ceil(nums_data / batch_size)
for epoch in range(epoch_num):
    for step in range(step_num):
        for batch_x, batch_y in dataloader:
            y_pre = model(batch_x)
            loss = criterion(y_pre, batch_y)
            optimizer.zero_grad()
            loss.backward()
            optimizer.step()

plt.plot(batch_x.numpy(), y_pre.detach().numpy(), label = 'Fitted Line2', color = 'green')
reset_model_parameters(model)

epoch_num = 1000
step_num = math.ceil(nums_data / batch_size)
for epoch in range(epoch_num):
    for step in range(step_num):
        for batch_x, batch_y in dataloader:
            y_pre = model(batch_x)
            loss = criterion(y_pre, batch_y)
            optimizer.zero_grad()
            loss.backward()
            optimizer.step()

plt.plot(batch_x.numpy(), y_pre.detach().numpy(), label = 'Fitted Line3', color = 'blue')

# plt.figure()


# y_pre = model(x)
# plt.plot(x.numpy(), y_pre.detach().numpy(), label = 'Fitted Line2', color = 'green')
# print(1)


# plt.scatter(x.numpy(), y.numpy(), label = 'Original Data')
plt.legend()
plt.show()

#是否画图
# if if_plot:
#     plt.scatter(x.numpy(),y.numpy(),c=x.numpy())
#     plt.show()
# data = {"x":x, "y":y}

# print(1222)