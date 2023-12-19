#ifndef STORAGEDATA_H
#define STORAGEDATA_H

#include <filesystem>
#include <string>
#include <backend/include/rocksstore.h>
#include <backend/include/duckstore.h>

namespace SciStore {

/** Storage is the central class of the SciFi framework. It is highly recommended to only use this class to access the asset store.
 * @tparam A The backend for the data storage. Defaults to RocksStore which uses RocksDB.
 * @tparam M The backend for the meta storage. Defaults to DuckStore which uses DuckDB.
 */
template<typename A = RocksStore, typename M = DuckStore>
class Storage{
    public:
    
        std::string m_asset_path = ""; //!<stores the path to the asset store as given in the constructor
        std::string m_meta_path = ""; //!<stores the path to the meta store as given in the constructor
    
    /** Constructor of the storage class
     * @param assetDBname The name of the data storage. If the name doesn't exist, a new storage will be created.
     * @param metaDBname The name of the meta storage. If the name doesn't exist, a new storage will be created.
     * @param resultsfolder The name of the folder where any results are written to. If it doesn not exist, a folder with the given name will be created. Defaults to "results". 
     * @param temporarydevice Indicates whether results are written to disc or to a file system in main memory. This currently, this only works if your operating system provides a mounted /dev/shm. A symlink to your working directory will be created for easy access. We are working on a solution which uses FUSE to enable this feature for a wider range of systems without the need for root access.
    */
      Storage(std::string assetDBname, std::string metaDBname, std::string resultsfolder="results", int temporarydevice=0){
          asset_store = new A(assetDBname);
          asset_store->open();
          meta_store = new M(metaDBname);
          
          if (!fs::exists("/dev/shm") & temporarydevice!=0){
            #ifdef OUTPUTSHELL
            std::cout << "Temporary file system in user space is not supported by your operating system.\nCreating folder on disc.\n";
            #endif
            temporarydevice=0;
          }
          
          if (!fs::exists(resultsfolder)){
            if (temporarydevice==0){
                fs::create_directory(resultsfolder);
            }else{
                if (!fs::exists("/dev/shm/"+resultsfolder)) fs::create_directory("/dev/shm/"+resultsfolder);
                fs::create_directory_symlink("/dev/shm/"+resultsfolder,resultsfolder);
            }
          }
          
          meta_store->resultfolder=resultsfolder;
          asset_store->resultfolder=resultsfolder;
          
          m_asset_path=assetDBname;
          m_meta_path=metaDBname;
      };
      
      /** Destructor of the storage class
       */
      ~Storage(){
        delete meta_store;
      }
      
      A* asset_store; //!< A pointer to the data store
      M* meta_store; //!< A pointer to the meta store
      std::string afe = ".dat"; //!< File ending for returned assets @todo Use original file endings from filedata table in metastore
      std::string mfe = ".txt"; //!< 
      
      void open_meta_store(std::string metaDBname, std::string resultsfolder="results"){
        meta_store = new M(metaDBname);
        meta_store->resultfolder=resultsfolder;
      }
      /**
       */
      void load_asset_from_file(std::string file, std::string directory = ""){
        asset_store->insertFromFile(file, directory);
        return;
      }

