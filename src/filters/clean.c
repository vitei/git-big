#include <stdio.h>

#include "clean.h"
#include "../db.h"
#include "../patterns.h"

enum Error filter_clean_run(int argc, char *argv[])
{
	enum Error r = ERROR_NONE;
	char *filename = argv[0];
	char id[DB_ID_SIZE + 1];

	if(!patterns_file_is_present_wc() || !pattern_match_wc(filename))
	{
		r = ERROR_RUN_PASSTHROUGH;
		goto error_passthrough;
	}

	r = db_file_insert(id, stdin);

	if(r != ERROR_NONE)
	{
		goto error_db_file_insert;
	}

	id[DB_ID_SIZE] = '\0';

	fwrite(id, 1, sizeof(id), stdout);
	fflush(stdout);

error_db_file_insert:
error_passthrough:

	return r;
}

