#include <stdio.h>

#include "smudge.h"
#include "../db.h"
#include "../patterns.h"

enum Error filter_smudge_run(int argc, char *argv[])
{
	char *filename = argv[0];

	if(patterns_file_is_present_head() && pattern_match_head(filename))
	{
		enum Error error = ERROR_NONE;
		char hash[41];

		fread(hash, 1, sizeof(hash), stdin);
		hash[40] = '\0';

		error = db_file_query(hash, stdout);

		return error;
	}
	else
	{
		return ERROR_RUN_PASSTHROUGH;
	}
}

