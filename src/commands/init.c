#include "init.h"
#include "../db.h"
#include "../filters.h"
#include "../hooks.h"
#include "../repo.h"

enum Error command_init_run(int argc, char *argv[])
{
	enum Error r = ERROR_NONE;
	int error;

	error = git_repository_config(&repo_config_handle, repo_handle);

	if(error != 0)
	{
		r = ERROR_INIT_CORRUPT;
		goto error_repository_config;
	}

	error = db_init();

	if(error != ERROR_NONE)
	{
		goto error_db_init;
	}

	error = filters_init();

	if(error != ERROR_NONE)
	{
		goto error_filters_init;
	}

	error = hooks_init();

	if(error != ERROR_NONE)
	{
		goto error_hooks_init;
	}

error_hooks_init:
error_filters_init:
error_db_init:
	git_config_free(repo_config_handle);
error_repository_config:

	return r;
}

