
#line 1 "/Users/ben/Tools/git-big/master/src/attributes_parser.rl"
#include <stdio.h>
#include <string.h>

#include "attributes.h"
#include "attributes_parser.h"


#line 11 "/Users/ben/Tools/git-big/master/build/attributes_parser.c"
static const int attributes_parser_start = 3;
static const int attributes_parser_first_final = 3;
static const int attributes_parser_error = 0;

static const int attributes_parser_en_main = 3;


#line 10 "/Users/ben/Tools/git-big/master/src/attributes_parser.rl"


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

	
#line 36 "/Users/ben/Tools/git-big/master/build/attributes_parser.c"
	{
	cs = attributes_parser_start;
	}

#line 41 "/Users/ben/Tools/git-big/master/build/attributes_parser.c"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
tr9:
#line 30 "/Users/ben/Tools/git-big/master/src/attributes_parser.rl"
	{
			glob_end = p;
		}
	goto st3;
tr12:
#line 38 "/Users/ben/Tools/git-big/master/src/attributes_parser.rl"
	{
			name_end = p;
		}
#line 50 "/Users/ben/Tools/git-big/master/src/attributes_parser.rl"
	{
			/*
			*fpc = '\0';
			printf("TRUE FALSE  %s\n", name_start);
			*fpc = fc;
			*/
		}
	goto st3;
tr16:
#line 46 "/Users/ben/Tools/git-big/master/src/attributes_parser.rl"
	{
			value_end = p;
		}
