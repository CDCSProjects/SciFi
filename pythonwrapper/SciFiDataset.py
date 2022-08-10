#!/usr/bin/env python3
import scifi
import torch
from torchvision import transforms, datasets
from torch.utils.data import Dataset
from torchvision.transforms import ToTensor
from torch.utils.data import DataLoader

class SciFiDataset(torch.utils.data.Dataset):
    def __init__(self, datasetname, metaname):
        super(SciFiDataset).__init__()
        self.datasetname = datasetname
        self.rocks = scifi.PyRocksStore(datasetname.encode('utf-8'))
        self.rocks.open()
        self.duck = scifi.PyDuckStore(metaname.encode('utf-8'))
    #    self.duck.initDB(metaname.encode('utf-8'))
    
    def __getitem__(self, index):
        return self.rocks.getSingle(index.encode('utf-8'))
        
    def __len__(self):
        #rockdb only returns an esimated number of keys. Thus, we use our automatically generated metadata to get the count
        self.duck.execQuery("Select count(*) from filedata".encode('utf-8'))
        res=self.duck.getResultAsString().decode("utf-8").split(']',1)[1]
        return int(res)

    def getIDsByIndex(self,min,max):
        number= max-min
        self.duck.execQuery("Select key from filedata order by key limit ".encode('utf-8') + str(number).encode('utf-8') + " offset ".encode('utf-8') + str(min).encode('utf-8'))
        res=self.duck.getResultAsString().decode("utf-8").rstrip()
        # Our result is a formatted string, let's make a list of keys out of this
        res=(res.split(']',1)[1])
        res=res.split("\t\n")
        res[0]=res[0].replace("\n", '')
        res=list(filter(None,res))
        return res
        
    def getIDsByMeta(self,meta):
        res = [x.decode('utf-8') for x in self.duck.getIDsByConstraint(meta.encode('utf-8'))]
        return res
 