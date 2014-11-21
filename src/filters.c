#include <stdio.h>

#include "filters.h"
#include "repo.h"

const char * const CONFIG_FILENAME = ".git/config";

enum Error filters_init(void)
{
	enum Error r = ERROR_NONE;
	int error = 0;
	const char *filter_clean = NULL;
	const char *filter_smudge = NULL;

	error = git_config_get_string(&filter_clean, repo_config_handle, "filter.big.clean");

	if(error != 0 && error != GIT_ENOTFOUND)
	{
		r = ERROR_INTERNAL;
		goto error_git_config_get_string;
	}

	error = git_config_get_string(&filter_smudge, repo_config_handle, "filter.big.smudge");

	if(error != 0 && error != GIT_ENOTFOUND)
	{
		r = ERROR_INTERNAL;
		goto error_git_config_get_string;
	}

	if(filter_clean == NULL && filter_smudge == NULL)
	{
		char path[1024] = { '\0' };
		const char *working_dir = git_repository_workdir(repo_handle);
		FILE *config_file;

		// FIXME: unsafe for large paths
		snprintf(path, sizeof(path), "%s%s", working_dir, CONFIG_FILENAME);

		config_file = fopen(path, "a");

		if(config_file == NULL)
		{
			r = ERROR_FILTERS_INIT_COULD_NOT_OPEN_CONFIG;
			goto error_fopen;
		}

		fprintf(config_file, "[filter \"big\"]\n"
		                     "\tclean = git-big filter-clean %%f\n"
		                     "\tsmudge = git-big filter-smudge %%f\n"
		                     "\trequired\n");

		fclose(config_file);

error_fopen:
		;
	}

error_git_config_get_string:

	return r;
}

