#ifndef __TIDE_H__
#define __TIDE_H__

typedef struct{
	time_t time;
	double height;
	char description[32];
}tide_t;

int tide_get_data(char* area, tide_t** data, int* num_data);
void print_tide(tide_t* data, int num_data);
int tide_get_on_date(tide_t* data, int num_data, struct tm* ts, tide_t* result, int num_result);

#endif