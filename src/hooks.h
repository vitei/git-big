#ifndef HOOKS_H
#define HOOKS_H

#include "bool.h"
#include "errors.h"

enum Error hooks_init(void);

bool hook_full_path(char *full_path, const char *name);

#endif

