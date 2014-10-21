#include "init.h"
#include "../db.h"
#include "../filters.h"
#include "../hooks.h"
#include "../patterns.h"
#include "../repo.h"

enum Error command_init_run(int argc, char *argv[])
{
	int error;

	error = git_repository_config(&repo_config_handle, repo_handle);

	if(error == 0)
	{
		enum Error error = ERROR_NONE;

		error = db_init();

		if(error == ERROR_NONE)
		{
			error = filters_init();
		}

		if(error == ERROR_NONE)
		{
			error = hooks_init();
		}

		if(error == ERROR_NONE)
		{
			error = patterns_init();
		}

		git_config_free(repo_config_handle);
		repo_config_handle = NULL;

		return error;
	}
	else
	{
		return ERROR_INIT_CORRUPT;
	}
}

