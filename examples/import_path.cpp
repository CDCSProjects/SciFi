
#include <framework/include/storage.h>

int main(int argc, char *argv[]){

  if (!(argc == 3 || argc == 4)){
    printf("Usage: %s <dbname> <importpath> [metadatafile]\n", argv[0]);
    return 1;
  }

  string dbname = argv[1];
  string importpath = argv[2];



  SciStore::Storage<> * st = new SciStore::Storage<>(dbname + "_asset",dbname + "_meta");
  st->load_assets_from_directory(importpath, 0, 0, 0, 0, 0);
  //st->load_metadata_from_file(metadatapath, "key");  // load metadata from file
  if (argc == 4){
    string metadatapath = argv[3];
    st->load_metadata_advanced(metadatapath, 1, "key varchar, video varchar");  // load metadata from file
  }

  return 0;
}