      void load_assets_from_directory(std::string dir, int portable =0, int recursive=0, int depth=0, int ext=0, int prefix=0){
        meta_store->execQuery("DROP TABLE IF EXISTS filedata"); //TODO Delete this line for release
        meta_store->execQuery("CREATE TABLE IF NOT EXISTS filedata (key VARCHAR, collection VARCHAR DEFAULT(NULL), compressed BOOLEAN DEFAULT FALSE, path VARCHAR  DEFAULT '', fileextension VARCHAR DEFAULT '')");
        
        if (portable==0){
              std::vector<filedata> fd = asset_store->create(dir, recursive, depth, ext, prefix);
              #ifdef OUTPUTSHELL
              std::cout << "Added " << fd.size() << " assets" << std::endl;
              #endif
              for (int i=0; i< fd.size(); i++){
                meta_store->execQuery("INSERT INTO filedata values ('" + fd[i].key + "', " + fd[i].collection + " , " + (fd[i].compressed == true ? string("true") : string("false")) + ", '" + fd[i].path + "', '" + fd[i].fileextension + "')");
                
              }
             // meta_store->execQueryAndPrint("SELECT * FROM filedata;");
        }
        else {
          std::vector<filedata> fd = asset_store->createportable(dir, recursive, depth, ext, prefix);
          #ifdef OUTPUTSHELL
          std::cout << "Added " << fd.size() << " assets" << std::endl;
          #endif
              for (int i=0; i< fd.size(); i++){
                meta_store->execQuery("INSERT INTO filedata values ('" + fd[i].key + "', " + fd[i].collection + " , " + (fd[i].compressed == true ? string("true") : string("false")) + ", '" + fd[i].path + "', '" + fd[i].fileextension + "')");
                
              }
          }
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
          meta_store->execQueryAndPrint("DROP TABLE IF EXISTS metadata");
          //For some reason this doesn't work with the current version of duckdb. We will a wrapper for the csv_reader anyway in the close future
          //meta_store->execQueryAndPrint("CREATE TABLE IF NOT EXISTS metadata AS SELECT * FROM '" + file + "';");
          //This works, but we replaces it with a function of the framework implemented by the backend
          //meta_store->execQueryAndPrint("CREATE TABLE IF NOT EXISTS metadata AS SELECT * FROM read_csv_auto('" + file + "');");
          meta_store->execQueryAndPrint(meta_store->get_read_csv_query("metadata", file));
          meta_store->execQuery("DROP TABLE IF EXISTS metainfo;");
          meta_store->execQuery("CREATE TABLE metainfo (tablename VARCHAR, idcolname VARCHAR)");
          meta_store->execQuery("INSERT INTO metainfo VALUES ('metadata', '" + IDCol + "')");
          meta_store->idcolumn=IDCol;
         // meta_store->execQuery("CHECKPOINT");
      }
      
      void add_meta_table(std::string tablename, std::string foreign_key, std::string columns){
            meta_store->execQuery("select count(*) from metadata");
            if (!(meta_store->success)) {
                std::cerr << "Please create a central meta data table first before adding additional tables. Use one of the following functions: load_metadata_from_file, load_metadata_from_remote, load_metadata_advanced, create_empty_metadata_table\nMission abort!\n";
                return;
            }
          
            meta_store->execQuery("select count("+foreign_key+") from metadata");
            if (!(meta_store->success)) {
              std::cerr << foreign_key << " is not a column of the metadata table. Mission abort!\n";
              return;
            }
          
            meta_store->execQueryAndPrint("DROP TABLE IF EXISTS " + tablename );
            
            meta_store->execQueryAndPrint("CREATE TABLE " + tablename + " (" + columns + ")");
            meta_store->execQueryAndPrint("INSERT INTO metainfo VALUES ('"+ tablename +"', '" + foreign_key + "')");
      }
      
      /**
       * Add a metadata table from a csv file, but define own column names and types
       */
      void add_meta_table_advanced(std::string tablename, std::string foreign_key, std::string file, int skiplines, std::string columns){
      
          meta_store->execQuery("select count(*) from metadata");
          if (meta_store->success==false) {
              std::cerr << "Please create a central meta data table first before adding additional tables. Use one of the following functions: load_metadata_from_file, load_metadata_from_remote, load_metadata_advanced\nMission abort!\n";
              return;
          }
          
          meta_store->execQuery("select count("+foreign_key+") from metadata");
          if (meta_store->success == false) {
              std::cerr << foreign_key << " is not a column of the metadata table. Mission abort!\n";
              return;
          }
          
          if (columns.find(foreign_key)==string::npos){
                std::cerr << foreign_key << " is not in the list of provided columns. Mission abort!";
                return;
          }
          meta_store->execQuery("DROP TABLE IF EXISTS " + tablename);
          meta_store->execQuery("CREATE TABLE " + tablename + "("+ columns + ")");
          meta_store->printResult();
          meta_store->execQuery(meta_store->get_read_csv_query(tablename, file, true, skiplines));
          meta_store->printResult();
          meta_store->execQuery("INSERT INTO metainfo VALUES ('"+ tablename +"', '" + foreign_key + "')");
      }
      
      void load_metadata_from_remote(std::string address, std::string IDCol){
          std::string id= address.erase (address.find_last_not_of('.') - 1 , address.size()-1 );
          id = id.erase(0,std::min(id.find_last_of('[') + 1, id.size() - 1));
          asset_store->dl->download(address, id);
          load_metadata_from_file(id, IDCol);
          remove (id.c_str()); 
      };
      
