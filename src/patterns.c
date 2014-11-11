#include <fnmatch.h>
#include <stdio.h>
#include <unistd.h>

#include "patterns.h"
#include "repo.h"

struct BlobReaderData
{
	const void *data;
	git_off_t data_size;
	unsigned int i;
};

typedef char (*GetCharFunction)(void *);

static const char *const PATTERNS_FILE = ".gitbig";

static const char *get_path(void);
static git_blob *patterns_file_blob_index(void);
static git_blob *patterns_file_blob_head(void);
static bool pattern_match_blob(const char *filename, git_blob *blob);
static bool pattern_match(const char *filename, GetCharFunction get_char, void *data);
static unsigned char bgetc(struct BlobReaderData *data);

enum Error patterns_init(void)
{
	enum Error r = ERROR_NONE;
	FILE *patterns_file = NULL;

	if(patterns_file_is_present_wc())
	{
		return r;
	}

	// We need to make a dummy patterns file
	patterns_file = fopen(get_path(), "w");

	if(!patterns_file)
	{
		r = ERROR_PATTERNS_INIT_COULD_NOT_CREATE_PATTERNS_FILE;
		goto error_fopen;
	}

	fprintf(patterns_file, "# Put your git-big rules here!\n"
							"*\n");

	fclose(patterns_file);

error_fopen:

	return r;
}

bool patterns_file_is_present_wc(void)
{
	return access(get_path(), F_OK) != -1;
}

bool patterns_file_is_present_index(void)
{
	return patterns_file_blob_index() != NULL;
}

bool patterns_file_is_present_head(void)
{
	return patterns_file_blob_head() != NULL;
}

enum Error patterns_file_is_modified(bool *is_modified)
{
	enum Error r = ERROR_NONE;
	int error = 0;
	unsigned int status_flags = 0;

	error = git_status_file(&status_flags, repo_handle, PATTERNS_FILE);

	if(error != 0 && error != GIT_ENOTFOUND)
	{
		// FIXME: This does not handle errors exhaustively
		r = ERROR_INTERNAL;
		goto error_git_status_file;
	}

	if(error == GIT_ENOTFOUND)
	{
		*is_modified = false;
	}
	else
	{
		*is_modified = status_flags & (  GIT_STATUS_INDEX_NEW
		                               | GIT_STATUS_INDEX_MODIFIED
		                               | GIT_STATUS_INDEX_DELETED
		                               | GIT_STATUS_INDEX_RENAMED
		                               | GIT_STATUS_INDEX_TYPECHANGE);
	}

error_git_status_file:

	return r;
}

bool pattern_match_wc(const char *filename)
{
	bool r = false;
	FILE *patterns_file = NULL;

	patterns_file = fopen(get_path(), "r");

	if(!patterns_file)
	{
		goto error_fopen;
	}
		
	r = pattern_match(filename, (GetCharFunction)fgetc, patterns_file);
	fclose(patterns_file);

error_fopen:

	return r;
}

bool pattern_match_index(const char *filename)
{
	return pattern_match_blob(filename, patterns_file_blob_index());
}

bool pattern_match_head(const char *filename)
{
	return pattern_match_blob(filename, patterns_file_blob_head());
}

static const char *get_path(void)
{
	static char path[1024] = { '\0' };

	if(!*path)
	{
		const char *working_dir = git_repository_workdir(repo_handle);

		// FIXME: unsafe for large paths
		snprintf(path, sizeof(path), "%s%s", working_dir, PATTERNS_FILE);
	}

	return (const char *)path;
}

