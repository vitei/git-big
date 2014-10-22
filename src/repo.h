#ifndef REPO_H
#define REPO_H

#include <git2.h>

#include "errors.h"

typedef void (*RepoWalkCallbackFunction)(const git_index_entry *, void *);

enum Error repo_tree_walk_bigfiles_all_index(RepoWalkCallbackFunction function, void *ptr);

enum Error repo_tree_walk_bigfiles_changed_index(RepoWalkCallbackFunction function, void *ptr);

extern git_repository *repo_handle;
extern git_config *repo_config_handle;

#endif
