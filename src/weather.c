#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>

#include "sunrisesunset.h"
#include "swell.h"
#include "wind.h"
#include "tide.h"
#include "moon.h"
#include "rain.h"
#include "water.h"
#include "util.h"

typedef struct{
	sunrisesunset_t* sunrisesunset;
	int num_sunrisesunset;
	swell_t* swell;
	int num_swell;
	wind_t* wind;
	int num_wind;
	tide_t* tide;
	int num_tide;
	moon_t *moon;
	int num_moon;
	rain_t *rain;
	int num_rain;
	water_t *water;
	int num_water;
}weather_data_t;

static int weather_get_data(char* area, weather_data_t* weather)
{
	if ( rain_get_data(area, &weather->rain, &weather->num_rain) )
	{
		return -1;
	}

	if ( water_get_data(area, &weather->water, &weather->num_water) )
	{
		return -1;
	}

	if ( moon_get_data(area, &weather->moon, &weather->num_moon) )
	{
		return -1;
	}

	if ( sunrisesunset_get_data(area, &weather->sunrisesunset, &weather->num_sunrisesunset) )
	{
		return -1;
	}

	if ( tide_get_data(area, &weather->tide, &weather->num_tide) )
	{
		return -1;
	}

	if ( swell_get_data(area, &weather->swell, &weather->num_swell) )
	{
		return -1;
	}

	if ( wind_get_data(area, &weather->wind, &weather->num_wind) )
	{
		return -1;
	}

	return 0;
}

static void result_sunrisesunset(weather_data_t* weather, struct tm* ts)
{
	int i;
	struct tm sunrisesunset_tm;
	sunrisesunset_t sunrisesunset[4];
	char buff[128];
	
	if ( sunrisesunset_get_on_date(weather->sunrisesunset, weather->num_sunrisesunset, ts, sunrisesunset) )
	{
		fprintf(stderr, "sunrisesunset_get_on_date() failed");
		return;
	}
	
	printf("sun: ");
	for ( i = 0 ; i < 4 ; i++ )
	{
		util_get_ts(sunrisesunset[i].time, &sunrisesunset_tm);
		strftime(buff, sizeof(buff), "%H:%M", &sunrisesunset_tm);
		printf("%s ", buff);
	}
	printf("\n");
}

static void result_moon(weather_data_t* weather, struct tm* ts)
{
	moon_t moon;
	
	if ( moon_get_on_date(weather->moon, weather->num_moon, ts, &moon) )
	{
		fprintf(stderr, "moon_get_on_date() failed");
		return;
	}
	
	printf("moon: ");
	printf("%s%%", moon.description);

	printf("\n");
}

static void result_rain(weather_data_t* weather, struct tm* ts)
{
	rain_t rain;
	
	if ( rain_get_on_date(weather->rain, weather->num_rain, ts, &rain) )
	{
		fprintf(stderr, "rain_get_on_date() failed");
		return;
	}
	
	printf("rain: ");
	printf("%s", rain.description);

	printf("\n");
}

static void result_water(weather_data_t* weather, struct tm* ts)
{
	water_t water;
	
	if ( water_get_on_date(weather->water, weather->num_water, ts, &water) )
	{
		fprintf(stderr, "water_get_on_date() failed");
		return;
	}
	
	printf("sea-temperature: %s\n", water.description);
}
static void result_tide(weather_data_t* weather, struct tm* ts)
{
	int i;
	struct tm tide_tm;
	tide_t tide[8];
	int num_tide;
	char buff[128];
	
	num_tide = tide_get_on_date(weather->tide, weather->num_tide, ts, tide, 8);
	if ( num_tide < 0 )
	{
		fprintf(stderr, "tide_get_on_date() failed");
		return;
	}
	
	printf("tide: ");
	for ( i = 0 ; i < num_tide ; i++ )
	{
		if ( num_tide == 5 && i == 0 )
		{
			continue;
		}
		util_get_ts(tide[i].time, &tide_tm);
		strftime(buff, sizeof(buff), "%H:%M", &tide_tm);
		printf("%s-%s ", (tide[i].description[0]=='h')?"H":"L", buff);
	}
	printf("\n");
}

static void result_swell(weather_data_t* weather, struct tm* ts)
{
	int i;
	struct tm swell_tm;
	swell_t swell[64];
	wind_t wind;
	int num_swell;
	char buff[128];
	int ret;
	
	num_swell = swell_get_on_date(weather->swell, weather->num_swell, ts, swell, 64);
	if ( num_swell < 0 )
	{
		fprintf(stderr, "swell_get_on_date() failed");
		return;
	}
	
	printf("       %15s | %-12s\n", "swell", "wind");
	for ( i = 0 ; i < num_swell ; i++ )
	{
		util_get_ts(swell[i].time, &swell_tm);
		strftime(buff, sizeof(buff), "%H:%M", &swell_tm);
		printf(" %s: %3.1fm,%4.1fs,%3s", buff, swell[i].height, swell[i].period, swell[i].direction);
		ret = wind_get_on_time(weather->wind, weather->num_wind, swell[i].time, &wind);
		if ( ret == 0 )
		{
			printf(" | %4.1fkm/h,%3s", wind.speed, wind.direction);
		}
		printf("\n");
	}
}

int main(int argc, char* argv[])
{
	int i;
	weather_data_t weather;
	struct tm ts[7];
	char buff[128];
	
    setlocale(LC_ALL, "");

	if ( argc != 2 )
	{
		fprintf(stderr, "Usage: %s <area>\n", argv[0]);
		return -1;
	}
	
	memset(&weather, 0, sizeof(weather));
	if ( weather_get_data(argv[1], &weather) )
	{
		fprintf(stderr, "weather_get_data() failed\n");
		return -1;
	}

	if ( sunrisesunset_get_dates(weather.sunrisesunset, weather.num_sunrisesunset, ts, 7) != 7 )
	{
		fprintf(stderr, "not 7days\n");
		return -1;
	}
	
	printf("\n");
	for ( i = 0 ; i < 7 ; i++ )
	{	
		strftime(buff, sizeof(buff), "%a %F", &ts[i]);
		printf("%s\n", buff);
		printf("--------------\n");
		result_sunrisesunset(&weather, &ts[i]);
		result_rain(&weather, &ts[i]);
		result_moon(&weather, &ts[i]);
        if (0 == i)
        {
		    result_water(&weather, &ts[i]);
        }
		result_tide(&weather, &ts[i]);
		result_swell(&weather, &ts[i]);

		printf("\n");
	}
	
	return 0;
}
