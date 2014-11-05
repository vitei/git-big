#include <assert.h>
#include <stdbool.h>
#include <string.h>

#include "db.h"
#include "repo.h"

struct DiffCallbackData
{
	RepoWalkCallbackFunction function;
	void *ptr;
	git_index *idx;
	enum Error r;
};

static enum Error process_idx_entry(const git_index_entry *entry, RepoWalkCallbackFunction function, void *ptr);
static int diff_file_callback(const git_diff_delta *delta, float progress, void *payload);

git_repository *repo_handle = NULL;
git_config *repo_config_handle = NULL;



static int testone(const git_diff_delta *delta, float progress, void *payload)
{
	fprintf(stderr, "XXX: %s\n", delta->new_file.path);
	return 0;
}
static int walk_cb(const char *root, const git_tree_entry *entry, void *payload)
{
	fprintf(stderr, "XXX: %s\n", git_tree_entry_name(entry));
	return 0;
}

enum Error repo_tree_walk_bigfiles_for_push(const git_oid *from, const git_oid *to, RepoWalkCallbackFunction function, void *ptr)
{
	enum Error r = ERROR_NONE;
	int error = 0;
	bool from_initial = false;
	git_oid target;
	git_commit *current_commit = NULL;
	git_tree *current_tree = NULL;
	unsigned int parent_count = 0;

	from_initial = git_oid_iszero(from);

	if(!from_initial)
	{
		error = git_merge_base(&target, repo_handle, from, to);

		if(error != 0 && error != GIT_ENOTFOUND)
		{
			r = ERROR_INTERNAL;
			goto error_git_merge_base;
		}
	}

	if(from_initial || error == GIT_ENOTFOUND)
	{
		memset(&target, 0, sizeof(target));
		from_initial = true;
	}

	error = git_commit_lookup(&current_commit, repo_handle, to);

	if(error != 0)
	{
		r = ERROR_INTERNAL;
		goto error_git_commit_lookup;
	}

	error = git_commit_tree(&current_tree, current_commit);

	if(error != 0)
	{
		r = ERROR_INTERNAL;
		goto error_git_commit_tree_current;
	}

	do
	{
		git_commit *parent_commit = NULL;
		git_tree *parent_tree = NULL;
		git_diff *diff = NULL;
		git_diff_options diff_options = GIT_DIFF_OPTIONS_INIT;

		parent_count = git_commit_parentcount(current_commit);

		switch(parent_count)
		{
			case 0:
				error = git_tree_walk(current_tree, GIT_TREEWALK_PRE, walk_cb,     NULL);

				if(error != 0)
				{
					r = ERROR_INTERNAL;
					goto error_git_tree_walk;
				}

				assert(from_initial);

				break;

			default:
				for(unsigned int i = 1; i < parent_count; ++i)
				{
					const git_oid *new_to = NULL;

					error = git_commit_parent(&parent_commit, current_commit, i);

					if(error != 0)
					{
						r = ERROR_INTERNAL;
						goto error_git_commit_parent;
					}

					error = git_commit_tree(&parent_tree, parent_commit);

					if(error != 0)
					{
						r = ERROR_INTERNAL;
						goto error_git_commit_tree_parent;
					}

					error = git_diff_tree_to_tree(&diff, repo_handle,
					                              parent_tree, current_tree,
					                              &diff_options);

					if(error != 0)
					{
						r = ERROR_INTERNAL;
						goto error_git_diff_tree_to_tree;
					}

					error = git_diff_foreach(diff, testone, NULL, NULL,       NULL);

					if(error != 0)
					{
						r = ERROR_INTERNAL;
						goto error_git_diff_foreach;
					}

					new_to = git_commit_id(parent_commit);

					git_diff_free(diff);
					git_tree_free(parent_tree);
					git_commit_free(parent_commit);

					r = repo_tree_walk_bigfiles_for_push(from, new_to, function, ptr);

					if(r != ERROR_NONE)
					{
						goto error_git_tree_walk;
					}
				}

				if(!from_initial)
				{
					error = git_commit_parent(&parent_commit, current_commit, 0);

					if(error != 0)
					{
						r = ERROR_INTERNAL;
						goto error_git_commit_parent;
					}

					to = git_commit_id(parent_commit);
					error = git_merge_base(&target, repo_handle, from, to); 

					if(error != 0 && error != GIT_ENOTFOUND)
					{
						r = ERROR_INTERNAL;
						goto error_git_merge_base_retarget;
					}

					if(error == GIT_ENOTFOUND)
					{
						memset(&target, 0, sizeof(target));
						from_initial = true;
					}
				}

			case 1:
				if(!parent_commit)
				{
					error = git_commit_parent(&parent_commit, current_commit, 0);
				}

				if(error != 0)
				{
					r = ERROR_INTERNAL;
					goto error_git_commit_parent;
				}

				error = git_commit_tree(&parent_tree, parent_commit);

				if(error != 0)
				{
					r = ERROR_INTERNAL;
					goto error_git_commit_tree_parent;
				}

				error = git_diff_tree_to_tree(&diff, repo_handle,
				                              parent_tree, current_tree,
				                              &diff_options);

				if(error != 0)
				{
					r = ERROR_INTERNAL;
					goto error_git_diff_tree_to_tree;
				}

				error = git_diff_foreach(diff, testone, NULL, NULL,       NULL);

				if(error != 0)
				{
					r = ERROR_INTERNAL;
					goto error_git_diff_foreach;
				}

				git_diff_free(diff);
				git_tree_free(current_tree);
				git_commit_free(current_commit);

				current_commit = parent_commit;
				current_tree = parent_tree;

				if(git_oid_equal(git_commit_id(current_commit), &target))
				{
					goto finished_commit_walk;
				}

				break;
		}

		continue;

error_git_diff_foreach:
		git_diff_free(diff);
error_git_diff_tree_to_tree:
		git_tree_free(parent_tree);
error_git_merge_base_retarget:
error_git_commit_tree_parent:
		git_commit_free(parent_commit);
error_git_commit_parent:
		goto error_git_tree_walk;

	}
	while(parent_count);

finished_commit_walk: // FIXME: this goto is a little nasty...

error_git_tree_walk:
	git_tree_free(current_tree);
error_git_commit_tree_current:
	git_commit_free(current_commit);
error_git_commit_lookup:
error_git_merge_base:

