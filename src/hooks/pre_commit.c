#include <stdio.h>
#include <sys/time.h>

#include "pre_commit.h"
#include "../attributes.h"
#include "../bool.h"
#include "../repo.h"

struct AttributeCheckPayload
{
	bool error_shown;
	enum Error r;
};

static void attribute_check(const char *repo_path, const char *db_hash, const char *db_path, void *payload);
static void touch_repo_file(const char *filename);

enum Error hooks_pre_commit_run(int argc, char *argv[])
{
	enum Error r = ERROR_NONE;
	enum AttributesStatus status = ATTRIBUTES_STATUS_UNMODIFIED;
	struct AttributeCheckPayload payload = { false, ERROR_NONE };

	if(git_repository_is_empty(repo_handle))
	{
		// Can't run a diff on an empty repository
		status = ATTRIBUTES_STATUS_MODIFIED;
	}
	else
	{
		r = attributes_get_status(&status);

		if(r != ERROR_NONE)
		{
			goto error_attributes_get_status;
		}
	}

	switch(status)
	{
		 case ATTRIBUTES_STATUS_UNMODIFIED:
			/* Only check staged files */
			r = repo_tree_walk_bigfiles_changed_index(attribute_check, &payload);

			if(r != ERROR_NONE)
			{
				goto error_repo_tree_walk_bigfiles_changed_index;
			}

			break;

		case ATTRIBUTES_STATUS_MODIFIED:
			/* All files need to be checked */
			r = repo_tree_walk_bigfiles_all_index(attribute_check, &payload);

			if(r != ERROR_NONE)
			{
				goto error_repo_tree_walk_bigfiles_all_index;
			}

			break;
	}

	if(payload.r != ERROR_NONE)
	{
		r = payload.r;
	}
	else if(payload.error_shown)
	{
		r = ERROR_SILENT;
	}

error_repo_tree_walk_bigfiles_all_index:
error_repo_tree_walk_bigfiles_changed_index:
error_attributes_get_status:

	return r;
}

static void attribute_check(const char *repo_path, const char *db_hash, const char *db_path, void *payload)
{
	enum Error r = ERROR_NONE;
	struct AttributeCheckPayload *check_payload = (struct AttributeCheckPayload *)payload;
	bool match = false;

	r = attributes_match_index(&match, repo_path, "filter", "big");

	if(r != ERROR_NONE)
	{
		goto error_attributes_match_index;
	}

	if(!match)
	{
		touch_repo_file(repo_path);

		if(!check_payload->error_shown)
		{
			fprintf(stderr, "The following files are handled by git-big and will break with the current .gitattributes\n"
			                "Please add valid rules to .gitattributes and/or re-add the files to git\n");

			check_payload->error_shown = true;
		}

		fprintf(stderr, "   %s\n", repo_path);
	}

error_attributes_match_index:

	/* FIXME: would like to make this nicer sometime... */
	if(r != ERROR_NONE)
	{
		check_payload->r = r;
	}
}

static void touch_repo_file(const char *filename)
{
	char path[1024] = { '\0' };
	const char *working_dir = git_repository_workdir(repo_handle);

	// FIXME: unsafe for large paths
	snprintf(path, sizeof(path), "%s%s", working_dir, filename);

	// Touch the file!
	utimes(path, NULL);
}