#line 58 "/Users/ben/Tools/git-big/master/src/attributes_parser.rl"
	{
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
	goto st3;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
#line 113 "/Users/ben/Tools/git-big/master/build/attributes_parser.c"
	switch( (*p) ) {
		case 9: goto st0;
		case 10: goto st3;
		case 32: goto st0;
		case 35: goto tr6;
	}
	goto tr4;
tr4:
#line 26 "/Users/ben/Tools/git-big/master/src/attributes_parser.rl"
	{
			glob_start = p;
		}
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 131 "/Users/ben/Tools/git-big/master/build/attributes_parser.c"
	switch( (*p) ) {
		case 9: goto tr8;
		case 10: goto tr9;
		case 32: goto tr8;
	}
	goto st4;
tr8:
#line 30 "/Users/ben/Tools/git-big/master/src/attributes_parser.rl"
	{
			glob_end = p;
		}
	goto st1;
tr11:
#line 38 "/Users/ben/Tools/git-big/master/src/attributes_parser.rl"
	{
			name_end = p;
		}
#line 50 "/Users/ben/Tools/git-big/master/src/attributes_parser.rl"
	{
			/*
			*fpc = '\0';
			printf("TRUE FALSE  %s\n", name_start);
			*fpc = fc;
			*/
		}
	goto st1;
tr15:
#line 46 "/Users/ben/Tools/git-big/master/src/attributes_parser.rl"
	{
			value_end = p;
		}
#line 58 "/Users/ben/Tools/git-big/master/src/attributes_parser.rl"
	{
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
	goto st1;
st1:
	if ( ++p == pe )
		goto _test_eof1;
case 1:
#line 204 "/Users/ben/Tools/git-big/master/build/attributes_parser.c"
	switch( (*p) ) {
		case 9: goto st1;
		case 10: goto st0;
		case 32: goto st1;
		case 61: goto st0;
	}
	goto tr0;
tr0:
#line 34 "/Users/ben/Tools/git-big/master/src/attributes_parser.rl"
	{
			name_start = p;
		}
	goto st5;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
#line 222 "/Users/ben/Tools/git-big/master/build/attributes_parser.c"
	switch( (*p) ) {
		case 9: goto tr11;
		case 10: goto tr12;
		case 32: goto tr11;
		case 61: goto tr13;
	}
	goto st5;
tr13:
#line 38 "/Users/ben/Tools/git-big/master/src/attributes_parser.rl"
	{
			name_end = p;
		}
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 240 "/Users/ben/Tools/git-big/master/build/attributes_parser.c"
	if ( (*p) == 32 )
		goto st0;
	if ( 9 <= (*p) && (*p) <= 10 )
		goto st0;
	goto tr3;
tr3:
#line 42 "/Users/ben/Tools/git-big/master/src/attributes_parser.rl"
	{
			value_start = p;
		}
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
#line 256 "/Users/ben/Tools/git-big/master/build/attributes_parser.c"
	switch( (*p) ) {
		case 9: goto tr15;
		case 10: goto tr16;
		case 32: goto tr15;
	}
	goto st6;
st0:
cs = 0;
	goto _out;
tr6:
#line 26 "/Users/ben/Tools/git-big/master/src/attributes_parser.rl"
	{
			glob_start = p;
		}
	goto st7;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
#line 276 "/Users/ben/Tools/git-big/master/build/attributes_parser.c"
	switch( (*p) ) {
		case 9: goto tr18;
		case 10: goto tr9;
		case 32: goto tr18;
	}
	goto st7;
tr18:
#line 30 "/Users/ben/Tools/git-big/master/src/attributes_parser.rl"
	{
			glob_end = p;
		}
	goto st8;
tr23:
#line 38 "/Users/ben/Tools/git-big/master/src/attributes_parser.rl"
	{
			name_end = p;
		}
#line 50 "/Users/ben/Tools/git-big/master/src/attributes_parser.rl"
	{
			/*
			*fpc = '\0';
			printf("TRUE FALSE  %s\n", name_start);
			*fpc = fc;
			*/
		}
	goto st8;
tr27:
#line 46 "/Users/ben/Tools/git-big/master/src/attributes_parser.rl"
	{
			value_end = p;
		}
#line 58 "/Users/ben/Tools/git-big/master/src/attributes_parser.rl"
	{
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
	goto st8;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
#line 349 "/Users/ben/Tools/git-big/master/build/attributes_parser.c"
	switch( (*p) ) {
		case 9: goto st8;
		case 10: goto st3;
		case 32: goto st8;
		case 61: goto st12;
	}
	goto tr19;
tr19:
#line 34 "/Users/ben/Tools/git-big/master/src/attributes_parser.rl"
	{
			name_start = p;
		}
	goto st9;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
#line 367 "/Users/ben/Tools/git-big/master/build/attributes_parser.c"
	switch( (*p) ) {
		case 9: goto tr23;
		case 10: goto tr12;
		case 32: goto tr23;
		case 61: goto tr24;
	}
	goto st9;
tr24:
#line 38 "/Users/ben/Tools/git-big/master/src/attributes_parser.rl"
	{
			name_end = p;
		}
	goto st10;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
#line 385 "/Users/ben/Tools/git-big/master/build/attributes_parser.c"
	switch( (*p) ) {
		case 9: goto st12;
		case 10: goto st3;
		case 32: goto st12;
	}
	goto tr25;
tr25:
#line 42 "/Users/ben/Tools/git-big/master/src/attributes_parser.rl"
	{
			value_start = p;
		}
	goto st11;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
#line 402 "/Users/ben/Tools/git-big/master/build/attributes_parser.c"
	switch( (*p) ) {
		case 9: goto tr27;
		case 10: goto tr16;
		case 32: goto tr27;
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
	case 4: 
	case 7: 
#line 30 "/Users/ben/Tools/git-big/master/src/attributes_parser.rl"
	{
			glob_end = p;
		}
	break;
	case 5: 
	case 9: 
#line 38 "/Users/ben/Tools/git-big/master/src/attributes_parser.rl"
	{
			name_end = p;
		}
#line 50 "/Users/ben/Tools/git-big/master/src/attributes_parser.rl"
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
#line 46 "/Users/ben/Tools/git-big/master/src/attributes_parser.rl"
	{
			value_end = p;
		}
#line 58 "/Users/ben/Tools/git-big/master/src/attributes_parser.rl"
	{
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
	break;
#line 499 "/Users/ben/Tools/git-big/master/build/attributes_parser.c"
	}
	}

	_out: {}
	}

#line 109 "/Users/ben/Tools/git-big/master/src/attributes_parser.rl"


	return false;
}

