import torch
from torchvision import transforms, datasets
from torch.utils.data import Dataset
from torchvision.transforms import ToTensor
import matplotlib.pyplot as plt
from torch.utils.data import DataLoader


training_data = datasets.FashionMNIST(
    root="testfiles/fashion",
    train=True,
    download=True,
    transform=ToTensor()
)


train_dataloader = DataLoader(training_data, batch_size=64, shuffle=True)


for x in train_dataloader:
    pass
