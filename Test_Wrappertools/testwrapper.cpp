#include <iostream>
#include <string>
#include <list>
#include <cstdlib>
#include "testwrapper.h"

namespace Test{
Address::Address(int no=0, std::string s="", std::string c=""){
			housenumber = no;
			street = s;
			city = c;
		}
void Address :: print_address(){
			std::cout << "Address:\n";
			std::cout << street <<" "<< housenumber << "\n" << city << "\n";
			
		}

Customer::Customer(int no = 0, std::string s = "", std::string c = ""){
			customer_id = rand();
		    	shipping_address = Address(no, s, c);
			
		}
void Customer :: show_data(){
			std::cout << "Customer Id:" << customer_id << "\n";
			shipping_address.print_address();
		}

template <typename T>
Purchase<T> :: Purchase( T p , Customer c) {
            purchase = p;
            customer = c;
            purchase_id = rand();
            
        }
template <typename T>
        void Purchase<T> :: get_purchase_overview(){
            std::cout << "Overview of Purchase " << purchase_id << "\n";
            std::cout << "Of Customer: " << customer.customer_id << "\n";
            std::cout << "Costs: " << cost << "\n";
        }
template <typename T>
        Customer Purchase<T> :: get_customer() {
            return customer;
        }
template class Purchase<std::list<Product>>;
template class Purchase<Product>;
        
         
Product :: Product(float p=0.0){
            product_id = rand() % 1000+1;
            price = p;
        }
        
        void Product :: overview() {
            std::cout << product_id <<"\n" << price << "\n";
        }


Book :: Book(int no = 0,std::string name = "", float p=0) {
            if (no == 0) {
                isbn = rand() % 1000 +1;
            }
            else {
              isbn = no;  
                
            }
            author = name;
            price = p;
        }
        void Book :: overview_book(){
            overview();
            std::cout << isbn << "\n" << author;
        }

}





