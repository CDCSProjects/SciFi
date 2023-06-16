#include <framework/include/metadata.h>

namespace SciStore {

MetaStorage::MetaStorage(){

}
     
MetaStorage::MetaStorage(std::string p_name){

}

void MetaStorage::init(){
        openAndConnect();
        execQuery("CREATE TABLE IF NOT EXISTS metainfo (tablename VARCHAR, idcolname VARCHAR)");
        execQuery("CREATE TABLE IF NOT EXISTS filter (filtername VARCHAR)");
        execQuery("SELECT idcolname FROM metainfo WHERE tablename = 'metadata'");
        std::string IDCol = getResultAsString();

        idcolumn = IDCol.erase (0, IDCol.find_last_of(']') +1 );


        execQuery("CREATE TABLE IF NOT EXISTS filedata (key VARCHAR, collection VARCHAR DEFAULT(NULL), compressed BOOLEAN DEFAULT FALSE, path VARCHAR  DEFAULT '', fileextension VARCHAR DEFAULT '')");
}

void MetaStorage::init(std::string p_name){
        openAndConnect(p_name);
   
        execQuery("CREATE TABLE IF NOT EXISTS metainfo (tablename VARCHAR, idcolname VARCHAR)");
        execQuery("CREATE TABLE IF NOT EXISTS filter (filtername VARCHAR)");
        execQuery("SELECT idcolname FROM metainfo WHERE tablename = 'metadata'");
        std::string IDCol = getResultAsString();

        idcolumn = crop_single_result(IDCol);

        execQuery("CREATE TABLE IF NOT EXISTS filedata (key VARCHAR, collection VARCHAR DEFAULT(NULL), compressed BOOLEAN DEFAULT FALSE, path VARCHAR  DEFAULT '', fileextension VARCHAR DEFAULT '')");
}


void MetaStorage::writeResultToFile(std::string filename, std::string fileextension){
    std::ofstream o;
    o.open(resultfolder + "/" + filename + fileextension); 
    o << getResultAsString();
    o.close();
    #ifdef OUTPUTSHELL
    std::cout << "\033[32mMetadata written to " << filename + fileextension << "\033[0m" << std::endl;
    #endif
}

void MetaStorage::getSingle(std::string pdbid){
    std::string query = "SELECT * FROM metadata where " + idcolumn + " = '" + pdbid + "'";
    execQuery(query);
    #ifdef OUTPUTSHELL
    std::cerr << getResultAsString();
    #endif
    return;
}

void MetaStorage::getSingleToFile(std::string pdbid, std::string p_fileextension){
    std::string query = "SELECT * FROM metadata where " + idcolumn + " = '" + pdbid + "'";
    execQuery(query);
    
    std::string id = pdbid;
    size_t split = id.find_last_of("/");
    if (split != std::string::npos){
        id=id.substr(split+1,id.size());
    }
    
    std::ofstream o;
    o.open(resultfolder + "/" + id + "meta" + p_fileextension); 
    o << getResultAsString();
    o.close();
    #ifdef OUTPUTSHELL
    std::cout << "\033[32mMetadata written to file " << id << "meta" << p_fileextension << "\033[0m" << std::endl;
    #endif
    return;
}

bool MetaStorage::get_isNumeric(std::string col_id){
    std::string query = "SELECT count(numeric_scale) from information_schema.columns where column_name='" + col_id +"'";
    execQuery(query);
    if (crop_single_result(getResultAsString()) == "0") return false;
    else return true;
}

bool MetaStorage::get_isText(std::string col_id){
    std::string query = "SELECT data_type from information_schema.columns where column_name='" + col_id +"'";
    execQuery(query);
    if (crop_single_result(getResultAsString()).rfind("VARCHAR", 0) == 0) return true;
    if (crop_single_result(getResultAsString()) == "TEXT") return true;
    return false;
}

//TODO get_isBoolean

}   
