#!/bin/bash
URL="https://www.seatemperature.org/australia-pacific/australia/"
OUTPUT_DIR=data
AREAS=("newcastle" "sydney" "wollongong")
OUTPUT_AREAS=("newcastle" "bondi" "wollongong")
NUM_AREAS=${#AREAS[@]}
LOG_DIR=log
LOG_FILE=${LOG_DIR}/data_get_log.txt

get_data()
{
   url_file=${AREAS[$1]}.htm
   output_file=${OUTPUT_AREAS[$1]}_water.txt
   #https://www.seatemperature.org/australia-pacific/australia/newcastle.htm
   cmd="wget -q -O - ${URL}${url_file} | grep -A2 'id=\"sea-temperature\"' | grep -v 'div' | sed -e 's/[ \t]*//' | tr '&' ' ' | awk -F '/' '{print \$1}'> $OUTPUT_DIR/${output_file}"
   #echo $cmd
   wget -q -O - ${URL}${url_file} | grep -A2 'id=\"sea-temperature\"' | grep -v 'div' | sed -e 's/[ \t]*//' | tr '&' ' ' | awk -F '/' '{print $1}'> $OUTPUT_DIR/${output_file}
   [ $? -eq 0 ] || echo "ERROR: "$cmd >> ${LOG_FILE}
}

for (( i=0; i<${NUM_AREAS}; i++ ));
do
    get_data $i
done

