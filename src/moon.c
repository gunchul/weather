#define _XOPEN_SOURCE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <time.h>

#include "moon.h"
#include "parser.h"
#include "util.h"

static moon_t g_data[1024];
static int g_num_data=0;

int moon_data_cb(char* data)
{
    struct tm tm;
	time_t time;
	char* p;
	char* q;
   
    /* time */
    p = strchr(data, '\"') + 1;
    q = strchr(p, '\"');
    *q = '\0';
    memset(&tm, 0, sizeof(tm));
    strptime(p, "%Y-%m-%d", &tm);
	time = mktime(&tm);
	time = time + 3600*11;

    /* description */
    p = strstr(q+1, "data-fill");
    p = strchr(p, '\"')+1;
    q = strchr(p, '\"');
	*q = '\0'; 

    g_data[g_num_data].time=time;
	sprintf(g_data[g_num_data].description, "%s", p);
	g_num_data++;
    
	return 0;
}

void print_moon(moon_t* data, int num_data)
{
	int i;
	struct tm ts;
	char buff[128];
	
	for ( i = 0 ; i < num_data; i++ )
	{
		util_get_ts(data[i].time, &ts);
		strftime(buff, sizeof(buff), "%a %F %T", &ts);
		printf("%s %s\n", buff, data[i].description);
	}
}

int moon_get_data(char *area, moon_t **data, int *num_data)
{
	FILE *f;
	char buff[1024*1024];
	char path[128];
	char *ret;

	sprintf(path, "data/%s_moonphases.txt", area);
	f = fopen(path, "rb");
	if (NULL == f)
	{
		fprintf(stderr, "%s:%d open(%s) failed", __FILE__, __LINE__, path);
		return -1;
	}

	if ( fread(buff, 1, sizeof(buff), f) <= 0 )
	{
		fprintf(stderr, "%s:%d fread() failed", __FILE__, __LINE__);
		return -1;
	}
    
	ret = parser(buff, "5-Day Forecast", "moonForecast", "datetime=", "</figure>",  moon_data_cb);
	if (NULL == ret)
	{
		fprintf(stderr, "%s:%d parser() failed with %s\n", __FILE__, __LINE__, path);
		return -1;
	}

	*data = g_data;
	*num_data = g_num_data;
	
	fclose(f);

	return 0;
}

int moon_get_dates(moon_t* data, int num_data, struct tm* ts, int num_ts)
{
	int i,j;
	for ( i = 0, j = 0 ; i < num_data && j < num_ts; i += 4, j++ )
	{
		util_get_ts(data[i].time, ts+j);		
	}
	
	return j;
}

int moon_get_on_date(moon_t* data, int num_data, struct tm* ts, moon_t* result)
{
	int i;
	struct tm moon_ts;
	
	for ( i = 0 ; i < num_data ; i++ )
	{
		util_get_ts(data[i].time, &moon_ts);
		if ( moon_ts.tm_year == ts->tm_year
		  && moon_ts.tm_mon == ts->tm_mon
		  && moon_ts.tm_mday == ts->tm_mday )
		{
			result[0] = data[i];
			return 0;
		}
	}
	return -1;
}