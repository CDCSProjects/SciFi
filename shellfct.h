#ifndef SHELLFCT_H
#define SHELLFCT_H

#include <cstdarg>
#include <string>
#include "storage.h"

typedef void (*ShellFunctionPtr)(SciStore::Storage<> * st);
typedef std::unordered_map<std::string, ShellFunctionPtr> function_map;
typedef std::unordered_map<std::string, std::string> doc_map;

namespace SciStore {

  static void importsst(Storage<> * st){
      std::string argument;
      std::cout << "Please provide the sst-file to import: ";
      std::cin >> argument;
      st->import_asset_store(argument);
  }
  
  static void getasset(Storage<> * st){
      std::string key;
      std::cout << "Please provide the key: ";
      std::cin >> key;
      st->get(key,1,st->afe,0,st->mfe);
  }
  
  static void changeext(Storage<> * st){
      std::string ext;
      std::cout << "Please provide the file extension for assets (currently '" << st->afe << "'): ";
      std::cin >> ext;
      if (ext != "none") st->afe=ext;
      else st->afe="";
      std::cout << "Please provide the file extension for meta data (currently '" << st->mfe << "'): ";
      std::cin >> ext;
      if (ext != "none") st->mfe=ext;
      else st->mfe="";
  }
  
  static void getfrommeta(Storage<> * st){
      std::string con;
      std::cout << "What are you looking for? (e.g. author = 'Klaus', compound like '%Hydroxyisocaproyl%'): ";
      std::string i;
      while (true){
        std::cin >> i;
        con+=" ";
        con+=i;
        if(std::cin.peek()=='\n')
          break;
      }
      st->get_by_constraint(con,1,st->afe,1,st->mfe);
  }
  
  static void createfromlocal(Storage<> * st){
      std::string dir;
      std::cout << "Please provide the directory: ";
      std::cin >> dir;
      char rec;
      std::cout << "Scan directory recursively? y/n: ";
      std::cin >> rec;
      char port;
      std::cout << "Create/add to portable sst file? y/n: ";
      std::cin >> port;
      int depth;
      if (rec=='y'){
        std::cout << "How many directories do you want to keep as part of the key? (0-9, type 0 if you are not sure, only works if you have this many subdirectories): ";
        std::cin >> depth;
        }else depth=0;
      char ext;
      std::cout << "Include file extension in key? y/n: ";
      std::cin >> ext;
      st->load_assets_from_directory(dir, port == 'y' ? 1 : 0, rec=='y' ? 1 : 0, depth, ext=='y' ? 1 : 0);
  }

}
#endif