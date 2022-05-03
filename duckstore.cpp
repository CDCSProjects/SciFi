#include "duckstore.h"

namespace SciStore {

DuckStore::DuckStore(){
    init();
}

DuckStore::DuckStore(std::string p_name){
    init(p_name); //Call the init function once before starting to execute any queries
}

void DuckStore::openAndConnect(){
        db = new DuckDB(nullptr);
	    conn = new Connection(*db);
}

void DuckStore::openAndConnect(std::string p_name){
        db = new DuckDB(p_name);
	    conn = new Connection(*db);
}

DuckStore::~DuckStore(){
    delete db;
}

std::string DuckStore::getResultAsString(){
    return current_result->ToString();
}

void DuckStore::printResult(){
    current_result->Print();
}


int DuckStore::createNewDB(){
    std::cout << "Not implemented and not needed atm\n";
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


std::vector<std::string> DuckStore::getIDsByConstraint(std::string constraint){

    std::string query = "SELECT " + idcolumn + " FROM metadata WHERE " + constraint;
    std::cout << "\033[36mMetastore full SQL query:\033[0m\n" << query << std::endl;
    execQuery(query);
    
    std::vector<std::string> idlist;
    
    //iterating all resuts did not work correctly when this was written, hence this ugly string magic
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
    std::cout << "loadDB not implemented\n";
        return 0;
}
}