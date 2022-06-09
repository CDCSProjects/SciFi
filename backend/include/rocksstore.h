#ifndef ROCKSSTORE_H
#define ROCKSSTORE_H




#include "rocksdb/include/rocksdb/db.h"
#include <framework/include/assetstore.h>


using namespace rocksdb;

namespace SciStore {

class RocksStore : public AssetStore{
    public:
        RocksStore(std::string name);
        void open();
        void insert(std::string key, std::string value);
        //void unzipAndCreate(std::string p_directory, int recursive=0, int pathdepth=0, int useext=0, int unzip=0);
        //std::vector<filedata> create(std::string directory, int recursive=0, int depth =0, int useext=0, int removeprefixchar=0);
        std::string getSingle(std::string pdbid);
       // std::vector<filedata> createportable(std::string directory, int recursive=0, int depth =0, int useext=0, int removeprefixchar=0);
        //void insertFromFile(std::string file,  std::string path="");
        //void getSingleToFile(std::string pdbid, std::string fileextension);
        void import(std::string sstfile);
        void remove(std::string key);
        void setOptions();
        int writePortable(std::vector<filepaths> fpv,filepaths fp, std::string directory, int recursive);
    protected:
        DB* database=NULL;
        Options options;

        
};



}
#endif
