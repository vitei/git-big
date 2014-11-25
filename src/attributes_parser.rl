#include <stdio.h>
#include <string.h>

#include "attributes.h"
#include "attributes_parser.h"

%%{
	machine attributes_parser;
	write data;
}%%

bool attributes_parser_match(const void *data, unsigned long size, const char *repo_path, const char *attribute, const char *match)
{
	int cs;
	char *p = (char *)data;
	char *pe = p + size;
	char *eof = pe;
	char *glob_start = NULL;
	char *glob_end = NULL;
	char *name_start = NULL;
	char *name_end = NULL;
	char *value_start = NULL;
	char *value_end = NULL;

	%%{
		action glob_start {
			glob_start = fpc;
		}

		action glob_end {
			glob_end = fpc;
		}

		action name_start {
			name_start = fpc;
		}

		action name_end {
			name_end = fpc;
		}

		action value_start {
			value_start = fpc;
		}

		action value_end {
			value_end = fpc;
		}

		action true_false {
			/*
			*fpc = '\0';
			printf("TRUE FALSE  %s\n", name_start);
			*fpc = fc;
			*/
		}

		action set {
			bool do_return = false;
			bool r = false;
			char glob_end_tmp = *glob_end;
			char name_end_tmp = *name_end;
			char value_end_tmp = *value_end;

			*glob_end = '\0';
			*name_end = '\0';
			*value_end = '\0';

			if(   attributes_fnmatch(repo_path, glob_start)
			   && strcmp(attribute, name_start) == 0)
			{
				do_return = true;

				if(strcmp(match, value_start) == 0)
				{
					r = true;
				}
				else
				{
					r = false;
				}
			}

			*glob_end = glob_end_tmp;
			*name_end = name_end_tmp;
			*value_end = value_end_tmp;

			if(do_return)
			{
				return r;
			}
		}

		ws = [ \t]+ ;
		fragment = [^ \t\n]+ ;
		name_fragment = [^ =\t\n]+ ;
		true_false_rule = ( '!'? name_fragment ) >name_start %name_end %true_false ;
		set_rule = ( name_fragment >name_start %name_end '=' fragment >value_start %value_end ) %set ;
		file_rule = ( fragment >glob_start %glob_end ( ws ( true_false_rule | set_rule ) )* ) ;
		attribute_rule = ( '[' fragment ']' fragment ( ws ( true_false_rule | set_rule ) )* ) ;
		comment = ( '#' [^\n]* ) ;

		rule = file_rule | attribute_rule | comment ;

		main := ( rule? '\n' )* rule? ;

		write init;
		write exec;
	}%%

	return false;
}

