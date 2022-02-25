#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <zlib.h>


#include <assert.h>
#include "duckstore.h"
#include "rocksdb/include/rocksdb/db.h"
#include "downloader.h"

#include "rocksstore.h"
#include "storage.h"

//using namespace rocksdb;
//namespace fs = std::filesystem;


int main(int argc, char *argv[]){

    if (argc<2 || (strcmp(argv[1], "-h") == 0)) {
        std::cout << "No task provided. Exit.\n";
        std::cout << "Usage: pdbkv <task> [pdbID/sst-file]\n";
        std::cout << "Tasks:\n\tcreate - Create new key-value store from files in \"pdb\" folder\n\tget - Return the contents of a pdb file\n";
        std::cout << "\timport - Import data from an sst file\n\tcreateportable - Create a portable sst file from files in \"pdb\" folder\n";
        std::cout << "\treload: Reload the files from online pdb\n\n";
        std::cout << "pdbID: e.g. 4BT3, necessary for get task\n";
        std::cout << "sst-file: A data file to import into a fresh database, necessary for import task\n\n";

        std::cout << "Exit codes: 0 - success, 1 - no task provided, 2 - third argument is missing\n";
        return 1;
    }

    SciStore::Storage<> * st = new SciStore::Storage<RocksStore, DuckStore>("pdbtest","ducktest");
    
    //RocksStore * as=new RocksStore("pdbtest");
    auto * as = st->asset_store;
    //DuckStore * m = new DuckStore("pdbmeta.db");
    auto m = st->meta_store;
    
    if (strcmp(argv[1], "create") == 0) {
        as->create("pdb");
        return 0;
    }
    if (strcmp(argv[1], "get") == 0 ) {
        if (argc < 3) { std::cout << "No pdbID provided. Exit.\n"; return 2; }
        else as->getSingleToFile(argv[2], "pdb");
        return 0;
    }     
    if (strcmp(argv[1], "import") == 0 ) {
        if (argc < 3) { std::cout << "No file for import provided. Exit.\n"; return 2; }
        as->import(argv[2]);
        return 0;
    }
    if (strcmp(argv[1], "createportable") == 0 ) {
        as->createportable("pdb");
        return 0;
    }  
         


    
    if (strcmp(argv[1], "reread") == 0 ) {
        Downloader * d = new Downloader();
        d->download(d->source_compound, "compound.idx");
 
        m->execQuery("DROP TABLE IF EXISTS compound");
        m->execQuery("CREATE TABLE compound(IDCODE VARCHAR PRIMARY KEY, COMPOUND VARCHAR DEFAULT '')");
        m->execQueryAndPrint("COPY compound FROM 'compound.idx' (AUTO_DETECT TRUE, skip 4)");
        return 0;
    }
    m->execQuery("SELECT IDCODE FROM compound ORDER BY IDCODE");

    auto chunk=m->current_result->Fetch();

/* Get some information about our data
        std::cout << "chunk values: " << chunk->size() << " \n";
        std::cout << "column count chunk: " << chunk->ColumnCount()  << " \n"; 
        std::cout << "column count res: " << m->current_result->ColumnCount()  << " \n"; 
        std::cout << "res count res: " << m->current_result->collection.Count() << " \n"; 
        std::cout << "column count res: " << m->current_result->collection.ColumnCount() << " \n"; 
        std::cout << "chunk count res: " << m->current_result->collection.ChunkCount() << " \n"; 
*/        
    int i=0;
    m->current_result->Fetch();
        
    std::string ch =  m->current_result->collection.GetChunk(0).ToString();
        
    std::cout << m->current_result->collection.GetChunk(0).size() << std::endl;
       
    ch.erase(0, std::min(ch.find_last_of('[') + 1, ch.size() - 1));
    ch.erase (ch.find_last_not_of(']') - 1 , std::string::npos );
    std::cout << "IDs in this chunk: \n" << ch << std::endl;
        
    std::vector<std::string> idlist;
    std::string id;
    std::istringstream chstream(ch);
    while(std::getline(chstream, id, ',')) {
        id.erase(0, 1); //erase leading space
        idlist.push_back(id);
    }
        
    std::cout << "begin: " << idlist.front() << std::endl;
    std::cout << "end: " << idlist.back() << std::endl;
    std::cout << "size: " << idlist.size() << std::endl;
    
    as->open();
    
    unsigned char unzipBuffer[8192];
    unsigned int unzippedBytes;
    std::vector<unsigned char> unzippedData;
    Downloader * d = new Downloader();
        
    for (int i=0; i< idlist.size(); i++){
        
        std::cerr << i << ", ";
        
        d->download_by_pdb_id(idlist[i]);    
        
        gzFile inFileZ = gzopen((idlist[i]+".pdb.gz").c_str(), "rb");

        unzippedData.clear();
        while (true) {
            unzippedBytes = gzread(inFileZ, unzipBuffer, 8192);
            if (unzippedBytes > 0) {
                unzippedData.insert(unzippedData.end(), unzipBuffer, unzipBuffer + unzippedBytes);
            } else {
                break;
            }
        }
        gzclose(inFileZ); 
      

        as->insert(idlist[i], string((const char*) unzippedData.data(),unzippedData.size()));
        
        remove ((idlist[i]+".pdb.gz").c_str());
        
    }
    as->close();
   // get(idlist.front());  
    
    //std::cout << unzippedData;
    for (int i=0; i< m->current_result->collection.ChunkCount(); i++){
            /*TODO:
                1. copy chunk iteration here
                2. insert in batches instead of single inserts
                */
                  
    }
        
 
    return 0;
}



/* Left here for later reference: delete, batch operations
   
    pdb->Delete( WriteOptions(), key1);

    std::string value3 = "another value";
    WriteBatch batch;
    batch.Delete("key2");
    batch.Put("k2", value3);
    batch.Put("k3", value3);
    Status s = pdb->Write(WriteOptions(), &batch);
    
*/