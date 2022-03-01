#include "duckstore.h"

namespace SciStore {

DuckStore::DuckStore(){
        db = new DuckDB(nullptr);
	    conn = new Connection(*db);
  //      current_result=make_unique<QueryResult>();
}

DuckStore::DuckStore(std::string p_name){
        db = new DuckDB(p_name);
	    conn = new Connection(*db);
        execQuery("CREATE TABLE IF NOT EXISTS metainfo (tablename VARCHAR, idcolname VARCHAR)");
        execQuery("SELECT idcolname FROM metainfo WHERE tablename = 'metadata'");/**/
        std::string IDCol = current_result->ToString();
        idcolumn = IDCol.erase (0, IDCol.find_last_of(']') +1 );
        current_result->Fetch();
     //   current_result=make_unique<QueryResult>();
}

DuckStore::~DuckStore(){
    delete db;
}

int DuckStore::createNewDB(){
        return 0;
}

void DuckStore::execQuery(std::string p_query){
        current_result = conn->Query(p_query);
       // std::cout << "column count current_result: " << current_result->ColumnCount()  << " \n";
        return ;
}
   
void DuckStore::execQueryAndPrint(std::string p_query){
        current_result = conn->Query(p_query);
        current_result->Print();
        return ;
}

void DuckStore::getSingle(std::string pdbid){
    std::string query = "SELECT * FROM metadata where " + idcolumn + " = '" + pdbid + "'";
    execQuery(query);
    std::cerr << current_result->ToString();
    return;
}

void DuckStore::getSingleToFile(std::string pdbid, std::string p_fileextension){
    std::string query = "SELECT * FROM metadata where " + idcolumn + " = '" + pdbid + "'";
    execQuery(query);
    
    std::string id = pdbid;
    size_t split = id.find_last_of("/");
    if (split != std::string::npos){
        id=id.substr(split+1,id.size());
    }
    
    std::ofstream o;
    o.open(id + "meta" + p_fileextension); 
    o << current_result->ToString();
    o.close();
    std::cout << "\033[32mMetadata written to file meta" + p_fileextension << "\033[0m" << std::endl;
    return;
}

std::vector<std::string> DuckStore::getIDsByConstraint(std::string constraint){

    std::string query = "SELECT " + idcolumn + " FROM metadata WHERE " + constraint;
    std::cout << "\033[36mMetastore full SQL query:\033[0m\n" << query << std::endl;
    execQuery(query);
    
    std::vector<std::string> idlist;
    
    for (int i=0; i<current_result->collection.ChunkCount();i++){
        std::string ch =  current_result->collection.GetChunk(i).ToString();
        
        ch.erase(0, std::min(ch.find_last_of('[') + 1, ch.size() - 1));
        ch.erase (ch.find_last_not_of(']') - 1 , std::string::npos );

        std::string id;
        std::istringstream chstream(ch);
        while(std::getline(chstream, id, ',')) {
          id.erase(0, 1); //erase leading space
          idlist.push_back(id);
        }
    }
    
    std::cout << "\033[36mFound IDs for constraint "  << constraint << ":\033[32m\n";
    for (int i=0; i<idlist.size(); i++) std::cout << idlist[i]  << std::endl;
    std::cout << "\033[0m"; 
    if (idlist.size() == 0) std::cout << "\033[31mnone\033[0m\n";
    std::cout << std::endl;
        
    return idlist;
}

int DuckStore::loadDB(std::string name){

        return 0;
}
}