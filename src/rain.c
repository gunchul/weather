#define _XOPEN_SOURCE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <time.h>

#include "rain.h"
#include "parser.h"
#include "util.h"

static rain_t g_data[1024];
static int g_num_data=0;

#define DASH ("&ndash;")

static void remove_dash(char *dest, char *src)
{
	char *p;

	strcpy(dest, src);
	p = strstr(src, DASH);
	if (p)
	{
		strcpy(dest + (unsigned int)(p - src), "-");
		strcat(dest, p + strlen(DASH));
	}
}

int rain_data_cb(char* data)
{
    struct tm tm;
	time_t time;
	char* p;
	char* q;
	char chance[128];
	char amount[128];

    /* time */
    p = strchr(data, '\"') + 1;
    q = strchr(p, '\"');
    *q = '\0';
    memset(&tm, 0, sizeof(tm));
    strptime(p, "%Y-%m-%d", &tm);
	time = mktime(&tm);
	time = time + 3600*11;

    /* chance */
    p = strstr(q+1, "chance-value");
    if (NULL == p)
    {
        strcpy(chance, "0%");
        strcpy(amount, "0mm");
    }
    else
    {
        p = strchr(p, '>')+1;
        q = strchr(p, '<');
        *q = '\0'; 
        sprintf(chance, "%s", p);

        p = strstr(q+1, "chance-amount");
        p = strchr(p, '>')+1;
        q = strchr(p, '<');
        *q = '\0'; 
        remove_dash(amount, p);
    }
	sprintf(g_data[g_num_data].description, "%s, %s", chance, amount);
	g_data[g_num_data].time=time;

	g_num_data++;
    
	return 0;
}

void print_rain(rain_t* data, int num_data)
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

int rain_get_data(char *area, rain_t **data, int *num_data)
{
	FILE *f;
	char buff[1024*1024];
	char path[128];
	char *ret;

	sprintf(path, "data/%s_rainfall.txt", area);
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
    
	ret = parser(buff, "Rainfall Forecast", "rainfallForecast", "datetime=", "</div>",  rain_data_cb);
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

int rain_get_dates(rain_t* data, int num_data, struct tm* ts, int num_ts)
{
	int i,j;
	for ( i = 0, j = 0 ; i < num_data && j < num_ts; i += 4, j++ )
	{
		util_get_ts(data[i].time, ts+j);		
	}
	
	return j;
}

int rain_get_on_date(rain_t* data, int num_data, struct tm* ts, rain_t* result)
{
	int i;
	struct tm rain_ts;
	
	for ( i = 0 ; i < num_data ; i++ )
	{
		util_get_ts(data[i].time, &rain_ts);
		if ( rain_ts.tm_year == ts->tm_year
		  && rain_ts.tm_mon == ts->tm_mon
		  && rain_ts.tm_mday == ts->tm_mday )
		{
			result[0] = data[i];
			return 0;
		}
	}
	return -1;
}
