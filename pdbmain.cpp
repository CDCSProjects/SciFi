#include <string>
//#include "storage.h"
#include "asshell.h"

int main(int argc, char *argv[]){

    if (argc<2 || (strcmp(argv[1], "-h") == 0)) {
        std::cout << "No task provided. Exit.\n";
        std::cout << "Usage: pdbkv <task> [pdbID/sst-file]\n";
        std::cout << "Tasks:\n\tcreate - Create new key-value store from files in \"pdb\" folder\n\tget - Return the contents of a pdb file\n";
        std::cout << "\timport - Import data from an sst file\n\tcreateportable - Create a portable sst file from files in \"pdb\" folder\n";
        std::cout << "\treload: Reload the files from online pdb\n\texamples - Run some example queries\n\n";
        std::cout << "pdbID: e.g. 4BT3, necessary for get task\n";
        std::cout << "sst-file: A data file to import into a fresh database, necessary for import task\n\n";

        std::cout << "Exit codes: 0 - success, 1 - no task provided, 2 - third argument is missing\n";
        return 1;
    }

    SciStore::Storage<> * st = new SciStore::Storage<SciStore::RocksStore, SciStore::DuckStore>("pdbtest","pdbmeta");
    
    

    if (strcmp(argv[1], "interactive") == 0) {
        SciStore::AsShell * as = new SciStore::AsShell(st);
        as->run();
        
    }
    
    if (strcmp(argv[1], "examples") == 0) {
        st->load_metadata_advanced("compound.idx", 4, "IDCODE VARCHAR, COMPOUND VARCHAR", 0);
        st->get_by_constraint("IDCODE = '4BT3'",1,"pdb",1,"txt");
        st->get_by_constraint("compound like '%Hydroxyisocaproyl%'",1,"pdb",1,"txt");
        st->get_by_constraint("IDCODE = 'ABC'",1,"pdb",1,"txt");
        st->get("4BT3",1,"pdb",1,"txt");
    }
    
    if (strcmp(argv[1], "create") == 0) {
        st->load_assets_from_directory("pdb");
        return 0;
    }
    if (strcmp(argv[1], "get") == 0 ) {
        if (argc < 3) { std::cout << "No pdbID provided. Exit.\n"; return 2; }
        else st->get(argv[2],1,"pdb",0,".txt");
        return 0;
    }     
    if (strcmp(argv[1], "import") == 0 ) {
        if (argc < 3) { std::cout << "No file for import provided. Exit.\n"; return 2; }
        st->import_asset_store(argv[2]);
        return 0;
    }
    if (strcmp(argv[1], "createportable") == 0 ) {
        st->load_assets_from_directory("pdb", 1);
        return 0;
    }  
    
    if (strcmp(argv[1], "reread") == 0 ) {
        
    }
    delete st;
    return 0;
}