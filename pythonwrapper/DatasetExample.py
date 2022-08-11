#!/usr/bin/env python3
from SciFiDataset import SciFiDataset

#Create a dataset
scifi_dataset = SciFiDataset(datasetname="../My_Assetstore", metaname="../Test_Metastore")

#Return an asset by its unique ID
sample=scifi_dataset["a1"]

#This is a test to see if our asset (a picture in our case) is returned correctly
with open('test.jpg', 'wb') as f:
    f.write(sample)

#Get the number of assets in the asset store. This looks up the number of entries in the automatcally created meta data
print(len(scifi_dataset))

#Return and print all IDs 
list = scifi_dataset.getIDsByIndex(0,len(scifi_dataset))
print (list)

#Iterate over all entries
j=0
for i in list:
    #todo create a new metadataset which includes the file ending. I somehow ended up using sst files and db files which do not belong together
    sample=scifi_dataset[i]
    print(i)
    with open(str(j)+'.jpg', 'wb') as f:
        f.write(sample) #Write data to file to see if it worked
    j=j+1
    
#Return and print all unique IDs where the attribute 'number' is larger than 10
metalist = scifi_dataset.getIDsByMeta("number>10")
print(metalist)
    
