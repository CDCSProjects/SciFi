#include "asshell.h"

namespace SciStore {
  static void addmetadata(Storage<> * st){
      std::string argument;
      std::cout << "Please provide a csv file: ";
      std::cin >> argument;
      st->load_metadata_advanced(argument, 1, "RA_J2000_degree FLOAT, DEC_J2000_degree FLOAT, Type1 FLOAT, Type2 FLOAT, Source VARCHAR, filepath_literature VARCHAR, filepath_CDL VARCHAR, coord_str VARCHAR, included_LOFAR_FIRST BOOL, split_literature VARCHAR, split_CDL VARCHAR", 5);
  }
  
  static void getallIDs(Storage<> * st){
      st->meta_store->execQueryAndPrint("Select " + st->meta_store->idcolumn + " FROM metadata");
  }
}

int main(int argc, char *argv[]){

    SciStore::Storage<> * st = new SciStore::Storage<SciStore::RocksStore, SciStore::DuckStore>("galaxyasset","galaxymeta");
    SciStore::AsShell * as = new SciStore::AsShell(st);
    
    as->register_function("addmeta",&SciStore::addmetadata,"Add metadata to store");
    as->register_function("getids",&SciStore::getallIDs,"Get all available IDs from the meta store");
    
    as->run();

    delete st;
    delete as;
    return 0;
}

