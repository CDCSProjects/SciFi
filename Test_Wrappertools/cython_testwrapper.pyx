# cython: c_string_type=unicode, c_string_encoding=utf8
from libcpp.string cimport string


cdef extern from "testwrapper.h" namespace "Test":
    cdef cppclass Product:
        int product_id
        float price
        Product(float)
        void overview()

cdef class PyProduct:
    cdef Product *productptr
    def __cinit__(self, price):
        self.productptr = new Product(price)
    def __dealloc__(self):
        del self.productptr
    def overview(self):
        self.productptr.overview()

cdef extern from "testwrapper.h" namespace "Test":
    cdef cppclass Book(Product):
        int isbn
        string author
        Book(float) except +
        Book(int, string, float) except +
        void overview_book()

cdef class PyBook(PyProduct):
    cdef Book  *bookptr
    def __cinit__(self, isbn = -1, author = 'unknown', price=0.0):
        if isbn == -1 :
            self.bookptr = self.productptr = new Book(price)
        else:
            self.bookptr = self.productptr = new Book(isbn, author, price)
            #self.bookptr = new Book(isbn, author, price)
    def __dealloc__(self):
        del self.bookptr
    def overview_book(self):
        self.bookptr.overview_book()

cdef extern from "testwrapper.h" namespace "Test":
    cdef cppclass Address:
        int housenumber
        string streeet
        string city
        Address(int,string,string)
        void print_address()

cdef class PyAddress:
    cdef Address *addressptr
    def __cinit__(self, number, street,city):
        self.addressptr = new Address(number, street, city)
    def __dealloc__(self):
        del self.addressptr
    def print_address(self):
        self.addressptr.print_address()

cdef extern from "testwrapper.h" namespace " Test":
    cdef cppclass Customer:
        int customer_id
        Address shipping_address
        Customer(int,string,string)
        void show_data()
        Address get_shipping()

cdef class PyCustomer:
    cdef Customer *customerptr
    def __cinit__(self,number,street,city):
        self.customerptr = new Customer(number, street, city)
    def __dealloc__(self):
        del self.customerptr
    def show_data(self):
        self.customerptr.show_data()
    def get_shipping(self):
        self.customerptr.get_shipping()


#cdef extern from "testwrapper.h" namespace "Test":
    #cdef cppclass Purchase:
        #Product purchase_
        #int purchase_id
       # float cost
       # Customer customer
      #  Purchase(Product,Customer) except +
        #Purchase(list(Product),Customer) except +
     #   void get_purchase_overview()
        #Customer get_customer()

#cdef class PyPurchase:
  #  cdef Purchase *purchaseptr
  #  def __cinit__(self, purchase_content, customer_1):
       # self.purchaseptr = new Purchase(purchase_content, customer_1)
   # def __dealloc__(self):
      # del self.purchaseptr
   # def get_purchase_overview(self):
     #   self.purchaseptr.get_purchase_overview()
  #  cdef get_customer(self):
       # return self.purchaseptr.get_customer()
