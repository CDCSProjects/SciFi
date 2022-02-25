<<<<<<< HEAD
#ifndef DUCKSTORE_H
#define DUCKSTORE_H

#include <fstream>
#include "metadata.h"
#include "ducklib/duckdb.hpp"

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
        void getSingle(std::string pdbid);
        void getSingleToFile(std::string pdbid, std::string fileextension);
        std::vector<std::string> getIDsByConstraint(std::string constraint);
        std::unique_ptr<MaterializedQueryResult> current_result;
        std::string idcolumn;
    protected:
        DuckDB * db;
        Connection * conn;

};
}
#endif
=======
#ifndef DUCKSTORE_H
#define DUCKSTORE_H

#include <fstream>
#include "metadata.h"
#include "ducklib/duckdb.hpp"

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
        void getSingle(std::string pdbid);
        void getSingleToFile(std::string pdbid, std::string fileextension);
        std::vector<std::string> getIDsByConstraint(std::string constraint);
        std::unique_ptr<MaterializedQueryResult> current_result;
        std::string idcolumn;
    protected:
        DuckDB * db;
        Connection * conn;

};
}
#endif
>>>>>>> e74854c726ae7ab7a38c5b60b9108fda5d6b0205
