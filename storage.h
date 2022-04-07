#ifndef STORAGEDATA_H
#define STORAGEDATA_H

#include <filesystem>
#include <string>
#include "rocksstore.h"
#include "duckstore.h"

namespace SciStore {
template<typename A = RocksStore, typename M = DuckStore>
class Storage{
    public:
      Storage(std::string assetDBname, std::string metaDBname){
          asset_store = new A(assetDBname);
          asset_store->open();
          meta_store = new M(metaDBname);
      };
      
      ~Storage(){
        delete meta_store;
      }
      
      A* asset_store;
      M* meta_store;
      std::string afe = ".dat";
      std::string mfe = ".txt";
      
      void load_asset_from_file(std::string file, std::string directory = ""){
        asset_store->insertFromFile(file, directory);
        return;
      }

      void load_assets_from_directory(std::string dir, int portable =0, int recursive=0, int depth=0, int ext=0, int prefix=0){
        meta_store->execQuery("DROP TABLE IF EXISTS filedata"); //TODO Delete this line for release
        meta_store->execQuery("CREATE TABLE IF NOT EXISTS filedata (key VARCHAR, collection VARCHAR DEFAULT(NULL), compressed BOOLEAN DEFAULT FALSE, path VARCHAR  DEFAULT '', fileextension VARCHAR DEFAULT '')");
        
        if (portable==0){
              std::vector<filedata> fd = asset_store->create(dir, recursive, depth, ext, prefix);
              std::cout << "Added " << fd.size() << " assets" << std::endl;
              for (int i=0; i< fd.size(); i++){
                meta_store->execQuery("INSERT INTO filedata values ('" + fd[i].key + "', " + fd[i].collection + " , " + (fd[i].compressed == true ? string("true") : string("false")) + ", '" + fd[i].path + "', '" + fd[i].fileextension + "')");
                
              }
             // meta_store->execQueryAndPrint("SELECT * FROM filedata;");
        }
          else asset_store->createportable(dir, recursive, depth, ext);
      }
      
      void load_asset_from_remote(std::string address){
          std::string id= address.erase (address.find_last_not_of('.') - 1 , address.size()-1 );
          id = id.erase(0,std::min(id.find_last_of('[') + 1, id.size() - 1));
          asset_store->dl->download(address, id);
          asset_store->insertFromFile(id);
          remove (id.c_str());   
      }
      
      /**
       * Reads metadata from a csv file. Note that this function makes a few assumptions:
       * 1) The unique identifier is stored in the first column.
       * 2) The first line of the file provides the column headers/property names.
       * 3) There is no content before the column headers and no content after the metadata is finished.
       * If this function fails to retrieve metadata from a file correctly, use @ref load_metadata_advanced. 
       * @param file The file containing the metadata
       * @param IDCol The name of the column with the unique identifier 
       */
      void load_metadata_from_file(std::string file, std::string IDCol){
          meta_store->execQuery("DROP TABLE IF EXISTS metadata");
          meta_store->execQuery("CREATE TABLE IF NOT EXISTS metadata AS SELECT * FROM read_csv_auto(" + file + ");");
          meta_store->execQuery("DROP TABLE IF EXISTS metainfo;");
          meta_store->execQuery("CREATE TABLE metainfo (tablename VARCHAR, idcolname VARCHAR)");
          meta_store->execQuery("INSERT INTO metainfo VALUES ('metadata', '" + IDCol + "')");
          meta_store->idcolumn=IDCol;
      }
      
      void load_metadata_from_remote(std::string address, std::string IDCol){
          std::string id= address.erase (address.find_last_not_of('.') - 1 , address.size()-1 );
          id = id.erase(0,std::min(id.find_last_of('[') + 1, id.size() - 1));
          asset_store->dl->download(address, id);
          load_metadata_from_file(id);
          remove (id.c_str()); 
      };
      
