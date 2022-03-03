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

void RocksStore::create(std::string p_directory, int recursive, int pathdepth, int useext){

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
           // pdb_files.push_back(get_stem(entry.path()));
            std::stringstream s;
            s << entry;
            pdb_file_names.push_back(s.str());
            pdb_file_names.back().erase(std::remove(pdb_file_names.back().begin(), pdb_file_names.back().end(), '"'), pdb_file_names.back().end());

            
            if (pathdepth==0){
                pdb_files.push_back(get_stem(entry.path()));
            }else{
                std::string test=s.str();
                std::string finals=s.str();
                size_t split = 0;
                for (size_t i=0; i<pathdepth+1;i++){
                  split = test.find_last_of("/");
                  if (split == std::string::npos) {
                    split=0;
                    break;
                    }
                  test=test.substr(0,split);
                  
                }
                
              
                if (useext == 0){
                size_t split2 = finals.find_last_of(".");
                pdb_files.push_back(finals.substr(split+1,split2-split-1));
                }else{
                pdb_files.push_back(finals.substr(split+1,finals.size()-split));
                }
                
                pdb_files.back().erase(std::remove(pdb_files.back().begin(), pdb_files.back().end(), '"'), pdb_files.back().end());
                pdb_files.back().erase(std::remove(pdb_files.back().begin(), pdb_files.back().end(), '\n'), pdb_files.back().end());
            }
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
       if (recursive == 0) path = p_directory + "/" + pdb_file_names[i];
       else path = pdb_file_names[i];
      std::ifstream ifs(path);
      content.assign( (std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>() ) );
      database->Put( WriteOptions(), pdb_files[i], content);
      std::cout << "Create asset with ID " << pdb_files[i] << " from file " << path << std::endl;
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
    
    std::string id = pdbid;
    size_t split = id.find_last_of("/");
    if (split != std::string::npos){
        id=id.substr(split+1,id.size());
    }
    std::ofstream o; //ofstream is the class for fstream package
    o.open(id + p_fileextension); //open is the method of ofstream
    o << content; // << operator which is used to print the file informations in the screen
    o.close();
    
    std::cout << "\033[32mAsset " + pdbid + " written to file " + pdbid + p_fileextension << "\033[0m" << std::endl;

    return;
}

void RocksStore::createportable(std::string directory, int recursive, int pathdepth, int useext){
    std::cout << "Starting to create key value store from pdb folder. This may take a while...\n";

     Options options;
          
     std::vector<filepaths> fpv;
     //std::vector<std::string> pdb_files;
     //std::vector<std::string> pdb_file_names;
     filepaths fp;
     

    if (recursive == 0){
        for (const auto & entry : fs::directory_iterator(directory)){
          //pdb_files.push_back(get_stem(entry.path()));
          //pdb_file_names.push_back(entry.path().filename().string());
          fp.files=get_stem(entry.path());
          fp.file_names=entry.path().filename().string();
        }
    }else{
        for (const auto & entry : fs::recursive_directory_iterator(directory)){
          if (!entry.is_directory()){
            std::stringstream s;
            s << entry;
            
            //pdb_file_names.push_back(s.str());
            //pdb_file_names.back().erase(std::remove(pdb_file_names.back().begin(), pdb_file_names.back().end(), '"'), pdb_file_names.back().end());
            
            fp.file_names=s.str();
            std::cout << fp.file_names << std::endl;
            fp.file_names.erase(std::remove(fp.file_names.begin(), fp.file_names.end(), '"'), fp.file_names.end());
            std::cout << fp.file_names << std::endl;
            
            if (pathdepth==0){
                //pdb_files.push_back(get_stem(entry.path()));
                fp.files=get_stem(entry.path());
            }else{
                std::string test=s.str();
                std::string finals=s.str();
                size_t split = 0;
                for (size_t i=0; i<pathdepth+1;i++){
                  split = test.find_last_of("/");
                  if (split == std::string::npos) {
                    split=0;
                    break;
                    }
                  test=test.substr(0,split);
                  
                }
                
                if (useext == 0){
                size_t split2 = finals.find_last_of(".");
                //pdb_files.push_back(finals.substr(split+1,split2-split-1));
                fp.files=finals.substr(split+1,split2-split-1);
                }else{
                //pdb_files.push_back(finals.substr(split+1,finals.size()-split));
                fp.files=finals.substr(split+1,finals.size()-split);
                }
                
                //pdb_files.back().erase(std::remove(pdb_files.back().begin(), pdb_files.back().end(), '"'), pdb_files.back().end());
                //pdb_files.back().erase(std::remove(pdb_files.back().begin(), pdb_files.back().end(), '\n'), pdb_files.back().end());
                fp.files.erase(std::remove(fp.files.begin(), fp.files.end(), '"'), fp.files.end());
                fp.files.erase(std::remove(fp.files.begin(), fp.files.end(), '\n'), fp.files.end());
            }
            
            fpv.push_back(fp);
            
          }
        }
        
        std::sort(fpv.begin(),fpv.end(),comparep);
    }
    
    
    //set options
    options.create_if_missing = true;
    options.keep_log_file_num = 1;
    

    
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

    //for (uint32_t i=0; i<pdb_files.size(); i++){
    for (uint32_t i=0; i<fpv.size(); i++){
       if (recursive == 0) path = directory + "/" + fpv[i].file_names; //path = directory + "/" + pdb_file_names[i];
       else path= fpv[i].file_names;//path = pdb_file_names[i];
      std::ifstream ifs(path);
      content.assign( (std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>() ) );
        //s_write = sst_file_writer.Put(pdb_files[i], content);
        s_write = sst_file_writer.Put(fpv[i].files, content);
        //std::cout << "Create asset with ID " << pdb_files[i] << " from file " << path << std::endl;
        std::cout << "Create asset with ID " << fpv[i].files << " from file " << path << std::endl;
          if (s_write.ok() == 0){
            std::cerr << "\033[31mSomething went wrong when adding assets. " << s_write.getState() << "Reopen file writer and try again. \033[0m" << std::endl;
            return;
          //sst_file_writer.Finish(); //I'm pretty sure I can avoid these failures by setting the right options, but I don't know which options.
          //sst_file_writer.Open(portablefile);
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
    
  
 /*   IngestExternalFileOptions efo;
    efo.write_global_seqno=false;*/
    Status s_import = database->IngestExternalFile({sstfile}, IngestExternalFileOptions());
    if (s_import.ok() == 0){
        std::cerr << "\033[31mSomething went wrong. Does the file " << sstfile << " exist?\033[0m" << std::endl;
        return;
    }

    std::cout << "...Finished importing sst file.\n";
    
    return;
}
}


