#include "rocksstore.h"

namespace fs = std::filesystem;

namespace SciStore {
RocksStore::RocksStore(std::string p_name){
    db_name=p_name;
    dl=new Downloader();
}

void RocksStore::open(){
    Options options;
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

void RocksStore::insertFromFile(std::string file, std::string path){
   // open();
    std::string id= file.erase (file.find_last_not_of('.') - 1 , std::string::npos );

    path = path + "/" + file;
    std::ifstream ifs(path);
        
    std::string content;
    content.assign( (std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>() ) );
    database->Put( WriteOptions(), id, content);
}

void RocksStore::create(std::string p_directory, int recursive){

    std::cout << "Starting to create key value store from pdb folder. This may take a while...\n";

     Options options;
          
     std::vector<std::string> pdb_files;
     std::vector<std::string> pdb_file_names;

    if (recursive==0){
       for (const auto & entry : fs::directory_iterator(p_directory)){
          pdb_files.push_back(get_stem(entry.path()));
          pdb_file_names.push_back(entry.path().filename().string());
      }
    }else{
        for (const auto & entry : fs::recursive_directory_iterator(p_directory)){
          if (!entry.is_directory()){
            pdb_files.push_back(get_stem(entry.path()));
            pdb_file_names.push_back(entry.path().filename().string());
          }
        }
    }
    
    //set options
    options.create_if_missing = true;
    options.keep_log_file_num = 1;
    //options.manual_wal_flush = true; //TODO find out if this makes a big difference once we have more data
    //options.error_if_exists = true; //TODO enable for release version 
    

   /*insert data
   TODO
   - insert in batches to increase performance?
   - Use compactions? Which one?
   */
    std::string content;
    std::string path;
    for (uint32_t i=0; i<pdb_files.size(); i++){
    //path = "pdb/" + pdb_files[i] + ".pdb";
        path = p_directory + "/" + pdb_file_names[i];
      std::ifstream ifs(path);
      content.assign( (std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>() ) );
      database->Put( WriteOptions(), pdb_files[i], content);
    }
    
    //pdb->FlushWAL(true); //TODO enable if we set manual_wal_flush
    std::cout << "...Finished creating key value store.\n";

    return;
}

void RocksStore::remove(std::string key){
    Status s_get = database->Delete(WriteOptions(), key);
    assert(s_get.ok());
    std::cerr << "Deleted " << key << std::endl;
    
    return;
}

void RocksStore::getSingle(std::string pdbid){

    std::string content;
    Status s_get = database->Get( ReadOptions(), pdbid, &content);

    if (s_get.ok() == 0){
        std::cerr << "No asset found for ID " << pdbid << std::endl;
        return;
    }
    std::cerr << content << std::endl;
    
    return;
}

void RocksStore::getSingleToFile(std::string pdbid, std::string p_fileextension){


    Options options;
    options.create_if_missing = false; 
    options.keep_log_file_num = 1;

    
    std::string content;
    Status s_get = database->Get( ReadOptions(), pdbid, &content);

    if (s_get.ok() == 0){
        std::cerr << "\033[31mNo asset found for ID " << pdbid << "\033[0m" << std::endl;
        return;
    }
    std::ofstream o; //ofstream is the class for fstream package
    o.open(pdbid + "." + p_fileextension); //open is the method of ofstream
    o << content; // << operator which is used to print the file informations in the screen
    o.close();
    
    std::cout << "\033[32mAsset " + pdbid + " written to file " + pdbid + "." + p_fileextension << "\033[0m" << std::endl;

    return;
}

void RocksStore::createportable(std::string directory, int recursive){
    std::cout << "Starting to create key value store from pdb folder. This may take a while...\n";

     Options options;
          
     std::vector<std::string> pdb_files;
     std::vector<std::string> pdb_file_names;
     

    if (recursive == 0){
        for (const auto & entry : fs::directory_iterator(directory)){
          pdb_files.push_back(get_stem(entry.path()));
          pdb_file_names.push_back(entry.path().filename().string());
        }
    }else{
        for (const auto & entry : fs::recursive_directory_iterator(directory)){
          if (!entry.is_directory()){
            pdb_files.push_back(get_stem(entry.path()));
            pdb_file_names.push_back(entry.path().filename().string());
          }
        }
    }
    
    
    //set options
    options.create_if_missing = true;
    options.keep_log_file_num = 1;
    //options.error_if_exists = true; //Todo enable for release version 

    
    SstFileWriter sst_file_writer(EnvOptions(), options, options.comparator);
    Status s = sst_file_writer.Open(portablefile);
    if (s.ok() == 0){
        std::cerr << "\033[31mFileWriter could not open.\033[0m" << std::endl;
        return;
    }
   
   /*insert data
   TODO
   - insert in batches to increase performance
   - Cannot use compactions in portable sst files, or can we?
   */
    std::string content;
    std::string path;
    Status s_write;
    for (uint32_t i=0; i<pdb_files.size(); i++){
      path = directory + "/" + pdb_file_names[i];
    //path = directory + pdb_files[i] + ".pdb";
      std::ifstream ifs(path);
      content.assign( (std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>() ) );
      s_write = sst_file_writer.Put(pdb_files[i], content);
        if (s_write.ok() == 0){
        std::cerr << "\033[31mSomething went wrong when adding assets.\033[0m" << std::endl;
        return;
        }
    }
    
    Status s_finish = sst_file_writer.Finish();
        if (s_finish.ok() == 0){
        std::cerr << "\033[31mFileWriter could not finish.\033[0m" << std::endl;
        return;
    }

    std::cout << "...Finished creating key value store.\n";

    return;
}


void RocksStore::import(std::string sstfile){

    std::cout << "Starting to create key value store from sst file. This may take a while...\n";
    
    portablefile=sstfile;
    Options options;
     
    options.create_if_missing = true;
    options.keep_log_file_num = 1;
    

    Status s_import = database->IngestExternalFile({sstfile}, IngestExternalFileOptions());
    if (s_import.ok() == 0){
        std::cerr << "\033[31mSomething went wrong. Does the file " << sstfile << " exist?\033[0m" << std::endl;
        return;
    }

    std::cout << "...Finished importing sst file.\n";
    
    return;
}
}


