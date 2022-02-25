#!/bin/bash

#g++ -std=c++17 -I./rocksdb/include/ pdbkv.cpp duckstore.cpp rocksstore.cpp storage.cpp -L. -lduckdb -o pdbkv -L/mnt/c/Users/ann__/Documents/rocksdb/rocksdb   -lrocksdb -lpthread -ldl -lz -lcurl -lcurlpp
g++ -std=c++17  -g -I./rocksdb/include/ pdbmain.cpp duckstore.cpp rocksstore.cpp storage.cpp asshell.cpp -L. -lduckdb -o pdbmain -L/mnt/c/Users/ann__/Documents/rocksdb/rocksdb/ -L/mnt/c/Users/ann__/Documents/rocksdb/rocksdb/ducklib   -lrocksdb -lpthread -ldl -lz -lcurl -lcurlpp

<<<<<<< HEAD
#libcurlpp-dev
=======
#libcurlpp-dev
>>>>>>> e74854c726ae7ab7a38c5b60b9108fda5d6b0205
