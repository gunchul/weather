#include <stdio.h>
#include <string.h>

#include "parser.h"

char* parser(
	char* buff, 
	char* parsing_start_str, 
	char* parsing_end_str, 
	char* start_str, 
	char* end_str, 
	int (*cb)(char*))
{
	char* parsing_start;
	char* parsing_end;
	char* start;
	char* end;
	char* ret=NULL;
	
	parsing_start = strstr(buff, parsing_start_str);
	if ( NULL == parsing_start )
	{
		fprintf(stderr, "%s doesn't exist\n", parsing_start_str);
		return NULL;
	}

	parsing_end = strstr(parsing_start + strlen(parsing_start_str), parsing_end_str);
	if ( NULL == parsing_end )
	{
		fprintf(stderr, "%s doesn't exist\n", parsing_end_str);
		return NULL;
	}
	
	start = parsing_start;
	do
	{
		start = strstr(start, start_str);
		if ( NULL == start || start > parsing_end )
		{
			break;
		}
		end = strstr(start, end_str);
		if ( NULL == end || end > parsing_end )
		{
			break;
		}
		*(end+strlen(end_str))='\0';

		cb(start);
		
		start = end+strlen(end_str)+1;
		ret = start;
	}while ( 1 );

	return ret;
}