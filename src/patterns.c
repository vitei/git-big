#include <stdio.h>
#include <unistd.h>

#include "patterns.h"
#include "repo.h"

static const char *kPatternsFile = ".gitbig";

static const char *getPath(void);

enum Error patternsInit(void)
{
	return kErrorNone;
}

bool patternsIsFilePresent(void)
{
	return access(getPath(), F_OK) != -1;
}

static const char *getPath(void)
{
	static char path[1024] = { '\0' };

	if(!*path)
	{
		const char *workingDir = git_repository_workdir(gRepoHandle);

		// FIXME: unsafe for large paths
		snprintf(path, sizeof(path), "%s%s", workingDir, kPatternsFile);
	}

	return (const char *)path;
}

