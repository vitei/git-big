
#line 1 "/Users/ben/Tools/git-big/master/src/attributes_parser.rl"
#include <string.h>

#include "attributes_parser.h"


#line 9 "/Users/ben/Tools/git-big/master/build/attributes_parser.c"
static const int attributes_parser_start = 12;
static const int attributes_parser_first_final = 12;
static const int attributes_parser_error = 0;

static const int attributes_parser_en_main = 12;


#line 8 "/Users/ben/Tools/git-big/master/src/attributes_parser.rl"


bool attributes_parser_match(const void *data, unsigned long size, const char *attribute, const char *match)
{
	int cs;
	char *p = (char *)data;
	char *pe = p + size;
	char *name_start = NULL;
	char *value_start = NULL;

	
#line 29 "/Users/ben/Tools/git-big/master/build/attributes_parser.c"
	{
	cs = attributes_parser_start;
	}

#line 34 "/Users/ben/Tools/git-big/master/build/attributes_parser.c"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
tr6:
#line 27 "/Users/ben/Tools/git-big/master/src/attributes_parser.rl"
	{
			/*
			*fpc = '\0';
			printf("TRUE FALSE  %s\n", name_start);
			*fpc = fc;
			*/
		}
	goto st12;
tr11:
#line 35 "/Users/ben/Tools/git-big/master/src/attributes_parser.rl"
	{
			char *name_end = value_start - 1;
			char tmp = *name_end;

			*name_end = '\0';
			*p = '\0';

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
			*p = (*p);
		}
	goto st12;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
#line 79 "/Users/ben/Tools/git-big/master/build/attributes_parser.c"
	switch( (*p) ) {
		case 10: goto st12;
		case 32: goto st0;
		case 35: goto st6;
	}
	goto st1;
st1:
	if ( ++p == pe )
		goto _test_eof1;
case 1:
	switch( (*p) ) {
		case 10: goto st12;
		case 32: goto st2;
	}
	goto st1;
tr7:
#line 27 "/Users/ben/Tools/git-big/master/src/attributes_parser.rl"
	{
			/*
			*fpc = '\0';
			printf("TRUE FALSE  %s\n", name_start);
			*fpc = fc;
			*/
		}
	goto st2;
tr12:
#line 35 "/Users/ben/Tools/git-big/master/src/attributes_parser.rl"
	{
			char *name_end = value_start - 1;
			char tmp = *name_end;

			*name_end = '\0';
			*p = '\0';

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
			*p = (*p);
		}
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 134 "/Users/ben/Tools/git-big/master/build/attributes_parser.c"
	switch( (*p) ) {
		case 10: goto st0;
		case 32: goto st0;
		case 61: goto st0;
	}
	goto tr3;
tr3:
#line 19 "/Users/ben/Tools/git-big/master/src/attributes_parser.rl"
	{
			name_start = p;
		}
	goto st3;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
#line 151 "/Users/ben/Tools/git-big/master/build/attributes_parser.c"
	switch( (*p) ) {
		case 10: goto tr6;
		case 32: goto tr7;
		case 61: goto st4;
	}
	goto st3;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	switch( (*p) ) {
		case 10: goto st0;
		case 32: goto st0;
	}
	goto tr9;
tr9:
#line 23 "/Users/ben/Tools/git-big/master/src/attributes_parser.rl"
	{
			value_start = p;
		}
	goto st5;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
#line 177 "/Users/ben/Tools/git-big/master/build/attributes_parser.c"
	switch( (*p) ) {
		case 10: goto tr11;
		case 32: goto tr12;
	}
	goto st5;
st0:
cs = 0;
	goto _out;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	switch( (*p) ) {
		case 10: goto st12;
		case 32: goto st7;
	}
	goto st6;
tr18:
#line 27 "/Users/ben/Tools/git-big/master/src/attributes_parser.rl"
	{
			/*
			*fpc = '\0';
			printf("TRUE FALSE  %s\n", name_start);
			*fpc = fc;
			*/
		}
	goto st7;
tr22:
#line 35 "/Users/ben/Tools/git-big/master/src/attributes_parser.rl"
	{
			char *name_end = value_start - 1;
			char tmp = *name_end;

			*name_end = '\0';
			*p = '\0';

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
			*p = (*p);
		}
	goto st7;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
#line 234 "/Users/ben/Tools/git-big/master/build/attributes_parser.c"
	switch( (*p) ) {
		case 10: goto st12;
		case 32: goto st11;
		case 61: goto st11;
	}
	goto tr15;
tr15:
#line 19 "/Users/ben/Tools/git-big/master/src/attributes_parser.rl"
	{
			name_start = p;
		}
	goto st8;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
#line 251 "/Users/ben/Tools/git-big/master/build/attributes_parser.c"
	switch( (*p) ) {
		case 10: goto tr6;
		case 32: goto tr18;
		case 61: goto st9;
	}
	goto st8;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	switch( (*p) ) {
		case 10: goto st12;
		case 32: goto st11;
	}
	goto tr20;
tr20:
#line 23 "/Users/ben/Tools/git-big/master/src/attributes_parser.rl"
	{
			value_start = p;
		}
	goto st10;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
#line 277 "/Users/ben/Tools/git-big/master/build/attributes_parser.c"
	switch( (*p) ) {
		case 10: goto tr11;
		case 32: goto tr22;
	}
	goto st10;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	if ( (*p) == 10 )
		goto st12;
	goto st11;
	}
	_test_eof12: cs = 12; goto _test_eof; 
	_test_eof1: cs = 1; goto _test_eof; 
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof10: cs = 10; goto _test_eof; 
	_test_eof11: cs = 11; goto _test_eof; 

	_test_eof: {}
	_out: {}
	}

#line 69 "/Users/ben/Tools/git-big/master/src/attributes_parser.rl"


	return false;
}

