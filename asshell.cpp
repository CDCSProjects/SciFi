<<<<<<< HEAD
#include "asshell.h"
namespace SciStore {

void AsShell::register_function(std::string option, ShellFunctionPtr func, std::string helptext){
    fmap[option]=func;
    dmap[option]=helptext;
}
//add your user defined functions here or call register_function from outside BEFORE calling run()
void AsShell::initialize_functions(){
    register_function("get",&getasset,"Get the asset and metadata for a key");
    register_function("getm", &getfrommeta, "Get the asset for metadata constraints");
    register_function("ext",&changeext,"Change the extensions for output files");
    register_function("scandir", &createfromlocal, "Add new assets by scanning a directory");
    register_function("import",&importsst,"Import a portable sst file into your local AssetStore");
}


AsShell::AsShell(){
    st=new SciStore::Storage<SciStore::RocksStore, SciStore::DuckStore>("assets","metadata");
    initialize_functions();
}

void AsShell::print_welcome_msg(){
        std::cout << "\033[33m ___  ___ \n";
        std::cout << "/   \\/ __|\n";
        std::cout << "| - |\\__ \\\tWelcome to the AssetStore!\n";
        std::cout << "|_|_||___/    ";
        std::cout << "\033[0m\n\n";
        std::cout << "q - Quit\n";
        std::cout << "h - Print help\n";
}

void AsShell::print_help_msg(){

        for (auto& x:fmap){
            std::cout << x.first << " - " << dmap.find(x.first)->second << "\n";
        }
        std::cout << std::endl;
}

int AsShell::run(){
        print_welcome_msg();
        //print_help_msg();
            
        while(1){

            std::cout << "AssetStore> ";
            std::string task;
            std::cin >> task;
            std::string argument;
             
            if (task == "q"){    
                  std::cout << "Bye!\n"; 
                  return 0;
            }
            if (task == "h"){
                    print_help_msg();
            }
            
            function_map::const_iterator found;
            found = fmap.find(task);
            if (found!=fmap.end()){
                (found->second)(st);
            }
            
        }
}
}
=======
#include "asshell.h"
namespace SciStore {

void AsShell::register_function(std::string option, ShellFunctionPtr func, std::string helptext){
    fmap[option]=func;
    dmap[option]=helptext;
}
//add your user defined functions here or call register_function from outside BEFORE calling run()
void AsShell::initialize_functions(){
    register_function("get",&getasset,"Get the asset and metadata for a key");
    register_function("getm", &getfrommeta, "Get the asset for metadata constraints");
    register_function("ext",&changeext,"Change the extensions for output files");
    register_function("scandir", &createfromlocal, "Add new assets by scanning a directory");
    register_function("import",&importsst,"Import a portable sst file into your local AssetStore");
}


AsShell::AsShell(){
    st=new SciStore::Storage<SciStore::RocksStore, SciStore::DuckStore>("assets","metadata");
    initialize_functions();
}

void AsShell::print_welcome_msg(){
        std::cout << "\033[33m ___  ___ \n";
        std::cout << "/   \\/ __|\n";
        std::cout << "| - |\\__ \\\tWelcome to the AssetStore!\n";
        std::cout << "|_|_||___/    ";
        std::cout << "\033[0m\n\n";
        std::cout << "q - Quit\n";
        std::cout << "h - Print help\n";
}

void AsShell::print_help_msg(){

        for (auto& x:fmap){
            std::cout << x.first << " - " << dmap.find(x.first)->second << "\n";
        }
        std::cout << std::endl;
}

int AsShell::run(){
        print_welcome_msg();
        //print_help_msg();
            
        while(1){

            std::cout << "AssetStore> ";
            std::string task;
            std::cin >> task;
            std::string argument;
             
            if (task == "q"){    
                  std::cout << "Bye!\n"; 
                  return 0;
            }
            if (task == "h"){
                    print_help_msg();
            }
            
            function_map::const_iterator found;
            found = fmap.find(task);
            if (found!=fmap.end()){
                (found->second)(st);
            }
            
        }
}
}
>>>>>>> e74854c726ae7ab7a38c5b60b9108fda5d6b0205
