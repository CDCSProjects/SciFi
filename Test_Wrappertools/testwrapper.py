# This file was automatically generated by SWIG (http://www.swig.org).
# Version 4.0.2
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.

from sys import version_info as _swig_python_version_info
if _swig_python_version_info < (2, 7, 0):
    raise RuntimeError("Python 2.7 or later required")

# Import the low-level C/C++ module
if __package__ or "." in __name__:
    from . import _testwrapper
else:
    import _testwrapper

try:
    import builtins as __builtin__
except ImportError:
    import __builtin__

def _swig_repr(self):
    try:
        strthis = "proxy of " + self.this.__repr__()
    except __builtin__.Exception:
        strthis = ""
    return "<%s.%s; %s >" % (self.__class__.__module__, self.__class__.__name__, strthis,)


def _swig_setattr_nondynamic_instance_variable(set):
    def set_instance_attr(self, name, value):
        if name == "thisown":
            self.this.own(value)
        elif name == "this":
            set(self, name, value)
        elif hasattr(self, name) and isinstance(getattr(type(self), name), property):
            set(self, name, value)
        else:
            raise AttributeError("You cannot add instance attributes to %s" % self)
    return set_instance_attr


def _swig_setattr_nondynamic_class_variable(set):
    def set_class_attr(cls, name, value):
        if hasattr(cls, name) and not isinstance(getattr(cls, name), property):
            set(cls, name, value)
        else:
            raise AttributeError("You cannot add class attributes to %s" % cls)
    return set_class_attr


def _swig_add_metaclass(metaclass):
    """Class decorator for adding a metaclass to a SWIG wrapped class - a slimmed down version of six.add_metaclass"""
    def wrapper(cls):
        return metaclass(cls.__name__, cls.__bases__, cls.__dict__.copy())
    return wrapper


class _SwigNonDynamicMeta(type):
    """Meta class to enforce nondynamic attributes (no new attributes) for a class"""
    __setattr__ = _swig_setattr_nondynamic_class_variable(type.__setattr__)


class Address(object):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc="The membership flag")
    __repr__ = _swig_repr
    housenumber = property(_testwrapper.Address_housenumber_get, _testwrapper.Address_housenumber_set)
    street = property(_testwrapper.Address_street_get, _testwrapper.Address_street_set)
    city = property(_testwrapper.Address_city_get, _testwrapper.Address_city_set)

    def __init__(self, *args):
        _testwrapper.Address_swiginit(self, _testwrapper.new_Address(*args))

    def print_address(self):
        return _testwrapper.Address_print_address(self)
    __swig_destroy__ = _testwrapper.delete_Address

# Register Address in _testwrapper:
_testwrapper.Address_swigregister(Address)

class Customer(object):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc="The membership flag")
    __repr__ = _swig_repr
    customer_id = property(_testwrapper.Customer_customer_id_get, _testwrapper.Customer_customer_id_set)
    shipping_address = property(_testwrapper.Customer_shipping_address_get, _testwrapper.Customer_shipping_address_set)

    def __init__(self, *args):
        _testwrapper.Customer_swiginit(self, _testwrapper.new_Customer(*args))

    def show_data(self):
        return _testwrapper.Customer_show_data(self)
    __swig_destroy__ = _testwrapper.delete_Customer

# Register Customer in _testwrapper:
_testwrapper.Customer_swigregister(Customer)

class Product(object):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc="The membership flag")
    __repr__ = _swig_repr
    product_id = property(_testwrapper.Product_product_id_get, _testwrapper.Product_product_id_set)
    price = property(_testwrapper.Product_price_get, _testwrapper.Product_price_set)

    def __init__(self, p=0.0):
        _testwrapper.Product_swiginit(self, _testwrapper.new_Product(p))

    def overview(self):
        return _testwrapper.Product_overview(self)
    __swig_destroy__ = _testwrapper.delete_Product

# Register Product in _testwrapper:
_testwrapper.Product_swigregister(Product)

class Book(Product):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc="The membership flag")
    __repr__ = _swig_repr
    isbn = property(_testwrapper.Book_isbn_get, _testwrapper.Book_isbn_set)
    author = property(_testwrapper.Book_author_get, _testwrapper.Book_author_set)

    def __init__(self, *args):
        _testwrapper.Book_swiginit(self, _testwrapper.new_Book(*args))

    def overview_book(self):
        return _testwrapper.Book_overview_book(self)
    __swig_destroy__ = _testwrapper.delete_Book

# Register Book in _testwrapper:
_testwrapper.Book_swigregister(Book)



