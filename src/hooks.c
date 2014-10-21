#include <stdio.h>
#include <sys/stat.h>

#include "hooks.h"
#include "repo.h"

static enum Error hooks_create_pre_commit(const char *repo_dir);

enum Error hooks_init(void)
{
	const char *repo_dir = git_repository_path(repo_handle);
	enum Error error = ERROR_NONE;
	
	error = hooks_create_pre_commit(repo_dir);

	return error;
}

static enum Error hooks_create_pre_commit(const char *repo_dir)
{
	char path[1024] = { '\0' };
	FILE *file = NULL;
	int error = 0;

	sprintf(path, "%shooks/pre-commit", repo_dir);

	file = fopen(path, "a+");

	if(!file)
	{
		return ERROR_HOOKS_INIT_COULD_NOT_CREATE_PRE_COMMIT;
	}

	fprintf(file, "#!/bin/sh\n"
	              "\n"
	              "exec git big hook-pre-commit\n");

	fclose(file);

	// FIXME: this is UNIX only
	error = chmod(path, 0777); 

	return ERROR_NONE;
}

