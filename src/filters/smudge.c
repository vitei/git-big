#include <stdio.h>

#include "smudge.h"
#include "../db.h"
#include "../patterns.h"

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

error_id_read:
error_passthrough:

	return r;
}

