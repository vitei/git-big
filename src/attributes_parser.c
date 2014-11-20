
#line 1 "/Users/ben/Tools/git-big/master/src/attributes_parser.rl"
#include "attributes_parser.h"


#line 7 "/Users/ben/Tools/git-big/master/build/attributes_parser.c"
static const int attributes_parser_start = 1;
static const int attributes_parser_first_final = 2;
static const int attributes_parser_error = 0;

static const int attributes_parser_en_main = 1;


#line 6 "/Users/ben/Tools/git-big/master/src/attributes_parser.rl"


bool attributes_parser_match(const void *data, unsigned long size, const char *attribute, const char *match)
{
	int cs;
	char *p = (char *)data;
	char *pe = p + size;

	
#line 25 "/Users/ben/Tools/git-big/master/build/attributes_parser.c"
	{
	cs = attributes_parser_start;
	}

#line 30 "/Users/ben/Tools/git-big/master/build/attributes_parser.c"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	if ( (*p) == 46 )
		goto st2;
	goto st0;
st0:
cs = 0;
	goto _out;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	if ( (*p) == 46 )
		goto st2;
	goto st0;
	}
	_test_eof2: cs = 2; goto _test_eof; 

	_test_eof: {}
	_out: {}
	}

#line 19 "/Users/ben/Tools/git-big/master/src/attributes_parser.rl"


	return false;
}

