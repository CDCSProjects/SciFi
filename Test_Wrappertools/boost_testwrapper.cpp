#include "testwrapper.h"
#include"testwrapper.cpp"
#include <boost/python.hpp>


using namespace boost::python;


BOOST_PYTHON_MODULE(boost_wrapper){


			
	{
	scope outer = class_<Test::Customer>("Customer", init<int,std::string,std::string>())
		.def("show_data", &Test::Customer::show_data)
		.def("get_shipping", &Test::Customer::get_shipping);

	class_<Test::Customer::Address>("Address", init<int, std::string, std::string>())
		.def("print_address", &Test::Customer::Address::print_address);
    }
    
	class_<Test::Product>("Product", init<float>())
		.def("overview_product", &Test::Product::overview);
		
	class_<Test::Book,bases<Test::Product>>("Book", init<int, std::string, float>())
		.def("overview_book", &Test::Book::overview_book);	
		
	class_<Test::Purchase<Test::Product>>("Purchase", init<Test::Product,Test::Customer*>())
		.def("get_purchase_overview", &Test::Purchase<Test::Product>::get_purchase_overview)
		.def("get_customer", &Test::Purchase<Test::Product>::get_customer);
	}	




/*
	{
	scope outer = class_<Test::Customer>("Customer", init<int,std::string,std::string>())
		.def("show_data", &Test::Customer::show_data)
		.def("get_shipping", &Test::Customer::get_shipping);

	class_<Test::Customer::Address>("Address", init<int, std::string, std::string>())
		.def("print_address", &Test::Customer::Address::print_address);
    }

	class_<Test::Product>("Product", init<float>())
		.def("overview_product", &Test::Product::overview);

	class_<Test::Book,bases<Test::Product>>("Book", init<int, std::string, float>())
		.def("overview_book", &Test::Book::overview_book);

	class_<Test::Purchase<Test::Product>>("Purchase", init<Test::Product,Test::Customer*>())
		.def("get_purchase_overview", &Test::Purchase<Test::Product>::get_purchase_overview)
		.def("get_customer", &Test::Purchase<Test::Product>::get_customer);
	}

*/