#ifndef __SWELL_H__
#define __SWELL_H__

typedef struct{
	time_t time;
	double height;
	double period;
	char direction[16];
}swell_t;

int swell_get_data(char* area, swell_t** data, int* num_data);
void print_swell(swell_t* data, int num_data);
int swell_get_on_date(swell_t* data, int num_data, struct tm* ts, swell_t* result, int num_result);

#endif