#ifndef __RAIN_H__
#define __RAIN_H__

typedef struct{
	time_t time;
	char description[32];
}rain_t;

int rain_get_data(char* area, rain_t** data, int* num_data);
void print_rain(rain_t* data, int num_data);
int rain_get_dates(rain_t* data, int num_data, struct tm* ts, int num_ts);
int rain_get_on_date(rain_t* data, int num_data, struct tm* ts, rain_t* result);

#endif