      /**
       * If the meta-data file is not trivially readable by the metadata store, use this function to provide more information about your file.
       * @param file A file in CSV format
       * @param skiplines The number of lines to skip at the beginning of the file. This can be useful if the file contains any documentation or column headers.
       * @param columns A comma separated list defining pairs of column/property names and their type. Provide the name first, then the type, e.g. myID int, myProperty varchar.
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
        #ifdef OUTPUTSHELL
        std::cout << createquery << std::endl;
        #endif
        
        meta_store->execQuery(createquery);
        meta_store->printResult();
       // meta_store->execQuery("COPY metadata FROM '" + file + "' (AUTO_DETECT TRUE, skip " + to_string(skiplines) + ")");
        meta_store->execQuery(meta_store->get_read_csv_query("metadata", file, true, skiplines));
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
      
      /**
       * Create am empty central metadata table. Column names and types have to be provided, as well as the index of the column containing the primary key (=the unique identifier)
       */
      void create_empty_metadata_table(std::string columns, int IDColumn=0){
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
        #ifdef OUTPUTSHELL
        std::cout << createquery << std::endl;
        #endif
        
        meta_store->execQuery(createquery);
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
        
        meta_store->execQuery("DROP TABLE IF EXISTS metainfo;");
        meta_store->execQuery("CREATE TABLE metainfo (tablename VARCHAR PRIMARY KEY, idcolname VARCHAR)");
        meta_store->execQuery("INSERT INTO metainfo VALUES ('metadata', '" + IDCol + "')");
      }
      
      void load_metadata_advanced_from_remote(std::string address, int skiplines, std::string columns, int IDColumn=0){
          std::string id= address.erase (address.find_last_not_of('.') - 1 , address.size() );
          id = id.erase(0,std::min(id.find_last_of('[') + 1, id.size() - 1));
          asset_store->dl->download(address, id);
          load_metadata_advanced(id, skiplines, columns, IDColumn);
          remove (id.c_str()); 
      };
      
      std::string get(std::string id, int assetToFile = 0, std::string fileextension="", int metaToFile = 0, std::string fileextension_meta=""){
        #ifdef OUTPUTSHELL
        std::cout << "\033[36mReturning asset and metadata \033[0m\n";
        #endif
        std::string result="";
        if (assetToFile != 0){
            asset_store->getSingleToFile(id, fileextension);
        }else{
            result=asset_store->getSingle(id);
        }
        if(metaToFile != 0){
            meta_store->getSingleToFile(id, fileextension_meta);
        }else{
            meta_store->getSingle(id);
        }
        #ifdef OUTPUTSHELL
        std::cout << meta_store->getResultAsString() << std::endl;
        std::cout << "________________" << std::endl << std::endl;
        #endif
        
        return result;
      };
      
      std::vector<std::string> get(std::vector<std::string> id, int assetToFile = 0, std::string fileextension="", int metaToFile = 0, std::string fileextension_meta=""){
        std::vector<std::string> result;
        //std::cout << "\033[36mReturning assets and metadata \033[0m\n";
        //if (id.size() == 0) std::cout << "\033[31mNothing to do\033[0m\n";
        
        if(metaToFile == 1){

                meta_store->writeResultToFile("metadata", fileextension_meta);
                }
        if (metaToFile == 0){

              meta_store->printResult();
          }
        for (int i=0; i<id.size();i++){
          if (assetToFile == 0){
              result.push_back(asset_store->getSingle(id[i]));
          }
          else if (assetToFile == 1){
              asset_store->getSingleToFile(id[i], fileextension);
          }
          else{


          }


        }
        //std::cout << "________________" << std::endl << std::endl;
        return result;
      };
      
     std::vector<std::string> get_by_constraint(std::string constraint, int assetToFile = 0, std::string fileextension="", int metaToFile = 0, std::string fileextension_meta=""){
        std::vector<std::string> result;

        constraint = (constraint.compare(1,3,"all") == 0) ? " " : (" WHERE " + constraint);
        

        
        std::vector<std::string> ids = meta_store->getIDsByConstraint(constraint,false);
        //std::string query = "SELECT * FROM metadata " + constraint + " ORDER BY " + meta_store->idcolumn;
        //meta_store->execQuery(query);
        //meta_store->getIDsByConstraint(constraint,false);
        result=get(ids,assetToFile,fileextension,metaToFile,fileextension_meta);
        
        return result;
        
      };

