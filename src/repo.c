#include "db.h"
#include "repo.h"

git_repository *repo_handle = NULL;
git_config *repo_config_handle = NULL;

enum Error repo_tree_walk_bigfiles_all_index(RepoWalkCallbackFunction function, void *ptr)
{
	int error = 0;
	git_index *idx = NULL;
	size_t count = 0;

	error = git_repository_index(&idx, repo_handle);

	count = git_index_entrycount(idx);

	for (size_t i = 0; i < count; ++i)
	{
		const git_index_entry *entry = git_index_get_byindex(idx, i);
		git_blob *blob = NULL;

		error = git_blob_lookup(&blob, repo_handle, &entry->id);

		if(error == 0)
		{
			size_t size = 0;

			size = git_blob_rawsize(blob) - 1; // -1 for NULL character

			if(size == DB_ID_SIZE)
			{
				const void *data = NULL;
				enum Error parse_error = ERROR_NONE;

				data = git_blob_rawcontent(blob);

				parse_error = db_id_parse(NULL, NULL, data);

				 if(parse_error == ERROR_NONE)
				 {
					function(entry, ptr);
				 }
			}
		}
		else
		{
			return ERROR_INTERNAL;
		}
	}

	return ERROR_NONE;
}

enum Error repo_tree_walk_bigfiles_changed_index(RepoWalkCallbackFunction function, void *ptr)
{
	// FIXME: use git_diff_tree_to_index and git_diff_foreach
	return repo_tree_walk_bigfiles_all_index(function, ptr);
}

