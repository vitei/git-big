#include <libgen.h>
#include <stdio.h>
#include <string.h>

#if defined(__APPLE__) || defined(__linux)
	#include <fnmatch.h>
#elif defined(_WIN32)
	#include <Shlwapi.h>
#else
	#error Unsupported platform
#endif

#include "attributes.h"
#include "attributes_parser.h"
#include "repo.h"

static enum Error match_index(bool *is_match, git_index *idx, const char *repo_attributes_path, const char *repo_path, const char *attribute, const char *match);

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

enum Error attributes_match_index(bool *is_match, const char *repo_path, const char *attribute, const char *match)
{
	enum Error r = ERROR_NONE;
	int error = 0;
	git_index *idx = NULL;
	char path[1024] = { '\0' };
	char *current_path = NULL;

	// No match by default
	*is_match = false;

	error = git_repository_index(&idx, repo_handle);

	if(error != 0)
	{
		goto error_git_repository_index;
	}

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

		r = match_index(is_match, idx, attributes_path, subdir_path, attribute, match);

		if(r != ERROR_NONE)
		{
			goto error_match_index;
		}

		if(*is_match)
		{
			break;
		}
	}

	// Construct the root .gitattributes manually
	if(!*is_match)
	{
		r = match_index(is_match, idx, ".gitattributes", repo_path, attribute, match);

		if(r != ERROR_NONE)
		{
			goto error_match_index;
		}
	}

error_match_index:
	git_index_free(idx);
error_git_repository_index:

	return r;
}

bool attributes_fnmatch(const char *filename, const char *filter)
{
#if defined(__APPLE__) || defined(__linux)
	return fnmatch(filter, filename, 0) == 0;
#elif defined(_WIN32)
	return PathMatchSpec(filename, filter) == TRUE;
#endif
}

static enum Error match_index(bool *is_match, git_index *idx, const char *repo_attributes_path, const char *repo_path, const char *attribute, const char *match)
{
	enum Error r = ERROR_NONE;
	int error = 0;
	const git_index_entry *entry = NULL;
	git_blob *blob = NULL;
	const void *data = NULL;
	git_off_t data_size = 0;

	entry = git_index_get_bypath(idx, repo_attributes_path, 0);

	if(entry == NULL)
	{
		goto skip_no_attributes;
	}

	error = git_blob_lookup(&blob, repo_handle, &entry->id);

	if(error != 0)
	{
		r = ERROR_INTERNAL;
		goto error_git_blob_lookup;
	}

	data = git_blob_rawcontent(blob);
	data_size = git_blob_rawsize(blob);

	*is_match = attributes_parser_match(data, data_size, repo_path, attribute, match);

	git_blob_free(blob);
error_git_blob_lookup:
skip_no_attributes:

	return r;
}

