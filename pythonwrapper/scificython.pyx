from cython.operator cimport dereference as deref
from libcpp.memory cimport unique_ptr
from libcpp.string cimport string
from libcpp.vector cimport vector
from libcpp cimport bool

cdef extern from "../backend/include/duckstore.h" namespace "SciStore":
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
        string getResultAsString()
        string idcolumn
        bool get_isNumeric(string)
        bool get_isText(string)
        string crop_single_result(string)
        string crop_result(string)
        vector[string] get_user_meta_column_names()
        vector[string] crop_and_split_result(string)
 #       void initDB(string)
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
    def getResultAsString(self):
        return self.thisptr.getResultAsString()
    def get_isNumeric(self, col_id):
        return self.thisptr.get_isNumeric(col_id)
    def get_isText(self, col_id):
        return self.thisptr.get_isText(col_id)    
    def crop_single_result(self, res):
        return self.thisptr.crop_single_result(res)
    def crop_result(self, res):
        return self.thisptr.crop_result(res)   
    def get_user_meta_column_names(self):
        return self.thisptr.get_user_meta_column_names() 
    def crop_and_split_result(self, string):
        return self.thisptr.crop_and_split_result(string)
  #  def initDB(self, name):
 #       return self.thisptr.initDB(name)
    
cdef extern from "../backend/include/rocksstore.h" namespace "SciStore":
    cdef cppclass RocksStore:
        RocksStore(string)
        void open()
        void insert(string, string)
        void create(string, int, int, int)
        string getSingle(string)
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

##cdef extern from "storage.h" namespace "SciStore":
    ##cdef cppclass DefaultStorage:
        ##DefaultStorage (string, string)
        ##void import_asset_store(string)
        ##void testout()
##    
##cdef class PyDefStorage:
    ##cdef DefaultStorage *thisptr
    ##def __cinit__(self, s1, s2):
        ##self.thisptr = new DefaultStorage(s1, s2)
    ##def __dealloc__(self):
        ##del self.thisptr
    ##def import_asset_store(self, file):
        ##return self.import_asset_store(file)
    ##def testout(self):
        ##return self.testout()
##        
##                
##cdef extern from "storage.h" namespace "SciStore":
    ##cdef cppclass Storage[T, U]:
        ##Storage(string, string) except +
        ##void import_asset_store(string)
        ##void load_asset_from_file(string, string)
        ##void testout()
        ##T *assetstore
        ##U *metastore
##                
##cdef class PyDefaultStorage:
    ##cdef Storage[RocksStore, DuckStore] *thisptr
    ##def __cinit__(self, s1, s2):
        ##self.thisptr = new Storage[RocksStore, DuckStore](s1, s2)
    ##def __dealloc__(self):
        ##del self.thisptr
    ##def import_asset_store(self, file):
        ##return self.import_asset_store(file)
    ##def load_asset_from_file(self, file, dir = ""):
        ##return self.load_asset_from_file(file, dir)
    ##def testout(self):
        ##return self.testout()
