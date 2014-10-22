#ifndef PATTERNS_H
#define PATTERNS_H

#include <stdbool.h>
#include "errors.h"

enum Error patterns_init(void);

bool patterns_file_is_present_wc(void);
bool patterns_file_is_present_index(void);
bool patterns_file_is_present_head(void);

bool patterns_file_is_modified(void);

bool pattern_match_wc(const char *filename);
bool pattern_match_index(const char *filename);
bool pattern_match_head(const char *filename);

#endif

