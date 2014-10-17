#include <stdio.h>

#include "clean.h"
#include "../db.h"

enum Error filter_clean_run(int argc, char *argv[])
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

