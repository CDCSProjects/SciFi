#include "asshell.h"

int main(int argc, char *argv[]){

    SciStore::Storage<> * st = new SciStore::Storage<>("My_Assetstore","Test_Metastore");
    SciStore::AsShell * as = new SciStore::AsShell(st);
    as->run();

    return 0;
}


