#include <libgen.h>
#include <stdio.h>
#include <string.h>

#include "attributes.h"
#include "repo.h"

static enum Error match_index(bool *match, const char *repo_attributes_path, const char *repo_path, const char *attribute, const char *test);

enum Error attributes_get_status(enum AttributesStatus *status)
{
	static char *PATHSPEC_STRINGS[] = { "**/.gitattributes" };

	enum Error r = ERROR_NONE;
	int error = 0;
	git_status_list *status_list;
	git_status_options status_options = GIT_STATUS_OPTIONS_INIT;
	size_t num_status_entries = 0;
	int i = 0;

	status_options.pathspec.count = 1;
	status_options.pathspec.strings = PATHSPEC_STRINGS;

	error = git_status_list_new(&status_list, repo_handle, &status_options);

	if(error != 0)
	{
		r = ERROR_INTERNAL;
		goto error_git_status_list_new;
	}

	num_status_entries = git_status_list_entrycount(status_list);

	*status = num_status_entries > 0
	        ? ATTRIBUTES_STATUS_MODIFIED
	        : ATTRIBUTES_STATUS_UNMODIFIED;

	git_status_list_free(status_list);
error_git_status_list_new:

	return r;
}

enum Error attributes_match_index(bool *match, const char *repo_path, const char *attribute, const char *test)
{
	enum Error r = ERROR_NONE;
	char path[1024] = { '\0' };
	char *current_path = NULL;

	// No match by default
	*match = false;

	// Make a copy of the path that we can mutate
	strcpy(path, repo_path);

	// Match top-down
	for(current_path = dirname(path); *current_path != '.'; current_path = dirname(current_path))
	{
		char attributes_path[1024] = { '\0' };
		const char *subdir_path = NULL;

		// FIXME: unsafe for large paths
		snprintf(attributes_path, sizeof(attributes_path), "%s/.gitattributes", current_path);

		// Get the path to the file in this subdirectory (+1 for slash)
		subdir_path = &repo_path[strlen(current_path) + 1];

		r = match_index(match, attributes_path, subdir_path, attribute, test);

		if(r != ERROR_NONE)
		{
			goto error_match_index;
		}

		if(*match)
		{
			break;
		}
	}

	// Construct the root .gitattributes manually
	if(!*match)
	{
		r = match_index(match, ".gitattributes", repo_path, attribute, test);

		if(r != ERROR_NONE)
		{
			goto error_match_index;
		}
	}

error_match_index:

	return r;
}

static enum Error match_index(bool *match, const char *repo_attributes_path, const char *repo_path, const char *attribute, const char *test)
{
	enum Error r = ERROR_NONE;

	//printf("%s    %s\n", repo_attributes_path, repo_path);

	return r;
}

