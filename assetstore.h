#ifndef ASSETSTORE_H
#define ASSETSTORE_H


#include <string>


namespace fs = std::filesystem;

namespace SciStore {
class AssetStore{
    public:
        std::string db_name="default";
        AssetStore(){;}
        AssetStore(std::string p_name){db_name=p_name;}
        virtual void open() = 0;
        void close(){;}
        virtual void insert(std::string key, std::string value) = 0;
        virtual void insertFromFile(std::string file,  std::string path) = 0;
        virtual void create(std::string directory, int recursive) = 0;
        virtual void getSingle(std::string pdbid) = 0; //This should open the database, get a single value, and close the database again
        virtual void getSingleToFile(std::string pdbid, std::string fileextension) = 0; //This should open the database, get a single value, and close the database again
        virtual void createportable(std::string directory, int recursive) = 0;
        virtual void import(std::string sstfile) = 0;
        //copied from cppreference
        std::string get_stem(const fs::path &p) { return (p.stem().string()); }
        void create_file(const fs::path &p) { std::ofstream o{p}; }
};
}
#endif
