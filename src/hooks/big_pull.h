#ifndef HOOKS_BIG_PULL_H
#define HOOKS_BIG_PULL_H

#include "../bool.h"
#include "../errors.h"

bool hook_big_pull_exists(void);
enum Error hook_big_pull_run(const char *db_hash, const char *db_path);

#endif

