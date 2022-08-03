#include <framework/include/assetstore.h>

namespace SciStore {

void AssetStore::init(std::string p_name){
    db_name=p_name;
    dl=new Downloader();
}

void AssetStore::insertFromFile(std::string file,  std::string path){
    std::string id= file.erase (file.find_last_not_of('.') - 1 , std::string::npos );

    path = path + "/" + file;
    std::ifstream ifs(path);
        
    std::string content;
    content.assign( (std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>() ) );
    insert(id, content);
}

void AssetStore::unzipAndCreate(std::string p_directory, int recursive, int pathdepth, int useext, int unzip){
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
          
            #ifdef OUTPUTSHELL
            std::cout << "file " << entry.path().string().c_str() << std::endl;
            #endif
            
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

std::vector<filedata> AssetStore::create(std::string p_directory, int recursive, int pathdepth, int useext, int removeprefixchar){
    #ifdef OUTPUTSHELL
   std::cout << "Starting to create key value store from pdb folder. This may take a while...\n";
    #endif
    setOptions();
    // Options options;
          
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
                
          
          }
        }
    }
    
    //set options
 /*   options.create_if_missing = true;
    options.keep_log_file_num = 1;*/
    //options.manual_wal_flush = true; //TODO find out if this makes a big difference once we have more data
    //options.error_if_exists = true; //TODO enable for release version 
    
    std::string content;
    std::string path;
    for (uint32_t i=0; i<pdb_files.size(); i++){
    //path = "pdb/" + pdb_files[i] + ".pdb";
       if (recursive == 0) path = p_directory + "/" + pdb_file_names[i];
       else path = pdb_file_names[i];
      std::ifstream ifs(path);
      content.assign( (std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>() ) );
      insert(pdb_files[i], content);
      //database->Put( WriteOptions(), pdb_files[i], content);
      #ifdef OUTPUTSHELL
      std::cout << "Create asset with ID " << pdb_files[i] << " from file " << path << std::endl;
      #endif
    }
    
    //pdb->FlushWAL(true); //TODO enable if we set manual_wal_flush
    #ifdef OUTPUTSHELL
    std::cout << "...Finished creating key value store.\n";
    #endif
    
    return fileinfo;
}

void AssetStore::getSingleToFile(std::string pdbid, std::string p_fileextension){


  //  Options options;
    setOptions();
    /*options.create_if_missing = false; 
    options.keep_log_file_num = 1;*/

    
    std::string content;
    
    content = getSingle(pdbid);
    
    if (content.compare("") == 0) return;
    
    std::string id = pdbid;
    size_t split = id.find_last_of("/");
    if (split != std::string::npos){
        id=id.substr(split+1,id.size());
    }
    std::ofstream o; //ofstream is the class for fstream package
    o.open(resultfolder + "/" + id + p_fileextension); //open is the method of ofstream
    o << content; // << operator which is used to print the file informations in the screen
    o.close();
    
    #ifdef OUTPUTSHELL
    std::cout << "\033[32mAsset " + pdbid + " written to file " + pdbid + p_fileextension << "\033[0m" << std::endl;
    #endif

    return;
}

