<<<<<<< HEAD
#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <string>
#include <fstream>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

namespace SciStore {
class Downloader{
        
    public:
        Downloader(){;}
        
        std::string source_compound="https://ftp.wwpdb.org/pub/pdb/derived_data/index/compound.idx";
        
        int download(std::string source,std::string dest){
        
          try
          	{
          		// That's all that is needed to do cleanup of used resources (RAII style).
          		curlpp::Cleanup myCleanup;
          
          		// Our request to be sent.
          		curlpp::Easy myRequest;
          
          		// Set the URL.
          		myRequest.setOpt<curlpp::options::Url>(source);
                
                std::ofstream fileout(dest);
		        myRequest.setOpt<curlpp::options::WriteStream>(&fileout);
          
          		// Send request and get a result.
          		// By default the result goes to standard output.
          		myRequest.perform();
                  
          	}
          
          	catch(curlpp::RuntimeError & e)
          	{
          		std::cout << e.what() << std::endl;
          	}
          
          	catch(curlpp::LogicError & e)
          	{
          		std::cout << e.what() << std::endl;
          	}
          return 0;
        }
        
        int download_by_pdb_id(std::string pdb_id){
           return download("https://files.rcsb.org/download/" + pdb_id + ".pdb.gz", pdb_id +".pdb.gz");
        }
                  
};
}

=======
#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <string>
#include <fstream>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

namespace SciStore {
class Downloader{
        
    public:
        Downloader(){;}
        
        std::string source_compound="https://ftp.wwpdb.org/pub/pdb/derived_data/index/compound.idx";
        
        int download(std::string source,std::string dest){
        
          try
          	{
          		// That's all that is needed to do cleanup of used resources (RAII style).
          		curlpp::Cleanup myCleanup;
          
          		// Our request to be sent.
          		curlpp::Easy myRequest;
          
          		// Set the URL.
          		myRequest.setOpt<curlpp::options::Url>(source);
                
                std::ofstream fileout(dest);
		        myRequest.setOpt<curlpp::options::WriteStream>(&fileout);
          
          		// Send request and get a result.
          		// By default the result goes to standard output.
          		myRequest.perform();
                  
          	}
          
          	catch(curlpp::RuntimeError & e)
          	{
          		std::cout << e.what() << std::endl;
          	}
          
          	catch(curlpp::LogicError & e)
          	{
          		std::cout << e.what() << std::endl;
          	}
          return 0;
        }
        
        int download_by_pdb_id(std::string pdb_id){
           return download("https://files.rcsb.org/download/" + pdb_id + ".pdb.gz", pdb_id +".pdb.gz");
        }
                  
};
}

>>>>>>> e74854c726ae7ab7a38c5b60b9108fda5d6b0205
#endif