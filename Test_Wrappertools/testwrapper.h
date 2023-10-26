#ifndef TESTWRAPPER_H
#define TESTWRAPPER_H


#include <iostream>
#include <string>
#include <cstdlib>
#include <list>

namespace Test {

struct Customer {
    public:
        int customer_id;
        struct Address{
            public:
            int housenumber;
	        std::string street;
	        std::string city;

	       Address(){};
	       Address(int no, std::string s, std::string c){
	            housenumber = no;
                street = s;
                city = c;
	        }
	        void print_address(){
	            std::cout << "Address:\n";
			    std::cout << street <<" "<< housenumber << "\n" << city << "\n";
	        }

        };
        Address shipping_address;
        Customer(int no, std::string s, std::string c);
        void show_data();
        Address get_shipping();
};

template <typename T>
 class Purchase{
	public:
		T purchase;
		int purchase_id;
		float cost;
		Customer* customer;
		Purchase(T p, Customer* c);
		void get_purchase_overview();
		Customer get_customer();
};

class Product {
	public:
		int product_id;
		float price;
		Product();
		Product(float p);
		void overview();
};

class Book: public Product{
	public:
		int isbn;
		std::string author;
		Book();
		Book(int no, std::string name, float p);
		void overview_book();
};

}

#endif
