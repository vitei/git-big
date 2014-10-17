#include <stdio.h>
#include <unistd.h>

#include "patterns.h"
#include "repo.h"

static const char *const PATTERNS_FILE = ".gitbig";

static const char *get_path(void);

enum Error patterns_init(void)
{
	if(!patterns_file_is_present())
	{
		FILE *patterns_file = NULL;

		// We need to make a dummy patterns file
		patterns_file = fopen(get_path(), "w");

		if(patterns_file)
		{
			fprintf(patterns_file, "# Put your git-big rules here!\n"
			                       "*\n");

			fclose(patterns_file);
		}
		else
		{
			return ERROR_PATTERNS_INIT_COULD_NOT_CREATE_PATTERNS_FILE;
		}
	}

	return ERROR_NONE;
}

bool patterns_file_is_present(void)
{
	return access(get_path(), F_OK) != -1;
}

static const char *get_path(void)
{
	static char path[1024] = { '\0' };

	if(!*path)
	{
		const char *working_dir = git_repository_workdir(repo_handle);

		// FIXME: unsafe for large paths
		snprintf(path, sizeof(path), "%s%s", working_dir, PATTERNS_FILE);
	}

	return (const char *)path;
}

