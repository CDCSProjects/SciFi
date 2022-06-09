#!/bin/bash

#g++ -std=c++17 -I./rocksdb/include/ pdbkv.cpp duckstore.cpp rocksstore.cpp storage.cpp -L. -lduckdb -o pdbkv -L/mnt/c/Users/ann__/Documents/rocksdb/rocksdb   -lrocksdb -lpthread -ldl -lz -lcurl -lcurlpp
#build with dynamic duckdb lib
#g++ -std=c++17  -g -I./rocksdb/include/ pdbmain.cpp duckstore.cpp rocksstore.cpp storage.cpp asshell.cpp -L. -L./ducklib -lduckdb -o pdbmain -L/mnt/c/Users/ann__/Documents/rocksdb/rocksdb/ -L/mnt/c/Users/ann__/Documents/rocksdb/rocksdb/ducklib   -lrocksdb -lpthread -ldl -lz -lcurl -lcurlpp
#build with static duckdb lib
#g++ -std=c++17  -g -I./rocksdb/include/ -I./../duckdb-0.3.1/duckdb-0.3.1/src/include pdbmain.cpp duckstore.cpp rocksstore.cpp storage.cpp asshell.cpp -L.  -o pdbmain -L./../duckdb-0.3.1/duckdb-0.3.1/src  -L/mnt/c/Users/ann__/Documents/rocksdb/rocksdb/  -lduckdb_static -lduckdb_re2 -lfmt -lhyperloglog -limdb -lminiz -lpg_query -lsqlite3 -lsqlite3_api_wrapper_static -ltest_helpers -lutf8proc -lrocksdb -lpthread -ldl -lz -lcurl -lcurlpp

g++ -std=c++17 -DOUTPUTSHELL -g -I./ -I./rocksdb/include/ -I./../duckdb-0.3.1/duckdb-0.3.1/src/include framework/src/assetstore.cpp examples/SimpleStore.cpp backend/src/duckstore.cpp framework/src/metadata.cpp backend/src/rocksstore.cpp framework/src/storage.cpp  shell/src/asshell.cpp -L.  -o simple -L./../duckdb-0.3.1/duckdb-0.3.1/src  -L/mnt/c/Users/ann__/Documents/rocksdb/rocksdb/  -lduckdb_static -lduckdb_re2 -lfmt -lhyperloglog -limdb -lminiz -lpg_query -lsqlite3 -lsqlite3_api_wrapper_static -ltest_helpers -lutf8proc -lrocksdb -lpthread -ldl -lz -lcurl -lcurlpp

#libcurlpp-dev
