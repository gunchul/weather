#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "swell.h"
#include "parser.h"
#include "util.h"

#define LINE_MAX (1024*1024*2)

static swell_t g_data[1024];
static int g_num_data=0;

int swell_height_data_cb(char* data)
{
	time_t time;
	double height;
	char direction_text[16];
	char* p;
	char* q;

    /* time */
	p = strchr(data, ':');
	time = strtoul(++p, &q, 10);
	
	/* height */
	p = strchr(q, ':');
	height = strtod(++p, &q);

    /* direction */
    p = strchr(q, ':');
    
    /* directionText */
    p = strchr(p+1, ':');
    p=p+2;
    q = strchr(p, '\"');
    sprintf(direction_text, "%.*s", (int)(q-p), p);
	//printf("%d, %f, %s\n", time, height, direction_text);
    
    g_data[g_num_data].time = time;
    g_data[g_num_data].height = height;
    sprintf(g_data[g_num_data].direction, "%s", direction_text);
    g_num_data++;
    
	return 0;
}

static void period_update(time_t time, double period)
{
    int i;
    for ( i = 0 ; i < g_num_data ; i++ )
    {
        if ( g_data[i].time == time )
        {
            g_data[i].period = period;          
            return;
        }
    }
}

int swell_period_data_cb(char* data)
{
	int time;
	double period;
	char* p;
	char* q;

    /* time */
	p = strchr(data, ':');
	time = strtoul(++p, &q, 10);
	
	/* period */
	p = strchr(q, ':');
	period = strtod(++p, &q);
	
	//printf("%d, %f\n", time, period);	
    period_update(time, period);
    
	return 0;
}

void print_swell(swell_t* data, int num_data)
{
	int i;
	struct tm ts;
	char buff[128];
	
	for ( i = 0 ; i < num_data; i++ )
	{
		util_get_ts(data[i].time, &ts);
		strftime(buff, sizeof(buff), "%a %F %T", &ts);
		printf("%s %fm %s %fs\n", buff, data[i].height, data[i].direction, data[i].period);
	}
}

int swell_get_data(char* area, swell_t** data, int* num_data)
{
	FILE* f;
	char buff[LINE_MAX];
	char* p;
	char path[128];
	
	sprintf(path, "data/%s_swell.txt", area);
	f = fopen(path, "r");
	if ( NULL == f )
	{
		fprintf(stderr, "%s:%d\n", __FILE__, __LINE__);
		return -1;
	}

	if ( fread(buff, 1, sizeof(buff), f) <= 0 )
	{
		fprintf(stderr, "%s:%d\n", __FILE__, __LINE__);
		return -1;
	}
	
	p = parser(buff, "\"id\":\"swell-height\"", "\"controlPoints\"", "{\"x\":", "}}",  swell_height_data_cb);
	if ( NULL == p )
	{
		fprintf(stderr, "%s:%d\n", __FILE__, __LINE__);
		return -1;
	}
	
	p = parser(p, "\"id\":\"swell-period\"", "\"controlPoints\"", "{\"x\":", "}",  swell_period_data_cb);
	if (NULL == p)
	{
		fprintf(stderr, "%s:%d\n", __FILE__, __LINE__);
	}
	
	*data=g_data;
	*num_data=g_num_data;
	
	fclose(f);

	return 0;
}

int swell_get_on_date(swell_t* data, int num_data, struct tm* ts, swell_t* result, int num_result)
{
	int i;
	int j;
	struct tm swell_ts;
	
	for ( i = 0, j = 0; i < num_data && j < num_result ; i++ )
	{
		util_get_ts(data[i].time, &swell_ts);
		if ( swell_ts.tm_year == ts->tm_year
		  && swell_ts.tm_mon == ts->tm_mon
		  && swell_ts.tm_mday == ts->tm_mday )
		{
			result[j++] = data[i];
		}
	}
	return j;
}
