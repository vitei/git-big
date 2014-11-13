#ifndef ERROR_H
#define ERROR_H

enum Error
{
	ERROR_NONE,
	ERROR_INTERNAL,
	ERROR_SILENT,

	ERROR_INIT_CORRUPT,

	ERROR_DB_INIT_COULD_NOT_CREATE_DIRECTORY,
	ERROR_DB_FILE_QUERY_COULD_NOT_FIND_FILE,
	ERROR_DB_FILE_INSERT_COULD_NOT_CREATE_FILE,
	ERROR_DB_ID_PARSE_HEADER_INVALID,

	ERROR_HOOKS_INIT_COULD_NOT_CREATE_PRE_COMMIT,
	ERROR_HOOKS_INIT_COULD_NOT_CREATE_PRE_PUSH,
	ERROR_HOOKS_PRE_PUSH_COULD_NOT_PUSH_FILE,

	ERROR_FILTER_SMUDGE_INVALID,

	ERROR_MAX
};

extern const char *error_string_table[ERROR_MAX];

#endif