      /**
       * If the meta-data file is not trivially readable by the metadata store, use this function to provide more information about your file.
       * @param file A file in CSV format
       * @param skiplines The number of lines to skip at the beginning of the file. This can be useful if the file contains any documentation or column headers.
       * @param columns A comma separated list defining pairs of column/propert names and their type. Provide the name first, then the type, e.g. myID int, myProperty varchar.
       * @param IDColumn Defines which column is the one with the unique identifier. This identifier is used to match the contents of the asset store. Note that this specificaion is zero-based. Contents of the ID column must be unique. Otherwise the metastore will throw a constraint error ('duplicated key').
       */
      void load_metadata_advanced(std::string file, int skiplines, std::string columns, int IDColumn=0){
        meta_store->execQuery("DROP TABLE IF EXISTS metadata");
        std::string createquery = "CREATE TABLE metadata("+ columns + ")";
        
        std::string before = "";
        std::string after = createquery;
        std::string idmt = "";
        for (int i=0; i<IDColumn+1;i++){
            int pos=after.find_first_of(",",1);
            if (pos==string::npos){
                pos = after.size()-1;
            }
            idmt=after;
            before.append(after.erase(pos,after.size()-1));
            after = idmt.erase(0,pos);
        }
        createquery = before + " PRIMARY KEY" + after;
        std::cout << createquery << std::endl;
        
        meta_store->execQuery(createquery);
        meta_store->printResult();
        meta_store->execQuery("COPY metadata FROM '" + file + "' (AUTO_DETECT TRUE, skip " + to_string(skiplines) + ")");
        meta_store->printResult();
        
        std::string IDCol = before;
        int pos_end = IDCol.find_last_of(" ");
        
        while (IDCol.at(pos_end) == ' '){
            IDCol=IDCol.erase(pos_end, IDCol.size()-1);
            pos_end--;
        }
        
        
        int pos_start1=IDCol.find_last_of(","); 
        int pos_start2=IDCol.find_last_of("(");
        int pos_start=0;
        if (pos_start1 > pos_start2) pos_start=pos_start1;
        else pos_start=pos_start2;
        IDCol.erase(0,pos_start+1);
        
        meta_store->idcolumn=IDCol;
        //std::cout << "IDcolumn name: " << IDCol << std::endl; 
        
        meta_store->execQuery("DROP TABLE IF EXISTS metainfo;");
        meta_store->execQuery("CREATE TABLE metainfo (tablename VARCHAR PRIMARY KEY, idcolname VARCHAR)");
        meta_store->execQuery("INSERT INTO metainfo VALUES ('metadata', '" + IDCol + "')");
        
        return;
      };
      
      void load_metadata_advanced_from_remote(std::string address, int skiplines, std::string columns, int IDColumn=0){
          std::string id= address.erase (address.find_last_not_of('.') - 1 , address.size() );
          id = id.erase(0,std::min(id.find_last_of('[') + 1, id.size() - 1));
          asset_store->dl->download(address, id);
          load_metadata_advanced(id, skiplines, columns, IDColumn);
          remove (id.c_str()); 
      };
      
      void get(std::string id, int assetToFile = 0, std::string fileextension="", int metaToFile = 0, std::string fileextension_meta=""){
        std::cout << "\033[36mReturning asset and metadata \033[0m\n";
        if (assetToFile != 0){
            asset_store->getSingleToFile(id, fileextension);
        }else{
            asset_store->getSingle(id);
        }
        if(metaToFile != 0){
            meta_store->getSingleToFile(id, fileextension_meta);
        }else{
            meta_store->getSingle(id);
        }
        std::cout << "________________" << std::endl << std::endl;
      };
      
      void get(std::vector<std::string> id, int assetToFile = 0, std::string fileextension="", int metaToFile = 0, std::string fileextension_meta=""){
        std::cout << "\033[36mReturning assets and metadata \033[0m\n";
        if (id.size() == 0) std::cout << "\033[31mNothing to do\033[0m\n";
        if(metaToFile != 0){

                meta_store->writeResultToFile("metadata", fileextension_meta);
          }else{

              meta_store->printResult();
          }
        for (int i=0; i<id.size();i++){
          if (assetToFile != 0){
              asset_store->getSingleToFile(id[i], fileextension);
          }else{
              asset_store->getSingle(id[i]);
          }

        }
        std::cout << "________________" << std::endl << std::endl;
      };
      
     void get_by_constraint(std::string constraint, int assetToFile = 0, std::string fileextension="", int metaToFile = 0, std::string fileextension_meta=""){
        std::vector<std::string> ids = meta_store->getIDsByConstraint(constraint);
        std::string query = "SELECT * FROM metadata where " + constraint;
        meta_store->execQuery(query);
        get(ids,assetToFile,fileextension,metaToFile,fileextension_meta);
        
      };
      
      void remove_asset(std::string id, int remove_metadata=0){
        asset_store->remove(id);
        if (remove_metadata!=0){
            meta_store->execQuery("DELETE FROM metadata WHERE " + meta_store->idcolumn + " = " + id);
        }
      };
      
      void import_asset_store(std::string file){
        asset_store->import(file);
        return;
      }
      
      void testout();
      /*TODO 
        - Wrapper for loading assets and/or metadata from file or internet
        - Wrapper for adding metadata manually
        */
};

//We need this for the specialization of the Storage class for the python wrapper. In case of a new specialization, a new wrapper is necessary.
class DefaultStorage : public Storage<SciStore::RocksStore, SciStore::DuckStore>{ 
    public:
        using Storage::Storage;
        //DefaultStorage(std::string assetDBname, std::string metaDBname) : Storage<SciStore::RocksStore, SciStore::DuckStore>(assetDBname, metaDBname){return;};
       // void iimport_asset_store(std::string file) {import_asset_store(file); return;}
        
};

typedef Storage<SciStore::RocksStore, SciStore::DuckStore> DefStorage;
}

#endif