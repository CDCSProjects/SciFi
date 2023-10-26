import cython_wrapper
import swig_wrapper
import boost_wrapper

print("Testing Cython-Wrapper")

print('Generating class object')
try:
    customer_cython = cython_wrapper.PyCustomer(1,'Reeperbahn', 'Hamburg')
except:
    print('Generating class object failed')
else:
    print('Generation class object successful')
    customer_cython.show_data()

print('Returning datatype Address from Customer')
try:
    address_cython = customer_cython.get_shipping()
    address_cython.print_address()
except:
    print("Returning own datatype failed")
else:
    print('Returning own datatype successful')

print('Testing Inheritance')
print('initialization of Base Class')
try:
    product_cython = cython_wrapper.PyProduct(9.99)
except:
    print('initialization of base class failed')
else:
    product_cython.overview()

print('initialization of Subclass')
try:
    book_cython = cython_wrapper.PyBook(1234, 'Murakami', 19.99)
except:
    print('initialization of subclass with own constructor failed\nInitialzing with Base class constructor')
    book_cython = cython_wrapper.PyBook(19.99)

book_cython.overview_book()
book_cython.overview()

try:
    purchase_cython = cython_wrapper.Purchase(product_cython, customer_cython)
except:
    print('Handling Templates failed')



print("Testing SWIG-Wrapper")
print('Generating Customer')
customer_swig = swig_wrapper.Customer(1,'Reeperbahn', 'Hamburg')
customer_swig.show_data()
print('Returning datatype Address from Customer')
try:
    address_swig = customer_swig.get_shipping()
    address_swig.print_address()
except:
    print("Returning own datatype failed")
else:
    print('Returning own datatype successful')

print('Testing Inheritance')



print('initialization of Base Class')
try:
    product_swig = swig_wrapper.Product(9.99)
except:
    print('Initializing base class failed')
else:
    product_swig.overview()

print('Initialize Subclass')
try:
    book_swig = swig_wrapper.Book(1234, 'Murakami', 19.99)
except:
    print('initialization of subclass with own constructor failed\ninitialization with Base class constructor')
    book_swig = swig_wrapper.Book(19.99)

book_swig.overview_book()
book_swig.overview()
try:
    purchase_swig = swig_wrapper.Purchase(product_swig, customer_swig)
except:
    print('Handling Templates failed')


print("Testing Boost-Wrapper")
print('Generating Customer')
customer_boost = boost_wrapper.Customer(1,'Reeperbahn', 'Hamburg')
customer_boost.show_data()
print('Returning datatype Address from Customer')
try:
    address_boost = customer_boost.get_shipping()
    address_boost.print_address()
except:
    print("Returning own datatype failed")
else:
    print('Returning own datatype successful')

print('Testing Inheritance')
#initilize Base Class
product_boost = boost_wrapper.Product(9.99)
product_boost.overview_product()

try:
    book_boost = boost_wrapper.Book(1234, 'Murakami', 19.99)
except:
    print('initialization of subclass with own constructor failed')
    try:
        book_boost = boost_wrapper.Book(19.99)
    except:
        print('initialization of book failed')
    else:
        book_boost.overview_book()
        book_boost.overview_product()
else:
    book_boost.overview_book()
    book_boost.overview_product()
    print('initialization of subclass successful')

print('testing template')
try:
    purchase_boost = boost_wrapper.Purchase(product_boost, customer_boost)
except:
    print('Handling Templates failed')
else:
    purchase_boost.get_purchase_overview()
    customer_boost_2 = purchase_boost.get_customer()
    address_boost_2 = customer_boost_2.get_shipping()
    address_boost_2.print_address()

