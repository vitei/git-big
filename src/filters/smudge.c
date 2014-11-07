#include <stdio.h>
#include <stdlib.h>

#include "smudge.h"
#include "../db.h"
#include "../hooks.h"
#include "../patterns.h"

static const char * const HOOK_NAME = "big-pull";

enum Error filter_smudge_run(int argc, char *argv[])
{
	enum Error r = ERROR_NONE;
	char *filename = argv[0];
	char id[DB_ID_SIZE] = { '\0' };
	size_t id_size = 0;

	if(!patterns_file_is_present_head() || !pattern_match_head(filename))
	{
		r = ERROR_RUN_PASSTHROUGH;
		goto error_passthrough;
	}

	id_size = fread(id, 1, sizeof(id), stdin);

	if(id_size != DB_ID_SIZE)
	{
		r = ERROR_FILTER_SMUDGE_INVALID;
		goto error_id_read;
	}

	r = db_file_query(stdout, id);

	if(r == ERROR_DB_FILE_QUERY_COULD_NOT_FIND_FILE)
	{
		bool exists = false;
		char hook_path[1024]; // XXX
		char hash[DB_ID_HASH_SIZE + 1] = { '\0' }; // +1 for null
		char db_path[1024];
		char tmp[1024];
		int error = 0;

		exists = hook_full_path(hook_path, HOOK_NAME);

		if(!exists)
		{
			goto skip_pull;
		}

		db_id_parse(NULL, hash, id);
		hash[DB_ID_HASH_SIZE] = '\0';

		db_file_path(db_path, sizeof(db_path), id);

		// FIXME: unsafe for large paths
		snprintf(tmp, sizeof(tmp), "%s %s %s", hook_path, hash, db_path);

		error = system(tmp);

		if(error != 0)
		{
			r = ERROR_INTERNAL;
			goto error_system;
		}

		r = db_file_query(stdout, id);
	}

error_system:
error_id_read:
error_passthrough:

skip_pull:

	return r;
}

