#include <shell/include/asshell.h>
#include <chrono>

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

    //SciStore::Storage<> * st = new SciStore::Storage<SciStore::RocksStore, SciStore::DuckStore>("galaxyasset","galaxymeta");
    SciStore::Storage<> * st = new SciStore::Storage<SciStore::RocksStore, SciStore::DuckStore>("pdbassetportable","pdbmetaportable");
    SciStore::AsShell * as = new SciStore::AsShell(st);
    
    auto start = std::chrono::steady_clock::now();
    st->load_assets_from_directory("/mnt/c/Users/ann__/Documents/pdb/pdb",1,1,0,0,3);
    auto end = std::chrono::steady_clock::now();
    auto time=std::chrono::duration_cast<std::chrono::hours>(end-start).count();
    auto time_min=std::chrono::duration_cast<std::chrono::minutes>(end-start).count();
    std::cout << "Import done. Elapsed time: " << time << "hours or " << time_min << " minutes\n";
    as->register_function("addmeta",&SciStore::addmetadata,"Add metadata to store");
    as->register_function("getids",&SciStore::getallIDs,"Get all available IDs from the meta store");
    
    //as->run();

    delete st;
    delete as;
    return 0;
}

