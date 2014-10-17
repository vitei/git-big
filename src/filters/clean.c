#include <stdio.h>

#include "clean.h"
#include "../db.h"
#include "../patterns.h"

enum Error filter_clean_run(int argc, char *argv[])
{
	if(patterns_file_is_present())
	{
		enum Error error = ERROR_NONE;
		char hash[41];

		error = db_file_insert(stdin, hash);
		hash[40] = '\0';

		if(error == ERROR_NONE)
		{
			fwrite(hash, 1, sizeof(hash), stdout);
		}

		return error;
	}
	else
	{
		return ERROR_RUN_PASSTHROUGH;
	}
}

