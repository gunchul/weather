#ifndef __MOON_H__
#define __MOON_H__

typedef struct{
	time_t time;
	char description[32];
}moon_t;

int moon_get_data(char* area, moon_t** data, int* num_data);
void print_moon(moon_t* data, int num_data);
int moon_get_dates(moon_t* data, int num_data, struct tm* ts, int num_ts);
int moon_get_on_date(moon_t* data, int num_data, struct tm* ts, moon_t* result);

#endif