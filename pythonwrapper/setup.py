#!/usr/bin/env python3
#cython: language_level=3

from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext

setup(ext_modules=[Extension("scifi", 
                             ["scificython.pyx", "./../backend/src/duckstore.cpp", "./../backend/src/rocksstore.cpp", "./../framework/src/metadata.cpp", "./../framework/src/assetstore.cpp"], language="c++", include_dirs = ['./../../duckdb-0.3.1/duckdb-0.3.1/src/include', './../rocksdb/include/', './..'],
                    libraries = ['rocksdb', 'duckdb_static', 'duckdb_re2', 'fmt', 'hyperloglog' ,'imdb', 'miniz','pg_query', 'sqlite3', 'sqlite3_api_wrapper_static', 'test_helpers' ,'utf8proc', 'pthread' ,'dl', 'z','curl','curlpp'],
                    library_dirs = ['./../../duckdb-0.3.1/duckdb-0.3.1/src','/mnt/c/Users/ann__/Documents/rocksdb/rocksdb/', '.'],
                    extra_compile_args = ['-std=c++17'])],
      cmdclass = {'build_ext': build_ext})
      
      #, "./../framework/src/storage.cpp", "./../framework/src/metadata.cpp"
      #, './..framework/include', './../backend/include'
