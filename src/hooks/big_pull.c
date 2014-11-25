#include <stdio.h>
#include <stdlib.h>

#if defined(__APPLE__) || defined(__linux)
	#include <unistd.h>
#elif defined(_WIN32)
	#include <Shlwapi.h>
#else
	#error Unsupported platform
#endif


#include "big_pull.h"
#include "../hooks.h"

static const char *get_hook_path();

bool hook_big_pull_exists(void)
{
#if defined(__APPLE__) || defined(__linux)
	return access(get_hook_path(), F_OK) == 0;
#elif defined(_WIN32)
	return PathFileExists(get_hook_path()) == TRUE;
#endif
}

enum Error hook_big_pull_run(const char *db_hash, const char *db_path)
{
	enum Error r = ERROR_NONE;
	int error = 0;
	char tmp[1024];

	// FIXME: unsafe for large paths
	snprintf(tmp, sizeof(tmp), "%s %s %s", get_hook_path(), db_hash, db_path);

	error = system(tmp);

	if(error != 0)
	{
		r = ERROR_INTERNAL;
	}

	return r;
}

static const char *get_hook_path()
{
	static const char * const HOOK_NAME = "big-pull";
	static char hook_path[1024] = { '\0' }; // XXX

	if(!*hook_path)
	{
		hook_full_path(hook_path, HOOK_NAME);
	}

	return hook_path;
}

