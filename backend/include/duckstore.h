#ifndef DUCKSTORE_H
#define DUCKSTORE_H

#include <fstream>
#include <framework/include/metadata.h>
#include "duckdb.hpp"

using namespace duckdb;

namespace SciStore {
class DuckStore : public MetaStorage{
    public:
        DuckStore(std::string p_name);
        DuckStore();
        ~DuckStore();
        int createNewDB();
        int loadDB(std::string p_name);
        void execQuery(std::string p_query);
        void execQueryAndPrint(std::string p_query);
        void printResult();
        void openAndConnect();
        void openAndConnect(std::string name);
        bool success;
        std::string getResultAsString();
        std::vector<std::string> format_getIDsByConstraint();
        std::vector<std::string> getIDsByFileData();
        std::string crop_single_result(std::string res);
        std::string crop_result(std::string res);
        std::string get_read_csv_query(std::string tablename, std::string filename, bool data_only=false, int skiplines=0);
        std::vector<std::string> get_user_meta_column_names();
        std::vector<std::string> crop_and_split_result(std::string res);
   //     void initDB(std::string name);

    protected:
        DuckDB * db;
        Connection * conn;
       // std::unique_ptr<MaterializedQueryResult> current_result;
        //duckdb::unique_ptr<QueryResult> current_result;
        duckdb::unique_ptr<MaterializedQueryResult> current_result;
};
}
#endif
