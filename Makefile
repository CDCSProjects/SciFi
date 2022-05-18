scifi: importer exporter simple

importer:
	g++ -std=c++17 -g -I./rocksdb/include/  -I./duckdb/src/include import_path.cpp \
     assetstore.cpp duckstore.cpp metadata.cpp rocksstore.cpp storage.cpp  asshell.cpp duckdb/staticlibs/*.a  -L.  \
     -o scifi_import_path -L./../duckdb/src  -L./rocksdb/ -L./duckdb/staticlibs  -lduckdb_static -lduckdb_re2 -lduckdb_fmt -lduckdb_hyperloglog -limdb -lduckdb_miniz -lduckdb_pg_query -lduckdb_sqlite3 -lsqlite3_api_wrapper_static -ltest_helpers -lduckdb_utf8proc -lrocksdb -lpthread -ldl -lz -lcurl -lcurlpp  -lduckdb_fastpforlib

exporter:
	g++ -std=c++17 -g -I./rocksdb/include/  -I./duckdb/src/include export_to_stdout.cpp \
     assetstore.cpp duckstore.cpp metadata.cpp rocksstore.cpp storage.cpp  asshell.cpp duckdb/staticlibs/*.a  -L.  \
     -o scifi_export_to_stdout -L./../duckdb/src  -L./rocksdb/ -L./duckdb/staticlibs  -lduckdb_static -lduckdb_re2 -lduckdb_fmt -lduckdb_hyperloglog -limdb -lduckdb_miniz -lduckdb_pg_query -lduckdb_sqlite3 -lsqlite3_api_wrapper_static -ltest_helpers -lduckdb_utf8proc -lrocksdb -lpthread -ldl -lz -lcurl -lcurlpp  -lduckdb_fastpforlib
     
exporterram:
	g++ -std=c++17 -g -I./rocksdb/include/  -I./duckdb/src/include export_to_ram.cpp \
     assetstore.cpp duckstore.cpp metadata.cpp rocksstore.cpp storage.cpp  asshell.cpp duckdb/staticlibs/*.a  -L.  \
     -o scifi_export_to_ram -L./../duckdb/src  -L./rocksdb/ -L./duckdb/staticlibs  -lduckdb_static -lduckdb_re2 -lduckdb_fmt -lduckdb_hyperloglog -limdb -lduckdb_miniz -lduckdb_pg_query -lduckdb_sqlite3 -lsqlite3_api_wrapper_static -ltest_helpers -lduckdb_utf8proc -lrocksdb -lpthread -ldl -lz -lcurl -lcurlpp  -lduckdb_fastpforlib

simple:
	g++ -std=c++17 -g -I./rocksdb/include/  -I./duckdb/src/include SimpleStore.cpp assetstore.cpp duckstore.cpp metadata.cpp rocksstore.cpp storage.cpp  asshell.cpp duckdb/staticlibs/*.a  -L.  -o simple -L./../duckdb/src  -L./rocksdb/ -L./duckdb/staticlibs  -lduckdb_static -lduckdb_re2 -lduckdb_fmt -lduckdb_hyperloglog -limdb -lduckdb_miniz -lduckdb_pg_query -lduckdb_sqlite3 -lsqlite3_api_wrapper_static -ltest_helpers -lduckdb_utf8proc -lrocksdb -lpthread -ldl -lz -lcurl -lcurlpp  -lduckdb_fastpforlib


test_export:
	./scifi_export_to_stdout testdb all | ffmpeg -f image2pipe -framerate 30 -i - -c:v libx264 -vf format=yuv420p -movflags +faststart out.mp4

dep:
	./downloadAndBuildDeps.sh

