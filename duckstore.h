#ifndef DUCKSTORE_H
#define DUCKSTORE_H

#include <fstream>
#include "metadata.h"
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
        std::string getResultAsString();
        std::vector<std::string> getIDsByConstraint(std::string constraint);

    protected:
        DuckDB * db;
        Connection * conn;
        std::unique_ptr<MaterializedQueryResult> current_result;

};
}
#endif