std::vector<filedata> AssetStore::createportable(std::string directory, int recursive, int pathdepth, int useext, int removeprefixchar){
    #ifdef OUTPUTSHELL
    std::cout << "Starting to create key value store from pdb folder. This may take a while...\n";
    #endif

     setOptions();
     
          
     std::vector<filepaths> fpv;
     //std::vector<std::string> pdb_files;
     //std::vector<std::string> pdb_file_names;
     filepaths fp;
     
     std::vector<filedata> fileinfo;

    if (recursive==0){
       for (const auto & entry : fs::directory_iterator(directory)){
          std::string ext=entry.path().string().substr(entry.path().string().find_last_of("."));
          filedata fi;
          if (useext){
            fp.files=(entry.path().string().substr(removeprefixchar));
            }
          else{
            fp.files=(get_stem(entry.path()).substr(removeprefixchar));
            fi.fileextension = ext;
            }
          fp.file_names=(entry.path().filename().string());
          fi.key = fp.files;
          if (ext == ".gz" ) fi.compressed=true;
          fileinfo.push_back(fi);
          fpv.push_back(fp);
      }
      
   /* if (recursive == 0){
        for (const auto & entry : fs::directory_iterator(directory)){
          //pdb_files.push_back(get_stem(entry.path()));
          //pdb_file_names.push_back(entry.path().filename().string());
          fp.files=get_stem(entry.path());
          fp.file_names=entry.path().filename().string();
        }*/
    }else{
        for (const auto & entry : fs::recursive_directory_iterator(directory)){
          if (!entry.is_directory()){
          
            std::string ext=entry.path().string().substr(entry.path().string().find_last_of("."));


            filedata fi;
            
            std::stringstream s;
            s << entry;
            
            //pdb_file_names.push_back(s.str());
            //pdb_file_names.back().erase(std::remove(pdb_file_names.back().begin(), pdb_file_names.back().end(), '"'), pdb_file_names.back().end());
            
            fp.file_names=s.str();

            fp.file_names.erase(std::remove(fp.file_names.begin(), fp.file_names.end(), '"'), fp.file_names.end());
  
            
           /* if (pathdepth==0){
                //pdb_files.push_back(get_stem(entry.path()));
                fp.files=get_stem(entry.path());
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
                    //pdb_files.push_back(finals.substr(split+1,split2-split-1));
                   // fp.files=finals.substr(split+1,split2-split-1);
                    size_t split2_begin = finals.find_first_of(".");
                  size_t diff = split2-split2_begin;
                  if (ext.compare(".gz") != 0 ){
                      fp.files=(finals.substr(split+1,split2-split-1).substr(removeprefixchar));
                      fi.fileextension = ext;

                  }
                  else{
                      fp.files=(finals.substr(split+1,split2_begin-split-1).substr(removeprefixchar));  
                      fi.fileextension = finals.substr(split2_begin,split2-split2_begin);
                      fi.compressed=true;

                  }
                }else{
                    //pdb_files.push_back(finals.substr(split+1,finals.size()-split));
                    //fp.files=finals.substr(split+1,finals.size()-split);
                    
                      if (ext.compare(".gz") == 0 ){
                        size_t split2 = finals.find_last_of(".");
                        fp.files=(finals.substr(split+1,split2-split-1).substr(removeprefixchar));  
                        fi.compressed=true;
                    }else{
                        fp.files=(finals.substr(split+1,finals.size()-split).substr(removeprefixchar));                    
                    }
                }
                
                //pdb_files.back().erase(std::remove(pdb_files.back().begin(), pdb_files.back().end(), '"'), pdb_files.back().end());
                //pdb_files.back().erase(std::remove(pdb_files.back().begin(), pdb_files.back().end(), '\n'), pdb_files.back().end());
                fp.files.erase(std::remove(fp.files.begin(), fp.files.end(), '"'), fp.files.end());
                fp.files.erase(std::remove(fp.files.begin(), fp.files.end(), '\n'), fp.files.end());
                
                fi.key=fp.files;
                fileinfo.push_back(fi);
           // }
            
            fpv.push_back(fp);
            
          }
        }
         auto start = std::chrono::steady_clock::now();
   

        std::sort(fpv.begin(),fpv.end(),comparep);
        
        auto end = std::chrono::steady_clock::now();
       
        auto time_min=std::chrono::duration_cast<std::chrono::minutes>(end-start).count();
        #ifdef OUTPUTSHELL
        std::cout << "Sorting done. Elapsed time: " << time_min << " minutes\n";
        #endif
    }
    
    
    if( writePortable(fpv, fp, directory, recursive) > 0) {return fileinfo;}
    #ifdef OUTPUTSHELL
    std::cout << "...Finished creating key value store.\n";
    #endif

    return fileinfo;
}

}