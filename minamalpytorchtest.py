import torch
from torchvision import transforms, datasets
from torch.utils.data import Dataset
from torchvision.transforms import ToTensor
import matplotlib.pyplot as plt
from torch.utils.data import DataLoader


training_data = datasets.ImageFolder("pytorchtest", transform=ToTensor())
# expects a folder wih other folder with images
# eg. pytorchtest/classlabel/image1.jpg
#     pytorchtest/classlabel/image2.jpg
#     pytorchtest/classlabel2/image1.jpg


train_dataloader = DataLoader(training_data, batch_size=1, shuffle=True)
# batch size is 1, usually it is higher 64 or so

for x in train_dataloader:
    # At this point we have np tensors. We can do whatever we want with them. Depends on what we want to benchmark.

    print("next")
    pass
    #for y in x:
    #    print(y.shape)
