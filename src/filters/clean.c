#include <stdio.h>

#include "clean.h"
#include "../db.h"
#include "../patterns.h"

enum Error filter_clean_run(int argc, char *argv[])
{
	char *filename = argv[0];

	if(patterns_file_is_present_wc() && pattern_match_wc(filename))
	{
		enum Error error = ERROR_NONE;
		char id[DB_ID_SIZE + 1];

		error = db_file_insert(id, stdin);
		id[DB_ID_SIZE] = '\0';

		if(error == ERROR_NONE)
		{
			fwrite(id, 1, sizeof(id), stdout);
		}

		return error;
	}
	else
	{
		return ERROR_RUN_PASSTHROUGH;
	}
}

