#include <stdio.h>

#include "smudge.h"
#include "../db.h"

enum Error filterSmudgeRun(int argc, char *argv[])
{
	char hash[40];

	fread(hash, 1, sizeof(hash), stdin);
	dbQueryFile(hash, stdout);

	return kErrorNone;
}

