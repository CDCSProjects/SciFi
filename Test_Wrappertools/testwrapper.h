#ifndef TESTWRAPPER_H
#define TESTWRAPPER_H


#include <iostream>
#include <string>
#include <cstdlib>
#include <list>

namespace Test {

struct Address {
	int housenumber;
	std::string street;
	std::string city;		
	Address(int no=0, std::string s="", std::string="");
	void print_address();
};

struct Customer {
	int customer_id;
	Address shipping_address;
	Customer(int no=0, std::string s="", std::string c="");
	void show_data();

};

template <typename T> class Purchase{
	public:
		T purchase;
		int purchase_id;
		float cost;
		Customer customer;
		Purchase(T p, Customer c);
		void get_purchase_overview();
		Customer get_customer();
};

class Product {
	public:
		int product_id;
		float price;
		Product(float p = 0.0);
		void overview();
};

class Book: public Product{
	public:
		int isbn;
		std::string author;
		Book(int no=0, std::string name="", float p=0);
		void overview_book();
};
}

#endif
