

#include "storage.h"

int main(int argc, char *argv[]){


  if (argc != 3) {
      printf("Usage: %s <dbname> [all|<key=value>]\n", argv[0]);
      return 1;
    }

  string dbname = argv[1];
  SciStore::Storage<> * st = new SciStore::Storage<>(dbname + "_asset",dbname + "_meta");

  std::vector<std::string> ids = st->get_all_assets();
  std::vector<string> res = st->get(ids);
  std::cout << res[0];

  return 0;
}
