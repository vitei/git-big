#ifndef HOOKS_BIG_PUSH_H
#define HOOKS_BIG_PUSH_H

#include "../bool.h"
#include "../errors.h"

bool hook_big_push_exists(void);
enum Error hook_big_push_run(const char *db_hash, const char *db_path);

#endif

