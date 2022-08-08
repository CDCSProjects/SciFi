#!/usr/bin/env python3
from SciFiDataset import SciFiDataset

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
    
