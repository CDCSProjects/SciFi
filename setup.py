#!/usr/bin/env python3

#from distutils.core import setup, Extension

#extension_mod = Extension("_rocksstore", ["rocksstore_wrap.cxx", "rocksstore.cpp"], 
 #                   include_dirs = ['./rocksdb/include/','./../duckdb-0.3.1/duckdb-0.3.1/src/include',],
 #                   libraries = ['duckdb_static', 'duckdb_re2', 'fmt', 'hyperloglog' ,'imdb', 'miniz','pg_query', 'sqlite3', 'sqlite3_api_wrapper_static', 'test_helpers' ,'utf8proc', 'rocksdb' ,'pthread' ,'dl', 'z','curl','curlpp'],
 #                   library_dirs = ['./../duckdb-0.3.1/duckdb-0.3.1/src','/mnt/c/Users/ann__/Documents/rocksdb/rocksdb/','.'],
 #                   extra_compile_args = ['-std=c++17'])

#setup(name = "rocksstore", ext_modules=[extension_mod])

from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext

setup(ext_modules=[Extension("duckstore_wrapper", 
                             ["duckcython.pyx", 
                              "duckstore.cpp"], language="c++", include_dirs = ['./../duckdb-0.3.1/duckdb-0.3.1/src/include'],
                    libraries = ['duckdb_static', 'duckdb_re2', 'fmt', 'hyperloglog' ,'imdb', 'miniz','pg_query', 'sqlite3', 'sqlite3_api_wrapper_static', 'test_helpers' ,'utf8proc', 'pthread' ,'dl', 'z','curl','curlpp'],
                    library_dirs = ['./../duckdb-0.3.1/duckdb-0.3.1/src','.'],
                    extra_compile_args = ['-std=c++17'])],
      cmdclass = {'build_ext': build_ext})