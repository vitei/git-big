#include <stdio.h>

#include "pre_commit.h"
#include "../patterns.h"
#include "../repo.h"

struct Checks
{
	bool error_comitted;
	bool error_staged;
	enum Error r;
};

static void bigfile_filter_check_comitted(const git_index_entry *entry, struct Checks *checks);
static void bigfile_filter_check_staged(const git_index_entry *entry, struct Checks *checks);

enum Error hooks_pre_commit_run(int argc, char *argv[])
{
	if(patterns_file_is_present_wc() || patterns_file_is_present_head())
	{
		struct Checks checks = { false, false, ERROR_NONE };

		if(patterns_file_is_modified())
		{
			repo_tree_walk_bigfiles_all_index((RepoWalkCallbackFunction)bigfile_filter_check_comitted, &checks);
			repo_tree_walk_bigfiles_all_index((RepoWalkCallbackFunction)bigfile_filter_check_staged, &checks);
		}
		else
		{
			repo_tree_walk_bigfiles_changed_index((RepoWalkCallbackFunction)bigfile_filter_check_staged, &checks);
		}

		return checks.r;
	}
	else
	{
		return ERROR_NONE;
	}
}

static void bigfile_filter_check_comitted(const git_index_entry *entry, struct Checks *checks)
{
	if(pattern_match_head(entry->path) && !pattern_match_index(entry->path))
	{
		if(!checks->error_comitted)
		{
			fprintf(stderr, "The following existing files are handled by git-big and will break with the current .gitbig\n"
			                "Please add valid rules to .gitbig or remove any unstaged rules and re-add the files to git\n");

			checks->error_comitted = true;
		}

		fprintf(stderr, "   %s\n", entry->path);

		checks->r = ERROR_SILENT;
	}
}

static void bigfile_filter_check_staged(const git_index_entry *entry, struct Checks *checks)
{
	if(pattern_match_wc(entry->path) && !pattern_match_index(entry->path))
	{
		if(!checks->error_staged)
		{
			fprintf(stderr, "%s"
			                "The following new files are handled by git-big and will break with the current .gitbig\n"
			                "Please add valid rules to .gitbig or remove any unstaged rules and re-add the files to git\n",
			                checks->error_comitted ? "\n" : "");

			checks->error_staged = true;
		}

		fprintf(stderr, "   %s\n", entry->path);

		checks->r = ERROR_SILENT;
	}
}

