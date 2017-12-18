#ifndef __WIND_H__
#define __WIND_H__

typedef struct{
	time_t time;
	double speed;
	char direction[32];
}wind_t;

int wind_get_data(char* area, wind_t** data, int* num_data);
void print_wind(wind_t* data, int num_data);
int wind_get_on_time(wind_t* data, int num_data, time_t time, wind_t* result);

#endif