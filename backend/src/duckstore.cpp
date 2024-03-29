#include <backend/include/duckstore.h>

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
        execQuery("PRAGMA enable_checkpoint_on_shutdown");
}

void DuckStore::openAndConnect(std::string p_name){
        db = new DuckDB(p_name);
	    conn = new Connection(*db);
}

DuckStore::~DuckStore(){
    execQuery("PRAGMA force_checkpoint;");
    execQuery("PRAGMA CHECKPOINT;");
    delete db;
}

std::string DuckStore::getResultAsString(){
    return current_result->ToString();
}

void DuckStore::printResult(){
    current_result->Print();
}


int DuckStore::createNewDB(){
    #ifdef OUTPUTSHELL
    std::cout << "Not implemented and not needed atm\n";
    #endif
    return 0;
}

void DuckStore::execQuery(std::string p_query){

        //current_result = conn->SendQuery(p_query);
        //current_result = ((StreamQueryResult &)*current_result).Materialize();
        current_result = conn->Query(p_query);
        if (current_result->HasError()) {
            success=false;
        }else {
            success = true;
        }
       // std::cout << "column count current_result: " << current_result->ColumnCount()  << " \n";
        return ;
}
   
void DuckStore::execQueryAndPrint(std::string p_query){
        #ifdef OUTPUTSHELL
        std::cout << p_query << std::endl;
        #endif
        //current_result = conn->SendQuery(p_query);
        //((StreamQueryResult &)*current_result).Materialize()->Print();
        current_result = conn->Query(p_query);
        if (current_result->HasError()) {
            success=false;
        }else {
            success = true;
        }
        return ;
}

std::string DuckStore::get_read_csv_query(std::string tablename, std::string filename, bool data_only, int skiplines){
    if (data_only==true){
        return "COPY "+ tablename + " FROM '" + filename + "' (AUTO_DETECT TRUE, skip " + to_string(skiplines) + ")";
    }else{
        return "CREATE TABLE IF NOT EXISTS " + tablename + " AS SELECT * FROM read_csv_auto('" + filename + "')";
    }
}

std::vector<std::string> DuckStore::format_getIDsByConstraint(){
    //constraint = (constraint == "all") ? "" : " WHERE " + constraint;

    
    std::vector<std::string> idlist;

    //iterating all results did not work correctly when this was written, hence this ugly string magic
  /*  for (int i=0; i<current_result->collection.ChunkCount();i++){
        std::string ch =  current_result->collection.GetChunk(i).ToString();
        
        ch.erase(0, std::min(ch.find_last_of('[') + 1, ch.size() - 1));
        ch.erase (ch.find_last_not_of(']') - 1 , std::string::npos );

        std::string id;
        std::istringstream chstream(ch);
        while(std::getline(chstream, id, ',')) {
          id.erase(0, 1); //erase leading space
          idlist.push_back(id);
        }
    }*/
    
    //Interface for materialized results has been changed. We still need ugly string magic, though
    
    std::string ch = current_result->ToString();
   // std::cout << "START" << ch << "END";
    ch.erase(0, std::min(ch.find_last_of(']') + 1, ch.size() - 1));
   // ch.erase (ch.find_last_not_of(']') - 1 , std::string::npos );

    std::string id;
    std::istringstream chstream(ch);
    while(std::getline(chstream, id, '\n')) {
     //   id.erase(0, 1); //erase leading space
        idlist.push_back(id);
    }
    idlist.erase(idlist.begin());//erase first element because it is empty
        idlist.erase(idlist.end());//erase last element because it is empty

    #ifdef OUTPUTSHELL
    std::cout << "\033[36mFound IDs:\033[32m\n";
    for (int i=0; i<idlist.size(); i++) std::cout << idlist[i] << std::endl;
    std::cout << "\033[0m"; 
    if (idlist.size() == 0) std::cout << "\033[31mnone\033[0m\n";
    std::cout << std::endl;
    #endif
       
    return idlist;
}

std::vector<std::string> DuckStore::getIDsByFileData(){
    std::string query = "SELECT key FROM filedata order by key ";
    execQuery(query);

    std::vector<std::string> idlist;
    

    //iterating all resuts did not work correctly when this was written, hence this ugly string magic
/*    for (int i=0; i<current_result->collection.ChunkCount();i++){
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
*/
   //Interface for materialized results has been changed. We still need ugly string magic, though

    std::string ch = current_result->ToString();
    ch.erase(0, std::min(ch.find_last_of(']') + 1, ch.size() - 1));
   // ch.erase (ch.find_last_not_of(']') - 1 , std::string::npos );

    std::string id;
    std::istringstream chstream(ch);
    while(std::getline(chstream, id, '\n')) {

        idlist.push_back(id);
    }
    
    idlist.erase(idlist.begin());//erase first element because it is empty
    idlist.erase(idlist.end());//erase last element because it is empty
    
    #ifdef OUTPUTSHELL
    for (int i=0; i<idlist.size(); i++) std::cout << idlist[i]  << std::endl;
    #endif
    //std::cout << std::endl;

    return idlist;
}

std::vector<std::string> DuckStore::crop_and_split_result(std::string res){
    //crop result
    //res.erase (0, res.find_last_of(']') +1 );
    for (int i=0; i<3; i++){
        res.erase (0, res.find_first_of('\n')+1);
    }
    //split result
    std::istringstream resstream(res);
    std::string r1;
    std::string r2;
    std::vector<std::string> resvec;

    while(std::getline(resstream, r1, '\n')) {
        std::istringstream r1stream(r1);
        bool missinglast=false;
        if (r1.back()=='\t') missinglast=true;
        while(std::getline(r1stream, r2, '\t')) {
            if (r2=="") resvec.push_back("");
            else resvec.push_back(r2);
        }
        if (missinglast==true) resvec.push_back("");
    }
    return resvec;
}

std::vector<std::string> DuckStore::get_user_meta_column_names(std::string table){
    std::string query = "select * from " + table +" limit 0";
    execQuery(query);
    std::istringstream resstream(getResultAsString());
    std::string r;

    int i=0;
    std::getline(resstream, r, '\n');

    std::istringstream rstream(r);
    std::vector<std::string> res;
    
    while(std::getline(rstream, r, '\t')) {
        if (r!=" ") res.push_back(r);
    }
    
    return res;
}

std::string DuckStore::crop_result(std::string res){
    res.erase (0, res.find_last_of(']') +1 );
    return res;
}

std::string DuckStore::crop_single_result(std::string res){
    res.erase (0, res.find_last_of(']') +1 );

    std::istringstream resstream(res);
    std::string r;

    int i=0;
    while(std::getline(resstream, r, '\n')) {

        if (i>0) return r;
        i++;
    }

    return r;
}

int DuckStore::loadDB(std::string name){
#ifdef OUTPUTSHELL
    std::cout << "loadDB not implemented\n";
    #endif
        return 0;

}

//!We only need this function for the python wrapper
/*void DuckStore::initDB(std::string name){
    execQuery("SELECT idcolname FROM metainfo WHERE tablename = 'metadata'");
    std::string IDCol = getResultAsString();
    idcolumn = IDCol.erase (0, IDCol.find_last_of(']') +1 );
    return;
}*/
}
