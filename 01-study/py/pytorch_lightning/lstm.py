import pytorch_lightning as pl
import torch
from torch import nn
from torch.nn import functional as F
from pytorch_lightning import Trainer
from torch.utils.data import DataLoader, Dataset

class LSTMModel(pl.LightningModule):
    def __init__(self, input_dim, hidden_dim, output_dim, num_layers):
        super(LSTMModel, self).__init__()
        self.lstm = nn.LSTM(input_size=input_dim, hidden_size=hidden_dim,
                 num_layers=num_layers, batch_first=True) # N, L, input_dim
        # 输出N, L, hidden_dim
        self.linear = nn.Linear(hidden_dim, output_dim) 
    
    def forward(self, x):
        lstm_out, _ = self.lstm(x)
        # 取最后一个时间步的输出
        last_time_step_out = lstm_out[:, -1, :]
        return self.linear(last_time_step_out) # 输出N, output_dim
    
    # # 分类
    # def training_step(self, batch, batch_idx):
    #     x, y = batch
    #     y_hat = self(x)
    #     print(y_hat.shape, y.shape)
    #     loss = F.cross_entropy(y_hat, y)
    #     self.log('train_loss', loss)
    #     return loss

    # 回归
    def training_step(self, batch, batch_idx):  # LightningModule API, 用于Trainer.fit
        x, y = batch
        y_hat = self(x)
        # 使用均方误差作为回归任务的损失函数
        loss = F.mse_loss(y_hat, y)
        self.log('train_loss', loss)
        return loss
    
    def test_step(self, batch, batch_idx):      # LightningModule API, 用于Trainer.test
        x, y = batch
        y_hat = self(x)
        loss = F.mse_loss(y_hat, y)
        self.log('test_loss', loss)
        return loss

    def configure_optimizers(self):
        return torch.optim.Adam(self.parameters(), lr=0.001)


class CustomDataset(Dataset):
    def __init__(self, data, targets):
        self.data = data
        self.targets = targets

    def __len__(self):
        return len(self.data)

    def __getitem__(self, idx):
        item = self.data[idx]
        label = self.targets[idx]
        return item, label

# 假设的数据维度
input_dim = 10
hidden_dim = 50
output_dim = 5
num_layers = 1
L = 11

# 示例数据和标签
data = torch.randn(100, L, input_dim)  # 假设我们有 100 个样本，每个样本是 11 个时间步长，每个时间步长是 10 维

# # 分类
# targets = torch.randint(0, output_dim, (100,))  # 假设有 5 个类别
# 回归
targets = torch.randn(100, output_dim)  # 假设输出是 output_dim 维

# 创建 Dataset 和 DataLoader
dataset = CustomDataset(data, targets)
train_dataloader = DataLoader(dataset, batch_size=8, shuffle=True)


# 创建模型实例
model = LSTMModel(input_dim, hidden_dim, output_dim, num_layers)



# 创建 Trainer 实例，假设我们在 CPU 上训练 10 轮
trainer = Trainer(max_epochs=10)

# 开始训练，假设 train_dataloader 是你的数据加载器
trainer.fit(model, train_dataloaders=train_dataloader)
# 假设我们有一批新的输入数据 x
# x 的形状应该与训练时期望的输入形状相匹配
x = torch.randn(10, L, input_dim)
predictions = model(x)

# 输出预测结果
print(predictions)

# 假设 test_data 和 test_targets 已经准备好
test_data = torch.randn(20, 10, input_dim)  # 20 个样本的测试数据
test_targets = torch.randn(20, output_dim)  # 对应的 20 个样本的输出维度

test_dataset = CustomDataset(test_data, test_targets)
test_dataloader = DataLoader(test_dataset, batch_size=10, shuffle=False)
trainer.test(model, dataloaders=test_dataloader)