	return r;
}

enum Error repo_tree_walk_bigfiles_all_index(RepoWalkCallbackFunction function, void *ptr)
{
	enum Error r = ERROR_NONE;
	int error = 0;
	git_index *idx = NULL;
	size_t count = 0;

	error = git_repository_index(&idx, repo_handle);

	if(error != 0)
	{
		r = ERROR_INTERNAL;
		goto error_git_repository_index;
	}

	count = git_index_entrycount(idx);

	for(size_t i = 0; i < count; ++i)
	{
		const git_index_entry *entry = git_index_get_byindex(idx, i);

		// Don't handle this now, we need to go through all files
		r = process_idx_entry(entry, function, ptr);
	}

	git_index_free(idx);
error_git_repository_index:

	return r;
}

enum Error repo_tree_walk_bigfiles_changed_index(RepoWalkCallbackFunction function, void *ptr)
{
	enum Error r = ERROR_NONE;
	int error = 0;
	struct DiffCallbackData diff_callback_data = { function, ptr, NULL, ERROR_NONE };
	git_reference *reference = NULL;
	git_oid *oid = NULL;
	git_commit *commit = NULL;
	git_tree *tree = NULL;
	git_diff *diff = NULL;

	error = git_repository_index(&diff_callback_data.idx, repo_handle);

	if(error != 0)
	{
		r = ERROR_INTERNAL;
		goto error_git_repository_index;
	}

	error = git_repository_head(&reference, repo_handle);

	if(error != 0)
	{
		r = ERROR_INTERNAL;
		goto error_git_repository_head;
	}

	oid = (git_oid *)git_reference_target(reference);

	if(oid == 0)
	{
		r = ERROR_INTERNAL;
		goto error_git_reference_target;
	}

	error = git_commit_lookup(&commit, repo_handle, oid);

	if(error != 0)
	{
		r = ERROR_INTERNAL;
		goto error_git_commit_lookup;
	}

	error = git_commit_tree(&tree, commit);

	if(error != 0)
	{
		r = ERROR_INTERNAL;
		goto error_git_commit_tree;
	}

	error = git_diff_tree_to_index(&diff, repo_handle, tree, NULL, NULL);

	if(error != 0)
	{
		r = ERROR_INTERNAL;
		goto error_git_diff_tree_to_index;
	}

	git_diff_foreach(diff, diff_file_callback, NULL, NULL, &diff_callback_data);

	git_diff_free(diff);

error_git_diff_tree_to_index:
	git_tree_free(tree);
error_git_commit_tree:
	git_commit_free(commit);
error_git_commit_lookup:
error_git_reference_target:
	git_reference_free(reference);
error_git_repository_head:
error_git_repository_index:

	return r;
}

static enum Error process_idx_entry(const git_index_entry *entry, RepoWalkCallbackFunction function, void *ptr)
{
	enum Error r = ERROR_NONE;
	int error = 0;
	git_blob *blob = NULL;
	size_t size = 0;

	error = git_blob_lookup(&blob, repo_handle, &entry->id);

	if(error != 0)
	{
		r = ERROR_INTERNAL;
		goto error_git_blob_lookup;
	}

	size = git_blob_rawsize(blob);

	if(size == DB_ID_SIZE + 1) // +1 for NULL character
	{
		const void *data = NULL;
		enum Error parse_error = ERROR_NONE;

		data = git_blob_rawcontent(blob);

		parse_error = db_id_parse(NULL, NULL, data);

		if(parse_error == ERROR_NONE)
		{
			function(entry->path, ptr);
		}
	}

error_git_blob_lookup:

	return r;
}

static int diff_file_callback(const git_diff_delta *delta, float progress, void *payload)
{
	struct DiffCallbackData *data = (struct DiffCallbackData *)payload;
	const git_index_entry *entry = git_index_get_bypath(data->idx,
	                                                    delta->new_file.path, 0);

	// Don't handle this now, we need to go through all files
	data->r = process_idx_entry(entry, data->function, data->ptr);

	return 0;
}

