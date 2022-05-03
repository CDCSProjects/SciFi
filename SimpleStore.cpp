#include "asshell.h"

int main(int argc, char *argv[]){

    SciStore::Storage<> * st = new SciStore::Storage<>("My_Assetstore","Test_Metastore");
    st->meta_store->init("Test_Metastore");
    st->load_metadata_advanced("galaxy_data_Gendre_MiraBest_Capetti_Baldi_Proctor_split.csv",1,"RA_J2000_degree FLOAT, DEC_J2000_degree FLOAT, Type1 FLOAT, Type2 FLOAT, Source VARCHAR, filepath_literature VARCHAR, filepath_CDL VARCHAR, coord_str VARCHAR, included_LOFAR_FIRST BOOL, split_literature VARCHAR, split_CDL VARCHAR", 5);
    SciStore::AsShell * as = new SciStore::AsShell(st);
    as->run();

    return 0;
}


