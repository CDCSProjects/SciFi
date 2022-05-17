#include "asshell.h"

int main(int argc, char *argv[]){

    //The most simple way to run a SciFi shell
    SciStore::Storage<> * st = new SciStore::Storage<>("My_Assetstore","Test_Metastore");
    SciStore::AsShell * as = new SciStore::AsShell(st);
    as->run();
    
    //Another example using pdb data
   /* SciStore::Storage<> * st = new SciStore::Storage<>("My_Assetstore","Test_Metastore", "testfolder",1);
    st->load_metadata_from_file(filename,col);
    st->load_assets_from_directory(directory, portable, recursive, 0, 0, 0);
    std::vector<std::string> ids;
    ids.push_back("100d");
    ids.push_back("11as");
    std::vector<string> res = st->get(ids,1);*/
   
    return 0;
}


