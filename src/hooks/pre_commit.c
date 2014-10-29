#include <stdio.h>
#include <sys/time.h>

#include "pre_commit.h"
#include "../patterns.h"
#include "../repo.h"

struct Checks
{
	bool error_comitted;
	bool error_staged;
	enum Error r;
};

static void bigfile_filter_check_comitted(const char *path, void *ptr);
static void bigfile_filter_check_staged(const char *path, void *ptr);
static void touch_repo_file(const char *filename);

enum Error hooks_pre_commit_run(int argc, char *argv[])
{
	enum Error r = ERROR_NONE;
	bool modified = false;
	struct Checks checks = { false, false, ERROR_NONE };

	if(!patterns_file_is_present_wc() && !patterns_file_is_present_head())
	{
		return ERROR_NONE;
	}

	if(git_repository_is_empty(repo_handle))
	{
		r = repo_tree_walk_bigfiles_all_index(bigfile_filter_check_staged, &checks);

		if(r != ERROR_NONE)
		{
			goto error_repo_tree_walk_bigfiles_all_index_staged_1;
		}
	}
	else
	{
		r = patterns_file_is_modified(&modified);

		if(r != ERROR_NONE)
		{
			goto error_patterns_file_is_modified;
		}

		if(modified)
		{
			r = repo_tree_walk_bigfiles_all_index(bigfile_filter_check_comitted,
			                                      &checks);

			if(r != ERROR_NONE)
			{
				goto error_repo_tree_walk_bigfiles_all_index_comitted;
			}

			r = repo_tree_walk_bigfiles_all_index(bigfile_filter_check_staged,
			                                      &checks);

			if(r != ERROR_NONE)
			{
				goto error_repo_tree_walk_bigfiles_all_index_staged_2;
			}
		}
		else
		{
			r = repo_tree_walk_bigfiles_changed_index(bigfile_filter_check_staged,
			                                          &checks);

			if(r != ERROR_NONE)
			{
				goto error_repo_tree_walk_bigfiles_changed_index;
			}
		}
	}

	// FIXME: if we add rules to ignore files then we should check for
	// files that would be detected as git-big managed but are not.
	
	r = checks.r;

error_repo_tree_walk_bigfiles_changed_index:
error_repo_tree_walk_bigfiles_all_index_staged_1:
error_repo_tree_walk_bigfiles_all_index_staged_2:
error_repo_tree_walk_bigfiles_all_index_comitted:
error_patterns_file_is_modified:

	return r;
}

static void bigfile_filter_check_comitted(const char *path, void *ptr)
{
	struct Checks *checks = (struct Checks *)ptr;

	if(!pattern_match_head(path) || pattern_match_index(path))
	{
		return;
	}

	touch_repo_file(path);

	if(!checks->error_comitted)
	{
		fprintf(stderr, "The following existing files are handled by git-big and will break with the current .gitbig\n"
		                "Please add valid rules to .gitbig or remove any unstaged rules and re-add the files to git\n");

		checks->error_comitted = true;
	}

	fprintf(stderr, "   %s\n", path);

	checks->r = ERROR_SILENT;
}

static void bigfile_filter_check_staged(const char *path, void *ptr)
{
	struct Checks *checks = (struct Checks *)ptr;

	if(!pattern_match_wc(path) || pattern_match_index(path))
	{
		return;
	}

	touch_repo_file(path);

	if(!checks->error_staged)
	{
		fprintf(stderr, "%s"
		                "The following new files are handled by git-big and will break with the current .gitbig\n"
		                "Please add valid rules to .gitbig or remove any unstaged rules and re-add the files to git\n",
		                checks->error_comitted ? "\n" : "");

		checks->error_staged = true;
	}

	fprintf(stderr, "   %s\n", path);

	checks->r = ERROR_SILENT;
}

static void touch_repo_file(const char *filename)
{
	char path[1024] = { '\0' };
	const char *working_dir = git_repository_workdir(repo_handle);

	// FIXME: unsafe for large paths
	snprintf(path, sizeof(path), "%s%s", working_dir, filename);

	// Touch the file!
	utimes(path, NULL);
}

