#include <stdio.h>

#include "clean.h"
#include "../db.h"

enum Error filterCleanRun(int argc, char *argv[])
{
	enum Error error = kErrorNone;
	char hash[40];

	error = dbInsertFile(stdin, hash);

	if(error == kErrorNone)
	{
		fwrite(hash, 1, sizeof(hash), stdout);
	}

	return error;
}

