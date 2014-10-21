#include <stdio.h>


#include "smudge.h"
#include "../db.h"
#include "../patterns.h"

enum Error filter_smudge_run(int argc, char *argv[])
{
	char *filename = argv[0];

	if(patterns_file_is_present_head() && pattern_match_head(filename))
	{
		char id[DB_ID_SIZE] = { '\0' };
		size_t id_size = 0;

		id_size = fread(id, 1, sizeof(id), stdin);

		if(id_size == DB_ID_SIZE)
		{
			return db_file_query(id, stdout);
		}
		else
		{
			return ERROR_FILTER_SMUDGE_INVALID;
		}
	}
	else
	{
		return ERROR_RUN_PASSTHROUGH;
	}
}

