
import testwrapper

book = testwrapper.Book()

customer = testwrapper.Customer()
customer.show_data()
shipping = testwrapper.Address()
shipping.print_address()
shipping.street = ['m']
shipping.city = 'mcapi'
shipping.housenumber = 9
shipping.print_address()
print(customer.customer_id)
