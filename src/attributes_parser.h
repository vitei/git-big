#ifndef ATTRIBUTES_PARSER_H
#define ATTRIBUTES_PARSER_H

#include "bool.h"

bool attributes_parser_match(const void *data, unsigned long size, const char *repo_path, const char *attribute, const char *match);

#endif

