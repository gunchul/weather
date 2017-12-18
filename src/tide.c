#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "tide.h"
#include "parser.h"
#include "util.h"

static tide_t g_data[1024];
static int g_num_data=0;

int tide_data_cb(char* data)
{
	time_t time;
	double height;
	char* p;
	char* q;
   
    /* time */
	p = strchr(data, ':');
	time = strtoul(++p, &q, 10);
	
	/* height */
	p = strchr(q, ':');
	height = strtod(++p, &q);
	
	/* description */
	p = strchr(q, ':');
	p += 2;
	q = strchr(p, '\"');

	g_data[g_num_data].time=time;
	g_data[g_num_data].height=height;
	sprintf(g_data[g_num_data].description, "%.*s", (int)(q-p), p);
	g_num_data++;
    
	return 0;
}

void print_tide(tide_t* data, int num_data)
{
	int i;
	struct tm ts;
	char buff[128];
	
	for ( i = 0 ; i < num_data; i++ )
	{
		util_get_ts(data[i].time, &ts);
		strftime(buff, sizeof(buff), "%a %F %T", &ts);
		printf("%s %fm %s\n", buff, data[i].height, data[i].description);
	}
}

int tide_get_on_date(tide_t* data, int num_data, struct tm* ts, tide_t* result, int num_result)
{
	int i;
	int j;
	struct tm tide_ts;
	
	for ( i = 0, j = 0; i < num_data && j < num_result ; i++ )
	{
		util_get_ts(data[i].time, &tide_ts);
		if ( tide_ts.tm_year == ts->tm_year
		  && tide_ts.tm_mon == ts->tm_mon
		  && tide_ts.tm_mday == ts->tm_mday 
		  && strlen(data[i].description) )
		{
			result[j++] = data[i];
		}
	}
	return j;
}

int tide_get_data(char* area, tide_t** data, int* num_data)
{
	FILE* f;
	char buff[1024*1024];
	char path[128];
	char *ret;

	sprintf(path, "data/%s_tides.txt", area);
	f = fopen(path, "rb");
	if (NULL == f)
	{
		fprintf(stderr, "%s:%d\n", __FILE__, __LINE__);
		return -1;
	}

	if (fread(buff, 1, sizeof(buff), f) <= 0)
	{
		fprintf(stderr, "%s:%d\n", __FILE__, __LINE__);
		return -1;
	}
	
	ret = parser(buff, "\"id\":\"tides\"", "\"controlPoints\":", "{\"x\":", "}",  tide_data_cb);
	if (NULL == ret)
	{
		fprintf(stderr, "%s:%d parser() failed\n", __FILE__, __LINE__);
	}
	
	*data=g_data;
	*num_data=g_num_data;
	
	fclose(f);

	return 0;
}
