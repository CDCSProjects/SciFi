#include "testwrapper.h"
#include <iostream>
#include <string>
#include <cstdlib>


int main (){
	Test::Customer customer_1(8,"mainroad", "hamburg");
	customer_1.show_data();
	Test::Book book_1(9345,"murakami", 9.9);
	book_1.overview_book();
	Test::Product product_1(0.5);
	Test::Product product_2(1.5);
	Test::Purchase <std::list<Test::Product>> purchase({product_1,product_2, book_1}, &customer_1);
	purchase.get_customer().show_data();
	purchase.get_customer().shipping_address.print_address();
	Test::Customer::Address address_test= Test::Customer::Address(44,"main","city");
    address_test.print_address();
}
