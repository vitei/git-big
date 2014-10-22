#include <stdio.h>

#include "pre_commit.h"
#include "../patterns.h"
#include "../repo.h"

static void bigfile_filter_check(const git_index_entry *entry, enum Error *error);

enum Error hooks_pre_commit_run(int argc, char *argv[])
{
	enum Error r = ERROR_NONE;

	if(patterns_file_is_present_wc() || patterns_file_is_present_head())
	{
		if(patterns_file_is_modified())
		{
			repo_tree_walk_bigfiles_all_index((RepoWalkCallbackFunction)bigfile_filter_check, &r);
			return r;
		}
		else
		{
			// Otherwise we check only added/updated files
			repo_tree_walk_bigfiles_changed_index((RepoWalkCallbackFunction)bigfile_filter_check, &r);
			return r;
		}
	}

	return r;
}

static void bigfile_filter_check(const git_index_entry *entry, enum Error *error)
{
	static bool message_displayed = false;

	if(   (   pattern_match_head(entry->path)
	       || pattern_match_wc(entry->path))
	   && !pattern_match_index(entry->path))
	{
		if(!message_displayed)
		{
			fprintf(stderr, "These files will not be correctly handled by your current .gitbig\n");
			message_displayed = true;
		}

		fprintf(stderr, "   %s\n", entry->path);

		*error = ERROR_SILENT;
	}
}

