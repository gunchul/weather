#!/bin/bash
URL=willyweather.com.au/nsw
OUTPUT_DIR=data
LOG_DIR=log
LOG_FILE=${LOG_DIR}/data_get_log.txt

AREAS=("newcastle" "bondi" "wollongong")
AREA_FILES=("/hunter/newcastle-beach.html" "/sydney/bondi-beach.html" "/illawarra/wollongong-city-beach.html")
NUM_AREAS=${#AREAS[@]}

SERIES=("wind" "rainfall" "sunrisesunset" "moonphases" "tides" "swell")
NUM_SERIES=${#SERIES[@]}

cleanup()
{
	rm -rf ${OUTPUT_DIR}
	mkdir -p ${OUTPUT_DIR}
	mkdir -p ${LOG_DIR}
}

get_data()
{
	area=${AREAS[$1]}
	area_file=${AREA_FILES[$1]}
	series=${SERIES[$2]}

	#https://moonphases.willyweather.com.au/nsw/sydney/sydney.html
	cmd="wget -q -O - https://${series}.${URL}${area_file} > $OUTPUT_DIR/${area}_${series}.txt"
	wget -q -O - https://${series}.${URL}${area_file} > $OUTPUT_DIR/${area}_${series}.txt
	[ $? -eq 0 ] || echo "ERROR: "$cmd >> ${LOG_FILE}
}

cleanup

echo "start: "`date` >> $LOG_FILE
for (( i=0; i<${NUM_AREAS}; i++ ));
do
  for (( j=0; j<${NUM_SERIES}; j++ ));
  do
     get_data $i $j
  done
done
echo "end: "`date` >> $LOG_FILE
