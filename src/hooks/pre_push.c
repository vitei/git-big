#include <stdio.h>

#include "big_push.h"
#include "pre_push.h"
#include "../repo.h"

static void bigfile_perform_push(const char *repo_path, const char *db_hash, const char *db_path, void *payload);

enum Error hooks_pre_push_run(int argc, char *argv[])
{
	static const char * const DELIM = " ";

	enum Error r = ERROR_NONE;
	char tmp[1024] = { '\0' };
	size_t tmp_size = 0;
	char local_sha1[40 + 1] = { '\0' };
	char remote_sha1[40 + 1] = { '\0' };
	git_oid local_oid;
	git_oid remote_oid;
	enum Error bigfile_push_r = ERROR_NONE;

	if(!hook_big_push_exists())
	{
		goto skip_no_hook;
	}

	fscanf(stdin, "%*s %40s %*s %40s", local_sha1, remote_sha1);

	git_oid_fromstr(&local_oid, local_sha1);
	git_oid_fromstr(&remote_oid, remote_sha1);

	// FIXME: this shouldn't be needed...
	if(git_oid_equal(&local_oid, &remote_oid))
	{
		goto skip_git_oid_equal;
	}

	r = repo_tree_walk_bigfiles_for_push(&remote_oid, &local_oid, bigfile_perform_push, &bigfile_push_r);

	if(r != ERROR_NONE)
	{
		goto error_repo_tree_walk_bigfiles_for_push;
	}

	r = bigfile_push_r;

skip_git_oid_equal:
error_repo_tree_walk_bigfiles_for_push:
skip_no_hook:

	return r;
}

static void bigfile_perform_push(const char *repo_path, const char *db_hash, const char *db_path, void *payload)
{
	enum Error *r = (enum Error *)payload;
	enum Error hook_error = ERROR_NONE;
	char tmp[1024];

	hook_error = hook_big_push_run(db_hash, db_path);

	if(hook_error != ERROR_NONE)
	{
		*r = ERROR_HOOKS_PRE_PUSH_COULD_NOT_PUSH_FILE;
	}
}

