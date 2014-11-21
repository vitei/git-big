
#line 1 "/Users/ben/Tools/git-big/master/src/attributes_parser.rl"
#include <stdio.h>
#include <string.h>

#include "attributes_parser.h"


#line 10 "/Users/ben/Tools/git-big/master/build/attributes_parser.c"
static const int attributes_parser_start = 3;
static const int attributes_parser_first_final = 3;
static const int attributes_parser_error = 0;

static const int attributes_parser_en_main = 3;


#line 9 "/Users/ben/Tools/git-big/master/src/attributes_parser.rl"


bool attributes_parser_match(const void *data, unsigned long size, const char *attribute, const char *match)
{
	int cs;
	char *p = (char *)data;
	char *pe = p + size;
	char *eof = pe;
	char *name_start = NULL;
	char *value_start = NULL;

	
#line 31 "/Users/ben/Tools/git-big/master/build/attributes_parser.c"
	{
	cs = attributes_parser_start;
	}

#line 36 "/Users/ben/Tools/git-big/master/build/attributes_parser.c"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
tr9:
#line 29 "/Users/ben/Tools/git-big/master/src/attributes_parser.rl"
	{
			/*
			*fpc = '\0';
			printf("TRUE FALSE  %s\n", name_start);
			*fpc = fc;
			*/
		}
	goto st3;
tr13:
#line 37 "/Users/ben/Tools/git-big/master/src/attributes_parser.rl"
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
	goto st3;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
#line 81 "/Users/ben/Tools/git-big/master/build/attributes_parser.c"
	switch( (*p) ) {
		case 9: goto st0;
		case 10: goto st3;
		case 32: goto st0;
		case 35: goto st7;
	}
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	switch( (*p) ) {
		case 9: goto st1;
		case 10: goto st3;
		case 32: goto st1;
	}
	goto st4;
tr8:
#line 29 "/Users/ben/Tools/git-big/master/src/attributes_parser.rl"
	{
			/*
			*fpc = '\0';
			printf("TRUE FALSE  %s\n", name_start);
			*fpc = fc;
			*/
		}
	goto st1;
tr12:
#line 37 "/Users/ben/Tools/git-big/master/src/attributes_parser.rl"
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
	goto st1;
st1:
	if ( ++p == pe )
		goto _test_eof1;
case 1:
#line 138 "/Users/ben/Tools/git-big/master/build/attributes_parser.c"
	switch( (*p) ) {
		case 32: goto st0;
		case 61: goto st0;
	}
	if ( 9 <= (*p) && (*p) <= 10 )
		goto st0;
	goto tr0;
tr0:
#line 21 "/Users/ben/Tools/git-big/master/src/attributes_parser.rl"
	{
			name_start = p;
		}
	goto st5;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
#line 156 "/Users/ben/Tools/git-big/master/build/attributes_parser.c"
	switch( (*p) ) {
		case 9: goto tr8;
		case 10: goto tr9;
		case 32: goto tr8;
		case 61: goto st2;
	}
	goto st5;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	if ( (*p) == 32 )
		goto st0;
	if ( 9 <= (*p) && (*p) <= 10 )
		goto st0;
	goto tr2;
tr2:
#line 25 "/Users/ben/Tools/git-big/master/src/attributes_parser.rl"
	{
			value_start = p;
		}
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
#line 183 "/Users/ben/Tools/git-big/master/build/attributes_parser.c"
	switch( (*p) ) {
		case 9: goto tr12;
		case 10: goto tr13;
		case 32: goto tr12;
	}
	goto st6;
st0:
cs = 0;
	goto _out;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	switch( (*p) ) {
		case 9: goto st8;
		case 10: goto st3;
		case 32: goto st8;
	}
	goto st7;
tr18:
#line 29 "/Users/ben/Tools/git-big/master/src/attributes_parser.rl"
	{
			/*
			*fpc = '\0';
			printf("TRUE FALSE  %s\n", name_start);
			*fpc = fc;
			*/
		}
	goto st8;
tr22:
#line 37 "/Users/ben/Tools/git-big/master/src/attributes_parser.rl"
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
	goto st8;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
#line 242 "/Users/ben/Tools/git-big/master/build/attributes_parser.c"
	switch( (*p) ) {
		case 9: goto st12;
		case 10: goto st3;
		case 32: goto st12;
		case 61: goto st12;
	}
	goto tr15;
tr15:
#line 21 "/Users/ben/Tools/git-big/master/src/attributes_parser.rl"
	{
			name_start = p;
		}
	goto st9;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
#line 260 "/Users/ben/Tools/git-big/master/build/attributes_parser.c"
	switch( (*p) ) {
		case 9: goto tr18;
		case 10: goto tr9;
		case 32: goto tr18;
		case 61: goto st10;
	}
	goto st9;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	switch( (*p) ) {
		case 9: goto st12;
		case 10: goto st3;
		case 32: goto st12;
	}
	goto tr20;
tr20:
#line 25 "/Users/ben/Tools/git-big/master/src/attributes_parser.rl"
	{
			value_start = p;
		}
	goto st11;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
#line 288 "/Users/ben/Tools/git-big/master/build/attributes_parser.c"
	switch( (*p) ) {
		case 9: goto tr22;
		case 10: goto tr13;
		case 32: goto tr22;
	}
	goto st11;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	if ( (*p) == 10 )
		goto st3;
	goto st12;
	}
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof1: cs = 1; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof10: cs = 10; goto _test_eof; 
	_test_eof11: cs = 11; goto _test_eof; 
	_test_eof12: cs = 12; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 5: 
	case 9: 
#line 29 "/Users/ben/Tools/git-big/master/src/attributes_parser.rl"
	{
			/*
			*fpc = '\0';
			printf("TRUE FALSE  %s\n", name_start);
			*fpc = fc;
			*/
		}
	break;
	case 6: 
	case 11: 
#line 37 "/Users/ben/Tools/git-big/master/src/attributes_parser.rl"
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
	break;
#line 357 "/Users/ben/Tools/git-big/master/build/attributes_parser.c"
	}
	}

	_out: {}
	}

#line 74 "/Users/ben/Tools/git-big/master/src/attributes_parser.rl"


	return false;
}

