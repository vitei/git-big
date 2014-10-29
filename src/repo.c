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
static int diff_file_callback(const git_diff_delta *delta, float progress, struct DiffCallbackData *payload);

git_repository *repo_handle = NULL;
git_config *repo_config_handle = NULL;

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

	git_diff_foreach(diff, (git_diff_file_cb)diff_file_callback, NULL, NULL, &diff_callback_data);

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

static int diff_file_callback(const git_diff_delta *delta, float progress, struct DiffCallbackData *payload)
{
	const git_index_entry *entry = git_index_get_bypath(payload->idx, delta->new_file.path, 0);

	// Don't handle this now, we need to go through all files
	payload->r = process_idx_entry(entry, payload->function, payload->ptr);

	return 0;
}

