#include <stdbool.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#include "hooks.h"
#include "repo.h"

static const char * const PATH_PATTERN = "%shooks/%s";

static bool hooks_create(const char *name, const char *repo_dir);

enum Error hooks_init(void)
{
	enum Error r = ERROR_NONE;
	const char *repo_dir = git_repository_path(repo_handle);

	if(!hooks_create("pre-commit", repo_dir))
	{
		r = ERROR_HOOKS_INIT_COULD_NOT_CREATE_PRE_COMMIT;
		goto error_hooks_create;
	}

	if(!hooks_create("pre-push", repo_dir))
	{
		r = ERROR_HOOKS_INIT_COULD_NOT_CREATE_PRE_COMMIT;
		goto error_hooks_create;
	}

error_hooks_create:

	return r;
}

bool hook_full_path(char *full_path, const char *name)
{
	bool r = true;
	const char *repo_dir = git_repository_path(repo_handle);
	int error = 0;

	sprintf(full_path, PATH_PATTERN, repo_dir, name);

	error = access(full_path, F_OK);

	if(error != 0)
	{
		*full_path = '\0';
		r = false;
	}

	return r;
}

static bool hooks_create(const char *name, const char *repo_dir)
{
	char path[1024] = { '\0' };
	FILE *file = NULL;
	int error = 0;

	sprintf(path, PATH_PATTERN, repo_dir, name);

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

