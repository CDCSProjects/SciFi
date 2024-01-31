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
        execQuery("CREATE TABLE IF NOT EXISTS filter (filtername VARCHAR, filtertext VARCHAR)");
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

std::string MetaStorage::construct_meta_query(std::string pdbid){
    std::string query = "SELECT * FROM ";// = "SELECT * FROM metadata where " + idcolumn + " = '" + pdbid + "'";
    std::vector<std::string> metatables = get_all_meta_tables();
    for (int i=0; i< metatables.size()-1;i++) {
        query.append(metatables[i] + ", ");
    }
    query.append(metatables[metatables.size()-1] + " WHERE ");
    
    for (int i=0; i< metatables.size();i++) {
        if ( metatables[i]!="metadata"){
          execQuery("SELECT idcolname FROM metainfo WHERE tablename='" + metatables[i] + "'");
          std::string joincol=crop_single_result(getResultAsString());
          //add the join condition
          if (i==1) query.append(" metadata."); //first table is always metadata
          else query.append(" AND metadata.");
          query.append(joincol);
          query.append("=");
          query.append(metatables[i]);
          query.append(".");
          query.append(joincol);
        }
    }
    if (metatables.size() > 1){
        query.append(" AND metadata." + idcolumn + " = '" + pdbid + "'");
    }
    else{
        query.append(" metadata." + idcolumn + " = '" + pdbid + "'");
    }
    
    return query;
}

std::string MetaStorage::getSingle(std::string pdbid){
    std::string query = construct_meta_query(pdbid);
    execQuery(query);
    #ifdef OUTPUTSHELL
    std::cerr << "Query: " << query << std::endl;
    std::cerr << getResultAsString();
    #endif
    return getResultAsString();
}

