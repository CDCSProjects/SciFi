
#include "storage.h"

int main(int argc, char *argv[]){

  if (argc != 3) {
    printf("Usage: %s <dbname> <importpath>\n", argv[0]);
    return 1;
  }

  string dbname = argv[1];
  string importpath = argv[2];


  SciStore::Storage<> * st = new SciStore::Storage<>(dbname + "_asset",dbname + "_meta");
  st->load_assets_from_directory(importpath, 0, 0, 0, 0, 0);

  return 0;
}
