#include "errors.h"

const char *gErrorStrings[kErrorMax] =
{
	"",
	"Internal error",

	"Git repository is corrupt",

	"Could not create the git-big database directory",

	"Could not create .gitbig file",

	"Could not find database file, please run git big sync"
	"Could not insert file into database",
};

