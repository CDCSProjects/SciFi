#!/bin/bash

#build example from source:
echo "Build simple example. Use source code directly."
g++ -std=c++17 -DOUTPUTSHELL -g -I./ -I./rocksdb/include/ -I./duckdb/src/include framework/src/assetstore.cpp examples/SimpleStore.cpp backend/src/duckstore.cpp framework/src/metadata.cpp backend/src/rocksstore.cpp framework/src/storage.cpp  shell/src/asshell.cpp -L.  -o simple -L./duckdb/staticlibs  -L./rocksdb/  -lduckdb_static -lduckdb_re2 -lduckdb_fmt -lduckdb_hyperloglog -limdb -lduckdb_miniz -lduckdb_pg_query -lduckdb_sqlite3 -lsqlite3_api_wrapper_static -ltest_helpers -lduckdb_utf8proc -lparquet_extension -lduckdb_fastpforlib -lduckdb_mbedtls -lduckdb_fsst -ljemalloc_extension -lrocksdb -lpthread -ldl -lz -lcurl -lcurlpp -lsnappy

#build scifi lib:
echo "Build libscifi.so library."
g++ -fPIC -shared -std=c++17 -I./ -I./rocksdb/include/ -I./duckdb/src/include framework/src/assetstore.cpp backend/src/duckstore.cpp framework/src/metadata.cpp backend/src/rocksstore.cpp framework/src/storage.cpp   -L.  -o libscifi.so -L./duckdb/staticlibs  -L./rocksdb/  -lduckdb_static -lduckdb_re2 -lduckdb_fmt -lduckdb_hyperloglog -limdb -lduckdb_miniz -lduckdb_pg_query -lduckdb_sqlite3 -lsqlite3_api_wrapper_static -ltest_helpers -lduckdb_utf8proc -lparquet_extension -lduckdb_fastpforlib -lduckdb_mbedtls -lduckdb_fsst -ljemalloc_extension -lrocksdb -lpthread -ldl -lz -lcurl -lcurlpp -lsnappy

#requires libsnappy-dev, libcurlpp-dev, build-essential, rocksdb , and static duckdb libs (rocksdb and duckdb should be built using the provided script downloadAndBuild.sh)
