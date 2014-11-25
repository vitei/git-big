#ifndef ATTRIBUTES_H
#define ATTRIBUTES_H

#include "bool.h"
#include "errors.h"

enum AttributesStatus
{
	ATTRIBUTES_STATUS_UNMODIFIED,
	ATTRIBUTES_STATUS_MODIFIED,
};

enum Error attributes_get_status(enum AttributesStatus *status);

enum Error attributes_match_index(bool *is_match, const char *repo_path, const char *attribute, const char *match);

bool attributes_fnmatch(const char *filename, const char *filter);

#endif

