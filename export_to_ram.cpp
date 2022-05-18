

#include "storage.h"

int main(int argc, char *argv[]){


  if (argc != 3) {
      printf("Usage: %s <dbname> [all|<key=value>]\n", argv[0]);
      return 1;
    }

  string dbname = argv[1];
  SciStore::Storage<> * st = new SciStore::Storage<>(dbname + "_asset",dbname + "_meta", "pictures", 1);

  std::vector<std::string> res;
  if (strcmp(argv[2],"all")==0) {
    res = st->get_all_assets(1, ".png", 2);
  } else {
    res = st->get_by_constraint(argv[2], 1, ".png", 2);
  }

 // for (int i = 0; i < res.size(); i++) {
 //   std::cout << res[i];
 // }
  std::cerr << res.size() << std::endl;
  //std::cout << res[0];

  return 0;
}
