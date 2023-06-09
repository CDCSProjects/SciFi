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
        virtual std::string crop_single_result(std::string res) = 0; //!Even when returning only a single record as a result, systems tend to add additional characters, e.g. spaces or line breaks. This function is supposed to get rid of these additional characters. 
        virtual std::string crop_result(std::string res) = 0;//!Remove any additional information from he result string that is not the plain tuples
        virtual std::vector<std::string> get_user_meta_column_names() = 0;//!Return a vector containing the names of the columns including the user meta data
        virtual std::vector<std::string> crop_and_split_result(std::string res) = 0;//!Return a vector where all tuples are split into individual strings, such that each element is a single attribute of a single tuple
        std::string idcolumn = "rowid";
        std::string resultfolder = "";
        bool get_isNumeric(std::string col_id);//!Overwrite this if your backend does not support the information_schema.columns table
        bool get_isText(std::string col_id);//!Overwrite this if your backend does not support the information_schema.columns table
};
}
#endif

 
