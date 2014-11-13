#include <stdio.h>
#include <string.h>

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
	const char *local_ref = NULL;
	const char *local_sha1 = NULL;
	const char *remote_ref = NULL;
	const char *remote_sha1 = NULL;
	git_oid local_oid;
	git_oid remote_oid;
	enum Error bigfile_push_r = ERROR_NONE;

	if(!hook_big_push_exists())
	{
		return ERROR_NONE;
	}

	tmp_size = fread(tmp, 1, sizeof(tmp), stdin);

	if(tmp_size == sizeof(tmp))
	{
		// Oh dear...
		r = ERROR_INTERNAL;
		goto error_fix_your_buffer_sizes;
	}
	else if(tmp_size == 0)
	{
		return ERROR_NONE;
	}

	local_ref = strtok(tmp, DELIM);
	local_sha1 = strtok(NULL, DELIM);
	remote_ref = strtok(NULL, DELIM);
	remote_sha1 = strtok(NULL, DELIM);

	git_oid_fromstr(&local_oid, local_sha1);
	git_oid_fromstr(&remote_oid, remote_sha1);

	// FIXME: this shouldn't be needed...
	if(git_oid_equal(&local_oid, &remote_oid))
	{
		return ERROR_NONE;
	}

	r = repo_tree_walk_bigfiles_for_push(&remote_oid, &local_oid, bigfile_perform_push, &bigfile_push_r);

	if(r != ERROR_NONE)
	{
		goto error_repo_tree_walk_bigfiles_for_push;
	}

	r = bigfile_push_r;

error_repo_tree_walk_bigfiles_for_push:
error_fix_your_buffer_sizes:

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

