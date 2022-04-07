#ifndef ASSETSTORE_H
#define ASSETSTORE_H


#include <string>


namespace fs = std::filesystem;

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
        std::string db_name="default";
        AssetStore(){;}
        AssetStore(std::string p_name){db_name=p_name;}
        virtual void open() = 0;
        void close(){;}
        virtual void insert(std::string key, std::string value) = 0;
        virtual void insertFromFile(std::string file,  std::string path) = 0;
        virtual void unzipAndCreate(std::string p_directory, int recursive=0, int pathdepth=0, int useext=0, int unzip=0) = 0;
        virtual std::vector<filedata> create(std::string directory, int recursive, int depth , int ext, int removeprefixchar) = 0;
        virtual void getSingle(std::string pdbid) = 0; //This should open the database, get a single value, and close the database again
        virtual void getSingleToFile(std::string pdbid, std::string fileextension) = 0; //This should open the database, get a single value, and close the database again
        virtual void createportable(std::string directory, int recursive, int pathdepth, int ext) = 0;
        virtual void import(std::string sstfile) = 0;
        //copied from cppreference
        std::string get_stem(const fs::path &p) { return (p.stem().string()); }
        void create_file(const fs::path &p) { std::ofstream o{p}; }
        void importsst(std::string sstfile) { return import(sstfile);} //We need this wrapper for our python API because apparently 'import' is a reserved keyword which cannot even be used as a function name
};


}
#endif
