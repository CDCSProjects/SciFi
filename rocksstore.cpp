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

void RocksStore::unzipAndCreate(std::string p_directory, int recursive, int pathdepth, int useext, int unzip){
    unsigned char unzipBuffer[8192];
    unsigned int unzippedBytes;
    std::vector<unsigned char> unzippedData;
    
    if (unzip==0){
        create( p_directory, recursive, pathdepth, useext);
        return;
    }
    
    
    if (recursive==0){
       fs::create_directories(p_directory+"/unzipped");
       for (const auto & entry : fs::directory_iterator(p_directory)){
        if (!entry.is_directory()){
            gzFile inFileZ = gzopen(entry.path().string().c_str(), "rb");
          
            std::cout << "file " << entry.path().string().c_str() << std::endl;
          
            std::ofstream ofs;

            ofs.open( p_directory+"/unzipped/"+get_stem(entry.path()), std::ofstream::out | std::ofstream::app );
            unzippedData.clear();
            
            while (true) {
                      //gzseek(inFileZ, offset, SEEK_SET);
                      unzippedBytes = gzread(inFileZ, unzipBuffer, 8192);
                      
                      //std::cout << "offset: " << offset << std::endl;
                    
                      if (unzippedBytes > 0) {
                            unzippedData.insert(unzippedData.end(), unzipBuffer, unzipBuffer + unzippedBytes);
                      } else {
                        //int z_errnum = 0;
			            // std::cout << "gzerror: " << gzerror(inFileZ, &z_errnum) << std::endl;
                          break;
                      }

              }
                
                
                ofs.write( std::string((const char*) unzippedData.data(),unzippedData.size()).c_str(), unzippedData.size() );
                ofs.close();
                gzclose(inFileZ);
        }

    }
        create(p_directory+"/unzipped", recursive, pathdepth, useext);
        fs::remove_all(p_directory+"unzipped");
    }
}

std::vector<filedata> RocksStore::create(std::string p_directory, int recursive, int pathdepth, int useext, int removeprefixchar){

    std::cout << "Starting to create key value store from pdb folder. This may take a while...\n";

     Options options;
          
     std::vector<std::string> pdb_files;
     std::vector<std::string> pdb_file_names;
     std::vector<filedata> fileinfo;

    if (recursive==0){
       for (const auto & entry : fs::directory_iterator(p_directory)){
          std::string ext=entry.path().string().substr(entry.path().string().find_last_of("."));
          filedata fi;
          if (useext){
            pdb_files.push_back(entry.path().string().substr(removeprefixchar));
            }
          else{
            pdb_files.push_back(get_stem(entry.path()).substr(removeprefixchar));
            fi.fileextension = ext;
            }
          pdb_file_names.push_back(entry.path().filename().string());
          fi.key = pdb_files.back();
          if (ext == ".gz" ) fi.compressed=true;
          fileinfo.push_back(fi);
      }
    }else{
        for (const auto & entry : fs::recursive_directory_iterator(p_directory)){
          if (!entry.is_directory()){
            
            std::string ext=entry.path().string().substr(entry.path().string().find_last_of("."));


            filedata fi;
            
            std::stringstream s;
            s << entry;
            pdb_file_names.push_back(s.str());
            pdb_file_names.back().erase(std::remove(pdb_file_names.back().begin(), pdb_file_names.back().end(), '"'), pdb_file_names.back().end());

            
          /*  if (pathdepth==0){
                pdb_files.push_back(get_stem(entry.path()));
            }else{*/
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
                
                fi.path=test.substr(1,s.str().find_last_of("/"));
                
                if (useext == 0){

                  size_t split2 = finals.find_last_of(".");
                  size_t split2_begin = finals.find_first_of(".");
                  size_t diff = split2-split2_begin;
                  if (ext.compare(".gz") != 0 ){
                      pdb_files.push_back(finals.substr(split+1,split2-split-1).substr(removeprefixchar));
                      fi.fileextension = ext;

                  }
                  else{
                      pdb_files.push_back(finals.substr(split+1,split2_begin-split-1).substr(removeprefixchar));  
                      fi.fileextension = finals.substr(split2_begin,split2-split2_begin);
                      fi.compressed=true;

                  }
                }else{
                    if (ext.compare(".gz") == 0 ){
                        size_t split2 = finals.find_last_of(".");
                        pdb_files.push_back(finals.substr(split+1,split2-split-1).substr(removeprefixchar));  
                        fi.compressed=true;
                    }else{
                        pdb_files.push_back(finals.substr(split+1,finals.size()-split).substr(removeprefixchar));                    
                    }
                }
                

                pdb_files.back().erase(std::remove(pdb_files.back().begin(), pdb_files.back().end(), '"'), pdb_files.back().end());
                pdb_files.back().erase(std::remove(pdb_files.back().begin(), pdb_files.back().end(), '\n'), pdb_files.back().end());
                

                fi.key=pdb_files.back();
                fileinfo.push_back(fi);
                
           // }
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
    return fileinfo;
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

            fp.file_names.erase(std::remove(fp.file_names.begin(), fp.file_names.end(), '"'), fp.file_names.end());
  
            
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
            std::cerr << "\033[31mSomething went wrong when adding assets. " << s_write.getState() << " Continue with next asset.\033[0m" << std::endl;
          //  return;
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