static git_blob *patterns_file_blob_index(void)
{
	static git_blob *blob = NULL;
	int error = 0;
	git_index *idx = NULL;
	const git_index_entry *entry = NULL;
	git_oid *oid = NULL;

	if(blob)
	{
		return blob;
	}

	error = git_repository_index(&idx, repo_handle);

	if(error != 0)
	{
		goto error_git_repository_index;
	}

	entry = git_index_get_bypath(idx, PATTERNS_FILE, 0);

	if(entry == NULL)
	{
		goto git_index_get_bypath;
	}

	error = git_blob_lookup(&blob, repo_handle, &entry->id);

	if(error != 0)
	{
		blob = NULL;
		goto git_blob_lookup;
	}

git_blob_lookup:
git_index_get_bypath:
	git_index_free(idx);
error_git_repository_index:

	return blob;
}

static git_blob *patterns_file_blob_head(void)
{
	static git_blob *blob = NULL;
	int error = 0;
	git_oid *oid = NULL;
	git_reference *reference = NULL;
	git_commit *commit = NULL;
	git_tree *tree = NULL;
	git_tree_entry *entry = NULL;

	if(blob)
	{
		return blob;
	}

	error = git_repository_head(&reference, repo_handle);

	if(error != 0)
	{
		goto error_git_repository_head;
	}

	oid = (git_oid *)git_reference_target(reference);

	if(oid == 0)
	{
		goto error_git_reference_target;
	}

	error = git_commit_lookup(&commit, repo_handle, oid);

	if(error != 0)
	{
		goto error_git_commit_lookup;
	}

	error = git_commit_tree(&tree, commit);

	if(error != 0)
	{
		goto error_git_commit_tree;
	}

	error = git_tree_entry_bypath(&entry, tree, PATTERNS_FILE);

	if(error != 0)
	{
		goto error_git_tree_entry_bypath;
	}

	if(git_tree_entry_type(entry) != GIT_OBJ_BLOB)
	{
		goto error_invalid_blob;
	}

	oid = (git_oid *)git_tree_entry_id(entry);

	if(oid == 0)
	{
		goto error_git_tree_entry_id;
	}

	error = git_blob_lookup(&blob, repo_handle, oid);

	if(error != 0)
	{
		blob = NULL;
		goto error_git_blob_lookup;
	}

error_git_blob_lookup:
error_git_tree_entry_id:
error_invalid_blob:
	git_tree_entry_free(entry);
error_git_tree_entry_bypath:
	git_tree_free(tree);
error_git_commit_tree:
	git_commit_free(commit);
error_git_commit_lookup:
error_git_reference_target:
	git_reference_free(reference);
error_git_repository_head:

	return blob;
}

static bool pattern_match_blob(const char *filename, git_blob *blob)
{
	struct BlobReaderData patterns_data;

	if(!blob)
	{
		return false;
	}

	patterns_data.data = git_blob_rawcontent(blob);
	patterns_data.data_size = git_blob_rawsize(blob);
	patterns_data.i = 0;

	return pattern_match(filename, (GetCharFunction)bgetc, &patterns_data);
}


static bool pattern_match(const char *filename, GetCharFunction get_char, void *data)
{
	FILE *patterns_file = NULL;

	patterns_file = fopen(get_path(), "r");

	if(patterns_file)
	{
		char pattern[1024]; // FIXME, this may overflow
		char current_char = 0;

		do
		{
			char *pattern_char = NULL;

			pattern[0] = '\0';

			do
			{
				current_char = get_char(data);
			}
			while(current_char == ' ' || current_char == '\t');

			for(pattern_char = pattern; ; ++pattern_char)
			{
				if(   current_char == '#'
				   || current_char == '\n'
				   || current_char == EOF)
				{
					while(current_char != '\n' && current_char != EOF)
					{
						current_char = get_char(data);
					}

					*pattern_char = '\0';
					break;
				}

				*pattern_char = current_char;
				current_char = get_char(data);
			}

			if(pattern[0])
			{
				if(fnmatch(pattern, filename, 0) == 0)
				{
					return true;
				}
			}
		}
		while(current_char != EOF);
	}

	return false;
}

static unsigned char bgetc(struct BlobReaderData *data)
{
	return data->i < data->data_size
	     ? ((unsigned char *)data->data)[++data->i]
	     : EOF;
}

