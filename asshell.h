<<<<<<< HEAD
#ifndef ASSHELL_H
#define ASSHELL_H

#include <cstdarg>
#include <string>
#include "storage.h"
#include "shellfct.h"


namespace SciStore {


class AsShell{
    public:
        AsShell();
 
        template <class A, class B>
        AsShell(Storage<A,B>* p_st){
            st=p_st;
            initialize_functions();
        }
    
        int run();
        void register_function(std::string option, ShellFunctionPtr func, std::string helptext);
        
    protected:
        SciStore::Storage<> * st;
        function_map fmap;
        doc_map dmap;
        void print_welcome_msg();
        void print_help_msg();
        void initialize_functions();   
        /**Overwrite this if you want to add your own functions. 
         * You may also use the clear or erase functions on fmap and dmap to remove already existing functions from the list.
        */
        void initialize_user_functions(); 
        
};
}
#endif
=======
#ifndef ASSHELL_H
#define ASSHELL_H

#include <cstdarg>
#include <string>
#include "storage.h"
#include "shellfct.h"


namespace SciStore {


class AsShell{
    public:
        AsShell();
 
        template <class A, class B>
        AsShell(Storage<A,B>* p_st){
            st=p_st;
            initialize_functions();
        }
    
        int run();
        void register_function(std::string option, ShellFunctionPtr func, std::string helptext);
        
    protected:
        SciStore::Storage<> * st;
        function_map fmap;
        doc_map dmap;
        void print_welcome_msg();
        void print_help_msg();
        void initialize_functions();   
        /**Overwrite this if you want to add your own functions. 
         * You may also use the clear or erase functions on fmap and dmap to remove already existing functions from the list.
        */
        void initialize_user_functions(); 
        
};
}
#endif
>>>>>>> e74854c726ae7ab7a38c5b60b9108fda5d6b0205
