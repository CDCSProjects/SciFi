#ifndef ROCKSSTORE_H
#define ROCKSSTORE_H


#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include <assert.h>
#include <algorithm>
#include <zlib.h>

#include "rocksdb/include/rocksdb/db.h"
#include "assetstore.h"
#include "downloader.h"

using namespace rocksdb;

struct filepaths{
     std::string files;
     std::string file_names;
};

static bool comparep(filepaths fp1, filepaths fp2){return fp1.files<fp2.files;}

namespace SciStore {

class RocksStore : public AssetStore{
    public:
        RocksStore(std::string name);
        void open();
        void insert(std::string key, std::string value);
        void unzipAndCreate(std::string p_directory, int recursive=0, int pathdepth=0, int useext=0, int unzip=0);
        std::vector<filedata> create(std::string directory, int recursive=0, int depth =0, int useext=0, int removeprefixchar=0);
        void getSingle(std::string pdbid);
        void createportable(std::string directory, int recursive=0, int depth =0, int useext=0);
        void insertFromFile(std::string file,  std::string path="");
        void getSingleToFile(std::string pdbid, std::string fileextension);
        void import(std::string sstfile);
        void remove(std::string key);
    protected:
        std::string portablefile = "myassetstore.sst";
        DB* database=NULL;
        Downloader * dl;
        
};



}
#endif
