#!/bin/bash

# get rocksdb
git clone https://github.com/facebook/rocksdb.git rocksdb

# get duckdb
git clone https://github.com/duckdb/duckdb.git duckdb

# build both 
cd rocksdb
PORTABLE=1 make static_lib
cd ../duckdb
make

# move all of the libs created by duckdb into one folder
mkdir staticlibs
find . -name \*.a -exec cp {} staticlibs \;

# go back to main folder
cd ..

