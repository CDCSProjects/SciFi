from cython.operator cimport dereference as deref
from libcpp.memory cimport unique_ptr
from libcpp.string cimport string
from libcpp.vector cimport vector

cdef extern from "duckstore.h" namespace "SciStore":
    cdef cppclass DuckStore:
        DuckStore(string)
        int createNewDB()
        int loadDB(string)
        void execQuery(string)
        void execQueryAndPrint(string)
        void getSingle(string)
        void getSingleToFile(string, string )
        void writeResultToFile(string, string)
        void printResult()
        vector[string] getIDsByConstraint(string)
        string idcolumn
#        DuckDB * db
#        Connection * conn
#        unique_ptr[MaterializedQueryResult] current_result

cdef class PyDuckStore:
    cdef DuckStore *thisptr
    def __cinit__(self, name):
        self.thisptr = new DuckStore(name)
    def __dealloc__(self):
        del self.thisptr
    def execQueryAndPrint(self, query):
        return self.thisptr.execQueryAndPrint(query)
    def createNewDB(self):
        return self.thisptr.createNewDB()
    def loadDB(self, name):
        return self.thisptr.loadDB(name)
    def execQuery(self, query):
        return self.thisptr.execQuery(query)
    def getSingle(self, id):
        return self.thisptr.getSingle(id)
    def getSingleToFile(self, id, ext):
        return self.thisptr.getSingleToFile(id, ext)
    def writeResultToFile(self, filename, ext):
        return self.thisptr.writeResultToFile(filename, ext)
    def printResult(self):
        return self.thisptr.printResult()
    def getIDsByConstraint(self, constr):
        return self.thisptr.getIDsByConstraint(constr)
    
cdef extern from "rocksstore.h" namespace "SciStore":
    cdef cppclass RocksStore:
        RocksStore(string)
        void open()
        void insert(string, string)
        void create(string, int, int, int)
        void getSingle(string)
        void createportable(string, int, int, int)
        void insertFromFile(string, string)
        void getSingleToFile(string, string)
        void importsst(string)
        void remove(string)
        
cdef class PyRocksStore:
    cdef RocksStore *thisptr
    def __cinit__(self, name):
        self.thisptr = new RocksStore(name)
    def __dealloc__(self):
        del self.thisptr
    def open(self):
        return self.thisptr.open()
    def insert(self, key, id):
        return self.thisptr.insert(key,id)
    def create(self, name, i1, i2, i3):
        return self.thisptr.create(name, i1, i2, i3)
    def getSingle(self, id):
        return self.thisptr.getSingle(id)
    def createportable(self, name, i1, i2, i3):
        return self.thisptr.create(name, i1, i2, i3)
    def insertFromFile(self, file, path):
        return self.thisptr.insertFromFile(file, path)
    def getSingleToFile(self, id, ext):
        return self.thisptr.getSingleToFile(id, ext)
    def importsst(self, name):
        return self.thisptr.importsst(name)
    def remove(self, id):
        return self.thisptr.remove(id)