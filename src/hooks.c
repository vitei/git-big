#include <stdbool.h>
#include <stdio.h>
#include <sys/stat.h>

#include "hooks.h"
#include "repo.h"

static bool hooks_create(const char *name, const char *repo_dir);

enum Error hooks_init(void)
{
	const char *repo_dir = git_repository_path(repo_handle);

	if(!hooks_create("pre-commit", repo_dir))
	{
		return ERROR_HOOKS_INIT_COULD_NOT_CREATE_PRE_COMMIT;
	}

	return ERROR_NONE;
}

static bool hooks_create(const char *name, const char *repo_dir)
{
	char path[1024] = { '\0' };
	FILE *file = NULL;
	int error = 0;

	sprintf(path, "%shooks/%s", repo_dir, name);

	file = fopen(path, "a+");

	if(!file)
	{
		return false;
	}

	fprintf(file, "#!/bin/sh\n"
	              "\n"
	              "exec git big hook-%s\n", name);

	fclose(file);

	// FIXME: this is UNIX only
	error = chmod(path, 0777); 

	return true;
}

