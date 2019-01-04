#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "water.h"

static char temperature[128];

int water_get_data(char *area, water_t **data, int *num_data)
{
	FILE *f;
	char buff[128];
	char path[128];
    double d;

	sprintf(path, "data/%s_water.txt", area);
	f = fopen(path, "rb");
	if (NULL == f)
	{
	    sprintf(buff, "N.A");
        return 0;
	}

    memset(buff, 0, sizeof(buff));
	if ( fread(buff, 1, sizeof(buff), f) <= 0 )
	{
	    sprintf(buff, "N.A");
        fclose(f);
        return 0;
	}
    
    d = strtod(buff, NULL);
	sprintf(temperature, "%1.f", d);
    strcat(temperature, "°C"); 
	fclose(f);
	return 0;
}

int water_get_on_date(water_t* data, int num_data, struct tm* ts, water_t* result)
{
    strcpy(result->description, temperature);
    return 0;
}
