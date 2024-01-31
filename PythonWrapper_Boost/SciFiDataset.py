#!/usr/bin/env python3
import boost_scifi
import torch
from torchvision import transforms, datasets
from torch.utils.data import Dataset
from torchvision.transforms import ToTensor
from torch.utils.data import DataLoader


class SciFiDataset(torch.utils.data.Dataset):
    def __init__(self, datasetname, metaname):
        super(SciFiDataset).__init__()
        self.storage = boost_scifi.Storage(datasetname, metaname)


    def __getitem__(self, index):
        return self.storage.get_item(index)

    def __len__(self):
        return self.storage.asset_count()

    def getIDsByIndex(self, min, max):
        number = max - min
        query= "Select key from filedata order by key limit ".encode('utf-8') + str(number).encode(
                'utf-8') + " offset ".encode('utf-8') + str(min).encode('utf-8')
        res = self.storage.get_IDsByIndex(query)
        # Our result is a formatted string, let's make a list of keys out of this
        res = (res.split(']', 1)[1])
        res = res.split("\n")
        res[0] = res[0].replace("\n", '')
        res = list(filter(None, res))
        return res

    def getIDsByMeta(self, meta):
        res = self.storage.get_IDs_by_constraint(meta)
        return res

    def getIDsFromFilter(self, filtername):
        res = self.storage.apply_filter(filtername)
        return res

