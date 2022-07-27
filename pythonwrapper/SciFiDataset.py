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
        #return self.rocks.getSingleToFile(index.encode('utf-8'), "".encode('utf-8'))
        if index.startswith('meta:'):
            self.duck.getIDsByConstraint(index[len('meta:'):].encode('utf-8'))
        else:
            return self.rocks.getSingle(index.encode('utf-8'))
        
    def __len__(self):
        #rockdb only returns an esimated number of keys. Thus, we use our automatically generated metadata to get the count
        self.duck.execQuery("Select count(*) from filedata".encode('utf-8'))
        res=self.duck.getResultAsString().decode("utf-8").split(']',1)[1]
        return int(res)

    def getIDsByIndex(self,min,max):
        number= max-min
        self.duck.execQuery("Select key from filedata order by key limit ".encode('utf-8') + str(number).encode('utf-8') + " offset ".encode('utf-8') + str(min).encode('utf-8'))
        # Our result is a formatted string, let's make a list of keys out of this
        res=(self.duck.getResultAsString().decode("utf-8").split(']',1)[1])
        res=res.split("\t\n")
        res[0]=res[0].replace('\n', '')
        res=list(filter(None,res))
        return res
        
scifi_dataset = SciFiDataset(datasetname="galaxyasset", metaname="galaxymeta")


sample=scifi_dataset["FRII/3.198_0.788_1_Capetti2017b.png"]

with open('test.png', 'wb') as f:
    f.write(sample)

print(len(scifi_dataset))

#list = scifi_dataset.getIDsByIndex(10,len(scifi_dataset))
list = scifi_dataset.getIDsByIndex(9010,9020)
print (list)

j=0
for i in list:
    #todo create a new metadataset which includes the file ending. I somehow ended up using sst files and db files which do not belong together
    sample=scifi_dataset[i+'.png']
    print(i)
    with open(str(j)+'.png', 'wb') as f:
        f.write(sample)
    j=j+1
    

sample2=scifi_dataset["meta:ra_j2000_degree < 200"]

        