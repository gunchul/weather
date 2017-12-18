#!/bin/bash

YEAR=`date +%Y`
DATE=`date +%Y-%m-%d`
AREAS=("newcastle" "bondi" "wollongong")
NUM_AREAS=${#AREAS[@]}

mkdir -p output/${YEAR}

for (( i=0; i<${NUM_AREAS}; i++ ));
do
    ./weather ${AREAS[$i]} > output/${YEAR}/${DATE}_${AREAS[$i]}.txt
done
