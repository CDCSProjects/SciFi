#include "rocksstore.h"
#include <chrono>

namespace fs = std::filesystem;

namespace SciStore {
RocksStore::RocksStore(std::string p_name){
    init(p_name);
    //db_name=p_name;
    //dl=new Downloader();
}

void RocksStore::open(){
  //  Options options;
    options.create_if_missing = true;
    options.keep_log_file_num = 1;
    //open database
    Status s_open = DB::Open(options, db_name, &database);
    assert(s_open.ok());
    return;
}

void RocksStore::insert(std::string key, std::string value){
    database->Put( WriteOptions(), key, value);
}

void RocksStore::setOptions(){
   options.create_if_missing = true;
   options.keep_log_file_num = 1;
}


void RocksStore::remove(std::string key){
    Status s_get = database->Delete(WriteOptions(), key);
    assert(s_get.ok());
    #ifdef OUTPUTSHELL
    std::cerr << "Deleted " << key << std::endl;
    #endif
    
    return;
}

std::string RocksStore::getSingle(std::string pdbid){

    std::string content;
    Status s_get = database->Get( ReadOptions(), pdbid, &content);

    if (s_get.ok() == 0){
        #ifdef OUTPUTSHELL
        std::cerr << "No asset found for ID " << pdbid << std::endl;
        #endif
        return "";
    }
    //std::cerr << content << std::endl;
    
    return content;
}


int RocksStore::writePortable(std::vector<filepaths> fpv,filepaths fp, std::string directory, int recursive){
    SstFileWriter sst_file_writer(EnvOptions(), options, options.comparator);
    Status s = sst_file_writer.Open(portablefile);
    if (s.ok() == 0){
        #ifdef OUTPUTSHELL
        std::cerr << "\033[31mFileWriter could not open.\033[0m" << std::endl;
        #endif
        return 3;
    }
   

    std::string content;
    std::string path;
    Status s_write;


    for (uint32_t i=0; i<fpv.size(); i++){
       if (recursive == 0) path = directory + "/" + fpv[i].file_names; 
       else path= fpv[i].file_names;
      std::ifstream ifs(path);
      content.assign( (std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>() ) );

        s_write = sst_file_writer.Put(fpv[i].files, content);

        #ifdef OUTPUTSHELL
        std::cout << "Create asset with ID " << fpv[i].files << " from file " << path << std::endl;
        #endif
          if (s_write.ok() == 0){
            #ifdef OUTPUTSHELL
            std::cerr << "\033[31mSomething went wrong when adding assets. " << s_write.getState() << " Continue with next asset.\033[0m" << std::endl;
            #endif

          }
    }
    
    Status s_finish = sst_file_writer.Finish();
        if (s_finish.ok() == 0){
        #ifdef OUTPUTSHELL
        std::cerr << "\033[31mFileWriter could not finish.\033[0m" << std::endl;
        #endif
        return 3;
    }
    
    return 0;

}

void RocksStore::import(std::string sstfile){
    #ifdef OUTPUTSHELL
    std::cout << "Starting to create key value store from sst file. This may take a while...\n";
    #endif
    
    portablefile=sstfile;
  //  Options options;
     
    options.create_if_missing = true;
    options.keep_log_file_num = 1;
    
  
 /*   IngestExternalFileOptions efo;
    efo.write_global_seqno=false;*/
    Status s_import = database->IngestExternalFile({sstfile}, IngestExternalFileOptions());
    if (s_import.ok() == 0){
        #ifdef OUTPUTSHELL
        std::cerr << "\033[31mSomething went wrong. Does the file " << sstfile << " exist?\033[0m" << std::endl;
        #endif
        return;
    }

    #ifdef OUTPUTSHELL
    std::cout << "...Finished importing sst file.\n";
    #endif
    
    return;
}
}


