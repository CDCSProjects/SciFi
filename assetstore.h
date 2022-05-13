#ifndef ASSETSTORE_H
#define ASSETSTORE_H


#include <string>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <vector>

#include <assert.h>
#include <algorithm>
#include <zlib.h>

#include "downloader.h"


namespace fs = std::filesystem;

struct filepaths{
     std::string files;
     std::string file_names;
};

static bool comparep(filepaths fp1, filepaths fp2){return fp1.files<fp2.files;}

//copied from cppreference
static std::string get_stem(const fs::path &p) { return (p.stem().string()); }

namespace SciStore {

struct filedata{
    std::string key;
    std::string collection {"NULL"};
    bool compressed {false};
    std::string path {""};
    std::string fileextension {""};   
};

class AssetStore{
    public:

        AssetStore(){;}
        AssetStore(std::string p_name){db_name=p_name;}

        virtual void open() = 0;
        virtual void insert(std::string key, std::string value) = 0;
        virtual void remove(std::string key) = 0;
        virtual std::string getSingle(std::string pdbid) = 0;
        virtual void import(std::string sstfile) = 0;
        virtual int writePortable(std::vector<filepaths> fpv,filepaths fp, std::string directory, int recursive) = 0;
                
       // virtual std::vector<filedata> createportable(std::string directory, int recursive, int pathdepth, int ext, int removeprefixchar=0) = 0;
        
        void create_file(const fs::path &p) { std::ofstream o{p}; }
        void importsst(std::string sstfile) { return import(sstfile);} //We need this wrapper for our python API because apparently 'import' is a reserved keyword which cannot even be used as a function name
        void close(){;}
        void setOptions(){;}
      
        void init(std::string p_name);
        void insertFromFile(std::string file,  std::string path);
        void unzipAndCreate(std::string p_directory, int recursive=0, int pathdepth=0, int useext=0, int unzip=0);
        std::vector<filedata> create(std::string directory, int recursive=0, int depth =0, int useext=0, int removeprefixchar=0);
        void getSingleToFile(std::string pdbid, std::string fileextension) ; 
        std::vector<filedata> createportable(std::string directory, int recursive=0, int depth =0, int useext=0, int removeprefixchar=0);
        
        Downloader * dl;
        std::string db_name="default";
        std::string portablefile = "myassetstore.sst";
};


}
#endif
