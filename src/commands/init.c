#include "init.h"
#include "../db.h"
#include "../filters.h"
#include "../patterns.h"
#include "../repo.h"

enum Error initRun(int argc, char *argv[])
{
	int error;

	error = git_repository_config(&gRepoConfigHandle, gRepoHandle);

	if(error == 0)
	{
		enum Error error = kErrorNone;

		error = dbInit();

		if(error == kErrorNone)
		{
			error = filtersInit();
		}

		if(error == kErrorNone)
		{
			error = patternsInit();
		}

		git_config_free(gRepoConfigHandle);
		gRepoConfigHandle = NULL;

		return error;
	}
	else
	{
		return kErrorInitCorrupt;
	}
}

