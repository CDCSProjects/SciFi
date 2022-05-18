#!/bin/bash

echo "unzip" > times_write.csv
for i in $(seq 10)
do
	rm -r testpictures/images
	time1=$(date +%s.%6N)
	unzip -d testpictures images.zip
	time2=$(date +%s.%6N)
	echo -n $(echo "$time2 - $time1" | bc) >> times_write.csv
	echo -n ", " >> times_write.csv
done

echo "" >> times_write.csv
echo "write_to_ram" >> times_write.csv
for i in $(seq 10)
do
	rm -r pictures
	time1=$(date +%s.%6N)
	./scifi_export_to_ram testdb_video all 
	time2=$(date +%s.%6N)
	echo -n $(echo "$time2 - $time1" | bc) >> times_write.csv
	echo -n ", " >> times_write.csv
done

echo "" >> times_write.csv
echo "write_to_disc" >> times_write.csv
for i in $(seq 10)
do
	rm -r picturesdisc
	time1=$(date +%s.%6N)
	./scifi_export_to_disc testdb_video all
	time2=$(date +%s.%6N)
	echo -n $(echo "$time2 - $time1" | bc) >> times_write.csv
	echo -n ", " >> times_write.csv
done

