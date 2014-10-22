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
	if(!patterns_file_is_present_wc())
	{
		FILE *patterns_file = NULL;

		// We need to make a dummy patterns file
		patterns_file = fopen(get_path(), "w");

		if(patterns_file)
		{
			fprintf(patterns_file, "# Put your git-big rules here!\n"
			                       "*\n");

			fclose(patterns_file);
		}
		else
		{
			return ERROR_PATTERNS_INIT_COULD_NOT_CREATE_PATTERNS_FILE;
		}
	}

	return ERROR_NONE;
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

bool pattern_match_wc(const char *filename)
{
	FILE *patterns_file = NULL;

	patterns_file = fopen(get_path(), "r");

	if(patterns_file)
	{
		bool r = false;

		r = pattern_match(filename, (GetCharFunction)fgetc, patterns_file);
		fclose(patterns_file);

		return r;
	}
	else
	{
		return false;
	}
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

	if(!blob)
	{
		int error = 0;
		const git_index_entry *entry = NULL;
		git_oid *oid = NULL;
		git_index *idx = NULL;

		entry = git_index_get_bypath(idx, PATTERNS_FILE, 0);

		if(entry == NULL)
		{
			return NULL;
		}

		error = git_blob_lookup(&blob, repo_handle, &entry->id);

		if(error != 0)
		{
			return NULL;
		}
	}

	return blob;
}

static git_blob *patterns_file_blob_head(void)
{
	static git_blob *blob = NULL;

	if(!blob)
	{
		int error = 0;
		git_oid *oid = NULL;
		git_reference *reference = NULL;
		git_commit *commit = NULL;
		git_tree *tree = NULL;
		git_tree_entry *entry = NULL;

		error = git_repository_head(&reference, repo_handle);

		if(error != 0)
		{
			return NULL;
		}

		oid = (git_oid *)git_reference_target(reference);

		if(oid == 0)
		{
			return NULL;
		}

		error = git_commit_lookup(&commit, repo_handle, oid);

		if(error != 0)
		{
			return NULL;
		}

		error = git_commit_tree(&tree, commit);

		if(error != 0)
		{
			return NULL;
		}

		error = git_tree_entry_bypath(&entry, tree, PATTERNS_FILE);

		if(error != 0)
		{
			return NULL;
		}

		if(git_tree_entry_type(entry) != GIT_OBJ_BLOB)
		{
			git_tree_entry_free(entry);

			return NULL;
		}

		oid = (git_oid *)git_tree_entry_id(entry);

		if(oid == 0)
		{
			git_tree_entry_free(entry);

			return NULL;
		}

		error = git_blob_lookup(&blob, repo_handle, oid);
		git_tree_entry_free(entry);

		if(error != 0)
		{
			return NULL;
		}
	}

	return blob;
}

static bool pattern_match_blob(const char *filename, git_blob *blob)
{
	if(blob)
	{
		bool r = false;
		struct BlobReaderData patterns_data;

		patterns_data.data = git_blob_rawcontent(blob);
		patterns_data.data_size = git_blob_rawsize(blob);
		patterns_data.i = 0;

		r = pattern_match(filename, (GetCharFunction)bgetc, &patterns_data);

		return r;
	}
	else
	{
		return false;
	}
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
			pattern[0] = '\0';

			do
			{
				current_char = get_char(data);
			}
			while(current_char == ' ' || current_char == '\t');

			for(char *pattern_char = pattern; ; ++pattern_char)
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
	if(data->i < data->data_size)
	{
		return ((unsigned char *)data->data)[++data->i];
	}
	else
	{
		return EOF;
	}
}

