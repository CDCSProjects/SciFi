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
    