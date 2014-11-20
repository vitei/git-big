#include "attributes.h"
#include "repo.h"

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

enum Error attributes_match_index(bool *match, const char *filename, const char *attribute, const char *test)
{
	enum Error r = ERROR_NONE;

	*match = false;

	/* FIXME: implement the matching here... */
	*match = true;

	return r;
}

