#!/bin/bash

OUTPUT_DIR=data
LOG_DIR=log
LOG_FILE=${LOG_DIR}/data_get_log.txt

cleanup()
{
	rm -rf ${OUTPUT_DIR}
	mkdir -p ${OUTPUT_DIR}
	mkdir -p ${LOG_DIR}
}

cleanup

bash ./weather_willy.sh
bash ./weather_water.sh
