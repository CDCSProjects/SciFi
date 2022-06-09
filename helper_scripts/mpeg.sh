#!/bin/bash

#make importer
#make exporter
#make importerram
#make exporterram

#wget https://upload.wikimedia.org/wikipedia/commons/7/79/Big_Buck_Bunny_small.ogv

#mkdir -p testfiles/images
#ffmpeg -i Big_Buck_Bunny_small.ogv -vf fps=30 testfiles/images/out%06d.png


# ./scifi_import_path testdb_video testfiles/images/ metadata.csv
#./scifi_import_path testdb_video testfiles/images/
#./scifi_import_path galaxy galaxy_data

echo "export_to_ram, from_disc, from_ramfs, from_pipe" > times.csv

time_start=$(date +%s.%6N)
for i in $(seq 10)
do

rm *.mp4

time1=$(date +%s.%6N)
#./scifi_export_to_ram galaxy all 
time2=$(date +%s.%6N)
  

#ffmpeg -f image2 -framerate 30 -benchmark -i testfiles/images/out%06d.png -c:v libx264 -vf format=yuv420p -movflags +faststart outdisc.mp4
cat $(find ./galaxy_data/all -maxdepth 2 -name "*.png" | sort -V) | ffmpeg -f image2pipe -framerate 30 -benchmark -i - -c:v libx264 -vf format=yuv420p -movflags +faststart outdisc.mp4
time3=$(date +%s.%6N)

#ffmpeg -f image2 -framerate 30 -benchmark -i pictures/out%06d.png -c:v libx264 -vf format=yuv420p -movflags +faststart outram.mp4
cat $(find pictures -maxdepth 2 -name "*.png" | sort -V) | ffmpeg -f image2pipe -framerate 30 -benchmark -i - -c:v libx264 -vf format=yuv420p -movflags +faststart outram.mp4
time4=$(date +%s.%6N)

#./scifi_export_to_stdout testdb_video all | ffmpeg -f image2pipe -benchmark -framerate 30 -i - -c:v libx264 -vf format=yuv420p -movflags +faststart out.mp4
./scifi_export_to_stdout galaxy all | ffmpeg -f image2pipe -benchmark -framerate 30 -i - -c:v libx264 -vf format=yuv420p -movflags +faststart outpipe.mp4
time5=$(date +%s.%6N)

echo -n $(echo "$time2 - $time1" | bc) >> times.csv
echo -n ", " >> times.csv
echo -n $(echo "$time3 - $time2" | bc) >> times.csv
echo -n ", " >> times.csv
echo -n $(echo "$time4 - $time3" | bc) >> times.csv
echo -n ", " >> times.csv
echo -n $(echo "$time5 - $time4" | bc) >> times.csv
echo "" >> times.csv

done

time_end=$(date +%s.%6N)
echo $(echo "$time_end - $time_start" | bc) 
