#include <iostream>
#include <string>
#include <list>
#include <cstdlib>
#include "testwrapper.h"

namespace Test{

Customer::Customer(int no, std::string s, std::string c){
			customer_id = rand();
		    shipping_address = Address(no,s,c);
		}

void Customer::show_data(){
			std::cout << "Customer Id:" << customer_id << "\n";
			shipping_address.print_address();
		}

Customer::Address Customer::get_shipping(){
			return shipping_address;
		}


template <typename T>
Purchase<T>::Purchase (T p, Customer* c) {
            purchase = p;
            customer = c;
            purchase_id = rand();
            
        }
template <typename T>
        void Purchase<T>::get_purchase_overview(){
            std::cout << "Overview of Purchase " << purchase_id << "\n";
            std::cout << "Of Customer: " << customer->customer_id << "\n";
            std::cout << "Costs: " << cost << "\n";
        }
template <typename T>
        Customer Purchase<T>::get_customer() {
            return *customer;
        }
template class Purchase<std::list<Product>>;
template class Purchase<Product>;
        
Product::Product(){
    product_id = rand() % 1000+1;
            price = 0;
}
Product :: Product(float p){
            product_id = rand() % 1000+1;
            price = p;
        }
        
        void Product :: overview() {
            std::cout << product_id <<"\n" << price << "\n";
        }

Book::Book(){
    isbn = rand() %1000+1;
    author = "";
    price = 0;
}

Book :: Book(int no,std::string name, float p) {
            isbn = no;
            author = name;
            price = p;
        }
        void Book :: overview_book(){
            overview();
            std::cout << isbn << "\n" << author<<"\n";
        }

}





