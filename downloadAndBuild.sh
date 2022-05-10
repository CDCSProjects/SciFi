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

#TODO make paths relative
g++ -std=c++17 -g -I./rocksdb/include/ -I./duckdb/src/include assetstore.cpp SimpleStore.cpp duckstore.cpp metadata.cpp rocksstore.cpp storage.cpp  asshell.cpp -L.  -o simple -L./../duckdb/src  -L./rocksdb/ -L./duckdb/staticlibs  -lduckdb_static -lduckdb_re2 -lfmt -lhyperloglog -limdb -lminiz -lpg_query -lsqlite3 -lsqlite3_api_wrapper_static -ltest_helpers -lutf8proc -lrocksdb -lpthread -ldl -lz -lcurl -lcurlpp
