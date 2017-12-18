#ifndef __SUNRISESUNSET_H__
#define __SUNRISESUNSET_H__

typedef struct{
	time_t time;
	char description[32];
}sunrisesunset_t;

int sunrisesunset_get_data(char* area, sunrisesunset_t** data, int* num_data);
void print_sunrisesunset(sunrisesunset_t* data, int num_data);
int sunrisesunset_get_dates(sunrisesunset_t* data, int num_data, struct tm* ts, int num_ts);
int sunrisesunset_get_on_date(sunrisesunset_t* data, int num_data, struct tm* ts, sunrisesunset_t* result);

#endif