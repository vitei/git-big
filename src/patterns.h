#ifndef PATTERNS_H
#define PATTERNS_H

#include <stdbool.h>
#include "errors.h"

enum Error patterns_init(void);

bool patterns_file_is_present_wc(void);

bool pattern_match_wc(char *filename);

#endif

