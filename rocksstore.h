#ifndef ROCKSSTORE_H
#define ROCKSSTORE_H


#include <iostream>
#include <fstream>
#include <filesystem>
#include <assert.h>
#include "rocksdb/include/rocksdb/db.h"
#include "assetstore.h"
#include "downloader.h"

using namespace rocksdb;

namespace SciStore {
class RocksStore : public AssetStore{
    public:
        RocksStore(std::string name);
        void open();
        void insert(std::string key, std::string value);
        void create(std::string directory, int recursive=0, int depth =0, int useext=0);
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
