#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "sunrisesunset.h"
#include "parser.h"
#include "util.h"

static sunrisesunset_t g_data[1024];
static int g_num_data=0;

int sunrisesunset_data_cb(char* data)
{
	time_t time;
	char* p;
	char* q;
   
    /* time */
	p = strchr(data, ':');
	time = strtoul(++p, &q, 10);
	
	/* description */
	p = strchr(q, ':');
	p += 2;
	q = strchr(p, '\"');

	g_data[g_num_data].time=time;
	sprintf(g_data[g_num_data].description, "%.*s", (int)(q-p), p);
	g_num_data++;
    
	return 0;
}

void print_sunrisesunset(sunrisesunset_t* data, int num_data)
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

int sunrisesunset_get_data(char *area, sunrisesunset_t **data, int *num_data)
{
	FILE *f;
	char buff[1024*1024];
	char path[128];
	char *ret;

	sprintf(path, "data/%s_sunrisesunset.txt", area);
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
	
	ret = parser(buff, "\"id\":\"sunrisesunset\"", "\"id\":", "{\"x\":", "}",  sunrisesunset_data_cb);
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

int sunrisesunset_get_dates(sunrisesunset_t* data, int num_data, struct tm* ts, int num_ts)
{
	int i,j;
	for ( i = 0, j = 0 ; i < num_data && j < num_ts; i += 4, j++ )
	{
		util_get_ts(data[i].time, ts+j);		
	}
	
	return j;
}

int sunrisesunset_get_on_date(sunrisesunset_t* data, int num_data, struct tm* ts, sunrisesunset_t* result)
{
	int i;
	struct tm sunrisesunset_ts;
	
	for ( i = 0 ; i < num_data ; i++ )
	{
		util_get_ts(data[i].time, &sunrisesunset_ts);
		if ( sunrisesunset_ts.tm_year == ts->tm_year
		  && sunrisesunset_ts.tm_mon == ts->tm_mon
		  && sunrisesunset_ts.tm_mday == ts->tm_mday )
		{
			result[0] = data[i];
			result[1] = data[i+1];
			result[2] = data[i+2];
			result[3] = data[i+3];
			return 0;
		}
	}
	return -1;
}