void MetaStorage::getSingleToFile(std::string pdbid, std::string p_fileextension){
    std::string query = construct_meta_query(pdbid);
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

std::vector<std::string> MetaStorage::get_all_meta_tables(){
    
    execQuery("SELECT tablename FROM metainfo;");
    return crop_and_split_result(getResultAsString());
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

bool MetaStorage::get_isNumeric_byAttr(std::string col_name, std::string table_name){
    std::string query = "SELECT count(numeric_scale) from information_schema.columns where column_name='" + col_name +"' AND table_name="+"'"+table_name+"'";
    execQuery(query);
    if (crop_single_result(getResultAsString()) == "0") return false;
    else return true;
}

bool MetaStorage::get_isText_byAttr(std::string col_name, std::string table_name){
    std::string query = "SELECT data_type from information_schema.columns where column_name='" + col_name +"' AND table_name="+"'"+table_name+"'";
    execQuery(query);
    if (crop_single_result(getResultAsString()).rfind("VARCHAR", 0) == 0) return true;
    if (crop_single_result(getResultAsString()) == "TEXT") return true;
    return false;
}

std::string MetaStorage::get_metaQuery_fromMetaData(std::string constraint){
    resultTablesMeta.clear();
    std::vector<std::string> metatables = get_all_meta_tables();
   // for (int i=0;i<metatables.size();i++) std::cout << "Metatable " << i << ": " << metatables[i] << std::endl;
    std::string jointables;
    std::string joincond;
    std::vector<std::string> jointablesV;
    //If there is more than just the metadata table, we need some joins
    if (metatables.size()>1){
     
      //Scan constraint for tables
      std::size_t pos = 0;
      std::string before = "";
      std::string after = constraint;
      int firsttab=0;
      while (pos!=std::string::npos){
          pos = after.find_first_of('.');
          if (pos==std::string::npos) break;
          before = after.substr(0,pos);
          after = after.substr((int)pos+1);
          size_t pos2 = before.find_last_of(' ');
          size_t pos3=(int)pos2+1;
          std::string tab = before.substr(pos3);
          //std::cout << "before: " << before << ", after: " << after << ", tab: " << tab << ", pos2: " << pos2 << ", pos3: " << pos3 << std::endl;
          
          
          
          //Check if tables in constraint string exist
         if (std::find(metatables.cbegin(), metatables.cend(), tab)!=metatables.cend()){
            if (tab!="metadata"){
                //Only insert table if we haven't inserted it before
                if (std::find(jointablesV.cbegin(), jointablesV.cend(), tab)==jointablesV.cend()){
                  jointables.append(tab);
                  jointables.append(", ");
                  jointablesV.push_back(tab);
                  //get the foreign key of the table
                  execQuery("SELECT idcolname FROM metainfo WHERE tablename='" + tab + "'");
                  std::string joincol=crop_single_result(getResultAsString());
                  //add the join condition
//                  std::cout << "constraint size: " << constraint.size() << "\nConstraint:" << constraint << std::endl;
                  if (firsttab==0 && constraint.size()<3){
                    joincond.append(" metadata.");
                    firsttab++;}
                    else joincond.append(" AND metadata.");
                  joincond.append(joincol);
                  joincond.append("=");
                  joincond.append(tab);
                  joincond.append(".");
                  joincond.append(joincol);
                }
            }
        }else{
            std::cerr << "Table '" << tab << "' does not exist. Quitting\n";
            std::string empty;
            return empty;
        }
      }
      

      
    }
    jointables.append("metadata ");

    //Construct query    
    std::string query;
    query = "SELECT DISTINCT metadata." + idcolumn + " FROM " + jointables + " " + constraint + " " + joincond;
    return query;
}

std::vector<std::string> MetaStorage::getIDsByConstraint(std::string constraint, bool ids_only, bool join_meta_tables){

    resultTablesMeta.clear();
    std::vector<std::string> metatables = get_all_meta_tables();
   // for (int i=0;i<metatables.size();i++) std::cout << "Metatable " << i << ": " << metatables[i] << std::endl;
    std::string jointables;
    std::string joincond;
    std::vector<std::string> jointablesV;
    //If there is more than just the metadata table, we need some joins
    if (metatables.size()>1){
     
      //Scan constraint for tables
      std::size_t pos = 0;
      std::string before = "";
      std::string after = constraint;
      int firsttab=0;
      while (pos!=std::string::npos){
          pos = after.find_first_of('.');
          if (pos==std::string::npos) break;
          before = after.substr(0,pos);
          after = after.substr((int)pos+1);
          size_t pos2 = before.find_last_of(' ');
          size_t pos3=(int)pos2+1;
          std::string tab = before.substr(pos3);
          //std::cout << "before: " << before << ", after: " << after << ", tab: " << tab << ", pos2: " << pos2 << ", pos3: " << pos3 << std::endl;
          
          
          
          //Check if tables in constraint string exist
         if (std::find(metatables.cbegin(), metatables.cend(), tab)!=metatables.cend()){
            if (tab!="metadata"){
                //Only insert table if we haven't inserted it before
                if (std::find(jointablesV.cbegin(), jointablesV.cend(), tab)==jointablesV.cend()){
                  jointables.append(tab);
                  jointables.append(", ");
                  jointablesV.push_back(tab);
                  //get the foreign key of the table
                  execQuery("SELECT idcolname FROM metainfo WHERE tablename='" + tab + "'");
                  std::string joincol=crop_single_result(getResultAsString());
                  //add the join condition
//                  std::cout << "constraint size: " << constraint.size() << "\nConstraint:" << constraint << std::endl;
                  if (firsttab==0 && constraint.size()<3){
                    joincond.append(" metadata.");
                    firsttab++;}
                    else joincond.append(" AND metadata.");
                  joincond.append(joincol);
                  joincond.append("=");
                  joincond.append(tab);
                  joincond.append(".");
                  joincond.append(joincol);
                }
            }
        }else{
            std::cerr << "Table '" << tab << "' does not exist. Quitting\n";
            std::vector<std::string> empty;
            return empty;
        }
      }
      

      
    }
    jointables.append("metadata ");

    //Construct query    
    std::string query;
    std::vector<std::string> ids;
    //Only return the IDs that match the query, not all the available metadata for these IDs
    if (ids_only){
        
        query = "SELECT DISTINCT metadata." + idcolumn + " FROM " + jointables + " " + constraint + " " + joincond;
        #ifdef OUTPUTSHELL
        std::cout << "\033[36mMetastore full SQL query:\033[0m\n" << query << std::endl;
        #endif
        execQuery(query);
        return format_getIDsByConstraint();
    }
    //Query all available metadata by joining the available user-defined meta tables
    else if (join_meta_tables){
        query = "DROP TABLE IF EXISTS res";
        execQuery(query);
        query = "DROP TABLE res";
        #ifdef OUTPUTSHELL
        std::cout << "\033[36mMetastore full SQL query:\033[0m\n" << query << std::endl;
        #endif
        execQuery(query);
        
        //insert all other meta tables which are not part of the query
        std::string jointables2="";
       // std::string joincond2="";
        bool firstset=false;
        for (int i=0; i<metatables.size(); i++){
          if (metatables[i]!="metadata"){  
            if (std::find(jointablesV.cbegin(), jointablesV.cend(), metatables[i])==jointablesV.cend()){
                execQuery("SELECT idcolname FROM metainfo WHERE tablename='" + metatables[i] + "'");
                std::string joincol=crop_single_result(getResultAsString());
                if (firstset) {
                   jointables2.append(" FULL OUTER JOIN ");
                }else{
                    
                    firstset=true;
                }
                jointables2.append(metatables[i] + " ON " + metatables[i] + "." + joincol + " = metadata." + joincol);
                /*if (i>0) {joincond2.append(" ON " + metatables[i] + "." + joincol + " = metadata." + joincol);}
                else{joincond2.append(" ON " + metatables[i] + "." + joincol + " = metadata." + joincol);}
                if (i==metatables.size()-1){
                  jointables2.append(metatables[i]);
                  break;
                }
                if (i<metatables.size()-1) jointables2.append(metatables[i] + " OUTER JOIN ");
                else jointables2.append(metatables[i] + " ");*/
            }
          }
        }
        execQuery(query);
         #ifdef OUTPUTSHELL
        std::cout << "\033[36mMetastore full SQL query:\033[0m\n" << query << std::endl;
        #endif
        //std::cout << "Size metatables: " << metatables.size() << ", size jointables:" << jointablesV.size() << std::endl;
        if ((metatables.size()-jointablesV.size())>1){
            query = "DROP VIEW IF EXISTS othermeta";
            execQuery(query);
            query = "CREATE VIEW othermeta AS SELECT * from metadata FULL OUTER JOIN " + jointables2;
            execQuery(query);
        }
        execQuery("SELECT idcolname FROM metainfo WHERE tablename='metadata'");
        std::string joincol=crop_single_result(getResultAsString());
        if ((metatables.size()-jointablesV.size())>1){
        query = "CREATE TABLE res AS SELECT * FROM " + jointables + " LEFT OUTER JOIN othermeta ON othermeta." + joincol + "=metadata." + joincol + " " + constraint + " " + joincond;
        }else
        query = "CREATE TABLE res AS SELECT * FROM " + jointables + constraint + " " + joincond;
        #ifdef OUTPUTSHELL
        std::cout << "\033[36mMetastore full SQL query:\033[0m\n" << query << std::endl;
        #endif
        execQuery(query);
                
        //query = "SELECT metadata." + idcolumn + " FROM " + jointables + " " + constraint + " " + joincond;
        query = "SELECT " + idcolumn + " FROM res GROUP BY " + idcolumn;
        #ifdef OUTPUTSHELL
        std::cout << "\033[36mMetastore full SQL query:\033[0m\n" << query << std::endl;
        #endif
        execQuery(query);
        
        ids = format_getIDsByConstraint();
        query="SELECT * FROM res";
        #ifdef OUTPUTSHELL
        std::cout << "\033[36mMetastore full SQL query:\033[0m\n" << query << std::endl;
        #endif
        execQuery(query);
        
        return ids;
    }
    //Do not join the meta tables. The results are stored in a vector of string vectors (member: resultTablesMeta). The order is the same as in the result of get_all_meta_tables();
    else{
    
        query = "SELECT DISTINCT metadata." + idcolumn + " FROM " + jointables + " " + constraint + " " + joincond;
        #ifdef OUTPUTSHELL
        std::cout << "\033[36mMetastore full SQL query:\033[0m\n" << query << std::endl;
        #endif
        execQuery(query);
        ids = format_getIDsByConstraint(); //get all IDs

        for (int i=0; i<ids.size(); i++){
            std::vector<std::string> tempres;
            //Get records from the main metadata table
            execQuery("SELECT idcolname FROM metainfo WHERE tablename='metadata'");
            std::string joincol=crop_single_result(getResultAsString());
                      
            query = "SELECT * FROM metadata WHERE " + joincol + " = '" + ids[i] + "'";
            #ifdef OUTPUTSHELL
            std::cout << "\033[36mMetastore full SQL query:\033[0m\n" << query << std::endl;
            #endif
            execQuery(query);
            
            tempres.push_back(getResultAsString());

            //create a new table from metadata that only contains the record with our ID to speed up the other queries
            execQuery("CREATE OR REPLACE TABLE tempmeta AS SELECT * FROM metadata WHERE " + joincol + "='" +ids[i]+"'");
            //Get records from the other meta data tables
            for (int j=0; j<metatables.size(); j++){
              if (metatables[j]!="metadata"){  
                    execQuery("SELECT idcolname FROM metainfo WHERE tablename='" + metatables[j] + "'");
                    joincol=crop_single_result(getResultAsString());
                    execQuery("SELECT * FROM " + metatables[j]  + " SEMI JOIN tempmeta ON " +" tempmeta." + joincol + "=" + metatables[j] + "." + joincol + " AND tempmeta." + joincol + "='"+ids[i]+"'");   
                    std::string r= getResultAsString();
                    tempres.push_back(r);
                }
            }
            resultTablesMeta.push_back(tempres);
        }
    }
    return ids;
}

void MetaStorage::writeResultMetaTablesToFile(std::string filename, std::string fileextension){
    std::vector<std::string> metatables = get_all_meta_tables();
    std::ofstream o;
    o.open(resultfolder + "/" + filename + fileextension); 
    for (int j=0; j < resultTablesMeta.size(); j++){
        o << "Result " << j << ": " << std::endl;
      for (int i=0; i< metatables.size();i++){
        o << "Meta data Table " << metatables[i] << ": " << std::endl;
      //  for (int k=0; k< resultTablesMeta[j][i].size();k++){
            o << resultTablesMeta[j][i] << std::endl;
       //   }
          o << std::endl;
    }
    }
    o.close();
    #ifdef OUTPUTSHELL
    std::cout << "\033[32mMetadata written to " << filename + fileextension << "\033[0m" << std::endl;
    #endif
}

void MetaStorage::printResultMetaTables(){
    std::vector<std::string> metatables = get_all_meta_tables();
    for (int j=0; j < resultTablesMeta.size(); j++){
        std::cout << "Result " << j << ": " << std::endl;
      for (int i=0; i< metatables.size();i++){
        std::cout << "Meta data Table " << metatables[i] << ": " << std::endl;
        //for (int k=0; k< resultTablesMeta[j][i].size();k++){
            std::cout << resultTablesMeta[j][i] <<std::endl;
         // }
          std::cout << std::endl;
    }
    }
}
//TODO get_isBoolean

}   
