#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>

#include "init.h"
#include "../repo.h"

static const char *kDBDirectory = "big";

static const char *getPath(void)
{
	static char path[1024] = { '\0' };

	if(!*path)
	{
		const char *dotGitDir = git_repository_path(gRepoHandle);

		// FIXME: unsafe for large paths
		snprintf(path, sizeof(path), "%s%s/", dotGitDir, kDBDirectory);
	}

	return (const char *)path;
}

int dbInit(void)
{
	int error;

	error = mkdir(getPath(), 777);

	if(error == 0 || errno == EEXIST)
	{
		return kErrorNone;
	}
	else
	{
		return kErrorDBInitCouldNotCreateDirectory;
	}
}