     std::vector<std::string> get_IDs_by_constraint(std::string constraint){
        std::vector<std::string> result;

        constraint = (constraint.compare(1,3,"all") == 0) ? " " : (" WHERE " + constraint);
        
        std::vector<std::string> ids = meta_store->getIDsByConstraint(constraint);

        return ids;
        
      };
      
      //stores filters as view
      void store_filter(std::string constraint, std::string filtername, std::string filtertext){
        
        //save the name of the view
        std::string query="INSERT INTO filter VALUES ('" + filtername + "', '" + filtertext +"')";
        meta_store->execQuery(query);
        
        //create a new view
        constraint = (constraint.compare(1,3,"all") == 0) ? " " : (" WHERE " + constraint);
        std::string query2="CREATE VIEW " + filtername + " AS SELECT " + meta_store->idcolumn + " FROM metadata " + constraint + " ORDER BY " + meta_store->idcolumn;
        meta_store->execQuery(query2);
      }
      
      std::vector<std::string> apply_filter(std::string filtername, int assetToFile = 0, std::string fileextension="", int metaToFile = 0, std::string fileextension_meta=""){

        std::string query = "SELECT * FROM " + filtername;
        meta_store->execQuery(query);
     
        std::vector<std::string> ids = meta_store->crop_and_split_result(meta_store->getResultAsString());
        //std::vector<std::string> result=get(ids,assetToFile,fileextension,metaToFile,fileextension_meta);
        return ids;
        
      }
      
    void remove_filter(std::string filtername){
        
          std::string query="DROP VIEW " + filtername + ")";
          meta_store->execQuery(query);
          
          std::string query2="DELETE FROM filter WHERE filtername = '" + filtername + "'";
          meta_store->execQuery(query2);
    }
    
    std::vector<std::string> get_all_filters(){
        std::string query="SELECT filtername from filter";
        meta_store->execQuery(query);
        std::vector<std::string> filters = meta_store->crop_and_split_result(meta_store->getResultAsString());
        return filters;
    }
    
    std::string get_filter_text(std::string filtername){
        std::string query="SELECT filtertext from filter WHERE filtername = '" + filtername + "'";
        meta_store->execQuery(query);
        return meta_store->crop_single_result(meta_store->getResultAsString());
    }
      
     std::vector<std::string> get_all_assets(int assetToFile = 0, std::string fileextension = "",
       int metaToFile = 0, std::string fileextension_meta = "", int ids_only = 0) {
       std::vector<std::string> ids = meta_store->getIDsByFileData();
        if (ids_only != 0) return ids;
        
       std::vector<std::string> result;
       result =
           get(ids, assetToFile, fileextension, metaToFile, fileextension_meta);
       return result;
     };

     void remove_asset(std::string id, int remove_metadata=0){
        asset_store->remove(id);
        if (remove_metadata!=0){
            meta_store->execQuery("DELETE FROM metadata WHERE metadata." + meta_store->idcolumn + " = '" + id + "'");
        }
      };
      
      void import_asset_store(std::string file){
        asset_store->import(file);
        return;
      }
      /**
       * Redefines the columns for the central meta data table.
       * NOTE!!!! This deletes all existing entries.
       * @param coldef A comma seperated list of name-type tuples, e.g.: "my_name VARCHAR, my_value FLOAT" creates two columns called my_name and my_float which are of a text type and a floating point number type.
       * @param IDname The index of the column which contains the unique ID
       */
      void define_user_meta_cols(std::string coldef, int IDColumn = 0){
        
        meta_store->execQuery("DROP TABLE IF EXISTS metadata");
        std::string createquery = "CREATE TABLE metadata("+ coldef + ")";
        
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
        #ifdef OUTPUTSHELL
        std::cout << createquery << std::endl;
        #endif
        
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
        meta_store->execQuery(createquery);
        meta_store->execQuery("INSERT INTO metainfo VALUES ('metadata', '" + IDCol + "')");
      }
      
      /**
       * Inserts a new row into the use metadata table. Do not use this to bulk insert data, it will be slow. Use load_metadata_advanced or load_metadata_from_file instead.
       * @param values A comma separated list of all values of the tuple to be inserted
      */
      void insert_new_meta_row(std::string values, std::string tablename="metadata"){
         meta_store->execQuery("INSERT INTO "+tablename+" VALUES (" + values + ")"); 
      }
      
      void testout();
      /*TODO 
        - Wrapper for loading assets and/or metadata from file or internet
        - More intuitive way of inserting new meta data
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
