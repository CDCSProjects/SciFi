#ifndef METADATA_H
#define METADATA_H

#include <fstream>
#include <string>
#include <iostream>
#include <vector>


namespace SciStore {
class MetaStorage{
    public:
        MetaStorage(std::string p_name);
        MetaStorage();
        ~MetaStorage() {};
        
        virtual int createNewDB() = 0;
        int loadDB(std::string p_name) {return 0;}
        virtual void execQueryAndPrint(std::string p_query) = 0;
        virtual void printResult() = 0;
        virtual std::vector<std::string> getIDsByConstraint(std::string constraint) = 0;
        virtual void execQuery(std::string p_query) = 0;
        virtual std::string getResultAsString() = 0;
        virtual void openAndConnect() = 0;
        virtual void openAndConnect(std::string name) = 0;
        virtual std::vector<std::string> getIDsByFileData() = 0;
        
        void getSingle(std::string pdbid);
        void getSingleToFile(std::string pdbid, std::string fileextension);
        void writeResultToFile(std::string filename, std::string fileextension);
        void init();
        void init(std::string p_name);
        
        std::string idcolumn = "rowid";
        std::string resultfolder = "";

};
}
#endif

 