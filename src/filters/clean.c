#include <stdio.h>

#include "clean.h"
#include "../db.h"

enum Error filterCleanRun(int argc, char *argv[])
{
	char hash[40];

	dbInsertFile(stdin, hash);
	fwrite(hash, 1, sizeof(hash), stdout);

	return kErrorNone;
}

