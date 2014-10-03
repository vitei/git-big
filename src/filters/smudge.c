#include <stdio.h>

#include "smudge.h"
#include "../db.h"

enum Error filterSmudgeRun(int argc, char *argv[])
{
	enum Error error = kErrorNone;
	char hash[40];

	fread(hash, 1, sizeof(hash), stdin);
	error = dbQueryFile(hash, stdout);

	return error;
}

