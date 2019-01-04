#ifndef __WATER_H__
#define __WATER_H__

struct tm;

typedef struct{
	char description[32];
}water_t;

int water_get_data(char* area, water_t** data, int* num_data);
int water_get_on_date(water_t* data, int num_data, struct tm* ts, water_t* result);

#endif
