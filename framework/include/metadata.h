#ifndef METADATA_H
#define METADATA_H

#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>


namespace SciStore {
class MetaStorage{
    public:
        MetaStorage(std::string p_name);
        MetaStorage();
        ~MetaStorage() {};
        
        std::string getSingle(std::string pdbid);
        void getSingleToFile(std::string pdbid, std::string fileextension);
        void writeResultToFile(std::string filename, std::string fileextension);
        void init();
        std::vector<std::string> get_all_meta_tables();
        void init(std::string p_name);
        std::vector<std::string> getIDsByConstraint(std::string constraint, bool ids_only=true, bool join_meta_tables=true);
        void writeResultMetaTablesToFile(std::string filename, std::string fileextension);
        void printResultMetaTables(); 
        std::string get_metaQuery_fromMetaData(std::string constraint);
        
                
        virtual int createNewDB() = 0;
        int loadDB(std::string p_name) {return 0;}
        virtual void execQueryAndPrint(std::string p_query) = 0;
        virtual void printResult() = 0;
        virtual std::vector<std::string> format_getIDsByConstraint() = 0; //Formats the current query result after running the SQL queries in getIDsByConstraint
        virtual void execQuery(std::string p_query) = 0;
        virtual std::string getResultAsString() = 0;
        virtual void openAndConnect() = 0;
        virtual void openAndConnect(std::string name) = 0;
        virtual std::vector<std::string> getIDsByFileData() = 0;
        virtual std::string crop_single_result(std::string res) = 0; //!Even when returning only a single record as a result, systems tend to add additional characters, e.g. spaces or line breaks. This function is supposed to get rid of these additional characters. 
        virtual std::string crop_result(std::string res) = 0;//!Remove any additional information from he result string that is not the plain tuples
        virtual std::vector<std::string> get_user_meta_column_names(std::string table="metadata") = 0;//!Return a vector containing the names of the columns including the user meta data
        virtual std::vector<std::string> crop_and_split_result(std::string res) = 0;//!Return a vector where all tuples are split into individual strings, such that each element is a single attribute of a single tuple
        /** There is no standard way in SQL to read a csv file. Each system implements this a bit differently (or not at all). 
        * The implementation of this function for each backend should return the query that creates a table from a csv file specified by filename.
        * @param tablename The name of the table that should be created
        * @param filename The path to the csv file containing the data
        * @param data_only If set to true, only the records are taken from the file, not the column definitions. Note that the table has to be created beforehand for this. The storage class does this before calling this function with data_only set to true.
        * @param skiplines Number of lines that should be skipped at the beginning of the file. Only used by storage when data_only is set to true. This can be useful if only a subset of the records should be used or if teh file contains some header information that cannot automatically be detected.
        */
        virtual std::string get_read_csv_query(std::string tablename, std::string filename, bool data_only=false, int skiplines=0) = 0; 

        bool get_isNumeric(std::string col_id);//!Overwrite this if your backend does not support the information_schema.columns table. This function only works if there is only one user-defined meta data table. If there are more please use get_isNumeric_byAttr. 
        bool get_isText(std::string col_id);//!Overwrite this if your backend does not support the information_schema.columns table. This function only works if there is only one user-defined meta data table. If there are more please use get_isText_byAttr. 
        bool get_isNumeric_byAttr(std::string attr, std::string table_name="metadata");//!Provide attr in the form tablename.attribute. Overwrite this if your backend does not support the information_schema.columns table
        bool get_isText_byAttr(std::string attr, std::string table_name="metadata");//!Provide attr in the form tablename.attribute. Overwrite this if your backend does not support the information_schema.columns table
        
        std::string idcolumn = "rowid";
        std::string resultfolder = "";
        bool success = true;
    private:
        std::string construct_meta_query(std::string pdbid);
        std::vector<std::vector<std::string>> resultTablesMeta; // perResult<perMetaTable<records>>
};
}
#endif

 
