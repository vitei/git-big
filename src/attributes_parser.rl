#include "attributes_parser.h"

%%{
	machine attributes_parser;
	write data;
}%%

bool attributes_parser_match(const void *data, unsigned long size, const char *attribute, const char *match)
{
	int cs;
	char *p = (char *)data;
	char *pe = p + size;

	%%{
		main := [.]+ ;

		write init;
		write exec;
	}%%

	return false;
}

