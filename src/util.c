#include <time.h>

#include "util.h"

void util_get_ts(time_t time, struct tm* ts)
{
	*ts = *gmtime(&time);
}