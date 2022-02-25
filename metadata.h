#ifndef METADATA_H
#define METADATA_H

#include <string>
#include <iostream>
#include <vector>


namespace SciStore {
class MetaStorage{
    public:
        MetaStorage(std::string p_name) {};
        MetaStorage() {};
        ~MetaStorage() {};
        virtual int createNewDB() = 0;
        virtual int loadDB(std::string p_name) = 0;
        virtual void execQuery(std::string p_query) = 0;
        virtual void execQueryAndPrint(std::string p_query) = 0;
        virtual std::vector<std::string> getIDsByConstraint(std::string constraint) = 0;
        virtual void getSingle(std::string pdbid) = 0;
        virtual void getSingleToFile(std::string pdbid, std::string fileextension) = 0;

};
}
#endif

 