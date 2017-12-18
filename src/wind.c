#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "wind.h"
#include "parser.h"
#include "util.h"

#define LINE_MAX (1024*1024*2)

static wind_t g_data[1024];
static int g_num_data=0;

int wind_data_cb(char* data)
{
	int time;
	double speed;
	char direction_text[16];
	char* p;
	char* q;
    
    /* time */
	p = strchr(data, ':');
	time = strtoul(++p, &q, 10);
	
	/* speed */
	p = strchr(q, ':');
	speed = strtod(++p, &q);

    /* direction */
    p = strchr(q, ':');
    
    /* directionText */
    p = strchr(p+1, ':');
    p=p+2;
    q = strchr(p, '\"');
    sprintf(direction_text, "%.*s", (int)(q-p), p);
	//printf("%d, %f, %s\n", time, speed, direction_text);
    
	g_data[g_num_data].time=time;
	g_data[g_num_data].speed=speed;
	sprintf(g_data[g_num_data].direction, "%.*s", (int)(q-p), p);
	g_num_data++;
    
	return 0;
}

void print_wind(wind_t* data, int num_data)
{
	int i;
	struct tm ts;
	char buff[128];
	
	for ( i = 0 ; i < num_data; i++ )
	{
		util_get_ts(data[i].time, &ts);
		strftime(buff, sizeof(buff), "%a %F %T", &ts);
		printf("%s %fkm/h %s\n", buff, data[i].speed, data[i].direction);
	}
}

int wind_get_data(char* area, wind_t** data, int* num_data)
{
	FILE* f;
	char buff[LINE_MAX];
	char path[128];
	char *ret;

	sprintf(path, "data/%s_wind.txt", area);
	f = fopen(path, "r");
	if ( NULL == f )
	{
		return -1;
	}

	if ( fread(buff, 1, sizeof(buff), f) <= 0 )
	{
		return -1;
	}
	
	ret = parser(buff, "\"id\":\"wind\"", "\"id\":", "{\"x\":", "}}",  wind_data_cb);
	if (NULL == ret)
	{
		fprintf(stderr, "%s:%d\n", __FILE__, __LINE__);
	}
	
	*data=g_data;
	*num_data=g_num_data;
	
	fclose(f);

	return 0;
}

int wind_get_on_time(wind_t* data, int num_data, time_t time, wind_t* result)
{
	int i;
	double diff;
	double prev_diff=3600.0*24.0*7.0;
	
	for ( i = 0; i < num_data ; i++ )
	{
		diff = difftime(data[i].time, time);
		if ( diff == 0.0 )
		{
			*result = data[i];
			return 0;
		}		
		else if ( diff < 0.0 )
		{
			prev_diff = diff;
			continue;
		}
		else
		{
			if ( i == 0 || fabs(prev_diff) > fabs(diff) )
			{
				*result = data[i];
			}
			else
			{
				*result = data[i-1];
			}
			return 0;
		}
	}
	return -1;
}
