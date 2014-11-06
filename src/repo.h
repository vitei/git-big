#ifndef REPO_H
#define REPO_H

#include <git2.h>

#include "errors.h"

typedef void (*RepoWalkCallbackFunction)(const char *repo_path, const char *db_path, void *payload);

enum Error repo_tree_walk_bigfiles_for_push(const git_oid *from, const git_oid *to, RepoWalkCallbackFunction function, void *payload);
enum Error repo_tree_walk_bigfiles_all_index(RepoWalkCallbackFunction function, void *payload);
enum Error repo_tree_walk_bigfiles_changed_index(RepoWalkCallbackFunction function, void *payload);

extern git_repository *repo_handle;
extern git_config *repo_config_handle;

#endif
