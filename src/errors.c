#include "errors.h"

const char *error_string_table[ERROR_MAX] =
{
	"",
	"Internal error",
	"",

	"",

	"Git repository is corrupt",

	"Could not create the git-big database directory",
	"Could not find database file, please run git big sync",
	"Could not insert file into database",
	"Corrupt database file",

	"Could not create pre-commit hook",
	"Could not create pre-push hook",
	"Could not push a file managed by git-big",

	"Could not create .gitbig file",

	"Filter input is invalid",
};

