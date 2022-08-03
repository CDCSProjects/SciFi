#!/bin/bash

g++ -std=c++17 -DOUTPUTSHELL -g -I./ -I./rocksdb/include/ -I./duckdb/src/include framework/src/assetstore.cpp examples/SimpleStore.cpp backend/src/duckstore.cpp framework/src/metadata.cpp backend/src/rocksstore.cpp framework/src/storage.cpp  shell/src/asshell.cpp -L.  -o simple -L./duckdb/staticlibs  -L./rocksdb/  -lduckdb_static -lduckdb_re2 -lduckdb_fmt -lduckdb_hyperloglog -limdb -lduckdb_miniz -lduckdb_pg_query -lduckdb_sqlite3 -lsqlite3_api_wrapper_static -ltest_helpers -lduckdb_utf8proc -lparquet_extension -lduckdb_fastpforlib -lduckdb_mbedtls -lrocksdb -lpthread -ldl -lz -lcurl -lcurlpp -lsnappy

#requires libcurlpp-dev, build-essential, rocksdb , and static duckdb libs (rocksdb and duckdb should be built using the provided script downloadAndBuild.sh)
