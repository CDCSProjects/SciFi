#!/usr/bin/env python3
#cython: language_level=3

from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext

setup(ext_modules=[Extension("scifi", 
                             ["scificython.pyx", "./../backend/src/duckstore.cpp", "./../backend/src/rocksstore.cpp", "./../framework/src/metadata.cpp", "./../framework/src/assetstore.cpp"], language="c++", include_dirs = ['./../duckdb/src/include', './../rocksdb/include/', './..'],
                    libraries = ['rocksdb', 'duckdb_static', 'duckdb_re2', 'duckdb_fmt', 'duckdb_hyperloglog' ,'imdb', 'duckdb_miniz','duckdb_pg_query', 'duckdb_sqlite3', 'sqlite3_api_wrapper_static', 'test_helpers' ,'duckdb_utf8proc', 'parquet_extension', 'duckdb_fastpforlib', 'duckdb_mbedtls', 'pthread' ,'dl', 'z','curl','curlpp', 'snappy'],
                    library_dirs = ['./../duckdb/staticlibs','./../rocksdb', '.'],
                    extra_compile_args = ['-std=c++17'])],
      cmdclass = {'build_ext': build_ext})
