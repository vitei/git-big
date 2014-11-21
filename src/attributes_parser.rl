#include <string.h>

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
	char *name_start = NULL;
	char *value_start = NULL;

	%%{
		action name_start {
			name_start = fpc;
		}

		action value_start {
			value_start = fpc;
		}

		action true_false {
			/*
			*fpc = '\0';
			printf("TRUE FALSE  %s\n", name_start);
			*fpc = fc;
			*/
		}

		action set {
			char *name_end = value_start - 1;
			char tmp = *name_end;

			*name_end = '\0';
			*fpc = '\0';

			if(strcmp(attribute, name_start) == 0)
			{
				if(strcmp(match, value_start) == 0)
				{
					return true;
				}
				else
				{
					return false;
				}
			}

			*name_end = tmp;
			*fpc = fc;
		}

		fragment = [^ \n]+ ;
		name_fragment = [^ =\n]+ ;
		true_false_rule = ( '!'? name_fragment ) >name_start %true_false ;
		set_rule = ( name_fragment >name_start '=' fragment >value_start ) %set ;
		file_rule = ( fragment ( ' ' ( true_false_rule | set_rule ) )* ) ;
		comment = ( '#' [^\n]* ) ;

		main := ( ( file_rule | comment )? '\n' )* ;

		write init;
		write exec;
	}%%

	return false;
}

