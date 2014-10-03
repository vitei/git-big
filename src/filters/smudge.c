#include <stdio.h>

#include "smudge.h"
#include "../db.h"

enum Error filterSmudgeRun(int argc, char *argv[])
{
	enum Error error = kErrorNone;
	char hash[41];

	fread(hash, 1, sizeof(hash), stdin);
	hash[40] = '\0';

	error = dbQueryFile(hash, stdout);

	return error;
}

