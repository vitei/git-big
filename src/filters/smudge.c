#include <stdio.h>

#include "smudge.h"
#include "../db.h"

enum Error filter_smudge_run(int argc, char *argv[])
{
	enum Error error = ERROR_NONE;
	char hash[41];

	fread(hash, 1, sizeof(hash), stdin);
	hash[40] = '\0';

	error = db_file_query(hash, stdout);

	return error;
}

