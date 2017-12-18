#ifndef __PARSER_H__
#define __PARSER_H__

char* parser(
	char* buff, 
	char* parsing_start_str, 
	char* parsing_end_str, 
	char* start_str, 
	char* end_str, 
	int (*cb)(char*));

#endif
