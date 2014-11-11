#include <stdio.h>
#include <string.h>

#include "bool.h"
#include "errors.h"
#include "repo.h"
#include "commands/init.h"
#include "filters/clean.h"
#include "filters/smudge.h"
#include "hooks/pre_commit.h"
#include "hooks/pre_push.h"

struct Command
{
	const char *command;
	enum Error (*function)(int argc, char *argv[]);
};

static const struct Command commands[] = {
	{ "gc",              NULL },
	{ "init",            command_init_run },
	{ "sync",            NULL },

	{ "filter-clean",    filter_clean_run },
	{ "filter-smudge",   filter_smudge_run },

	{ "hook-pre-commit", hooks_pre_commit_run },
	{ "hook-pre-push",   hooks_pre_push_run },
};

static void usage_instructions(void);
static void passthrough(void);

int main(int argc, char *argv[])
{
	int r = 0;
	const char *command;
	enum Error command_error = ERROR_NONE;
	int i = 0;
	int end = 0;

	if(argc < 2)
	{
		usage_instructions();

		goto error_usage;
	}

	command = argv[1];
	argc -= 2;
	argv = &argv[2];

	for(i = 0, end = sizeof(commands) / sizeof(commands[0]); i != end; ++i)
	{
		const struct Command *cmp_command = &commands[i];

		if(i == end)
		{
			fprintf(stderr, "Unrecognised command \"%s\"\n", command);
			goto error_invalid_command;
		}
		else if(strcmp(cmp_command->command, command) == 0)
		{
			int error = 0;

			error = git_repository_open_ext(&repo_handle, ".", 0, NULL);

			if(error != 0)
			{
				fprintf(stderr, "Invalid git repository\n");
				goto error_git_repository_open_ext;
			}

			command_error = cmp_command->function(argc, argv);

			break;
		}
	}

	switch(command_error)
	{
		case ERROR_SILENT:
			r = 1;

		case ERROR_NONE:
			break;

		case ERROR_RUN_PASSTHROUGH:
			passthrough();
			r = 1;

			break;

		case ERROR_INTERNAL:
			{
				const git_error *git_error = giterr_last();

				if(git_error)
				{
					fprintf(stderr,
					        "Internal Git Error: %s\n", git_error->message);
				}
			}
		default:
			fprintf(stderr, "git-big encountered an error:\n"
			                "%s\n", error_string_table[command_error]);
			r = 1;

			break;
	}

	git_repository_free(repo_handle);
error_git_repository_open_ext:
error_invalid_command:
error_usage:

	return r;
}

static void usage_instructions(void)
{
	fprintf(stdout, "usage: git big <command> [<args>]\n\n"
	                "The following are git-big commands:\n"
	                "   gc     Clean up file cache\n"
	                "   init   Initialise git-big for this repository\n"
	                "   sync   Synchronise big files\n");
}

static void passthrough(void)
{
	char buffer[1024];
	size_t read_size;

	do
	{
		read_size = fread(buffer, 1, sizeof(buffer), stdin);
		fwrite(buffer, 1, read_size, stdout);
	}
	while(read_size == sizeof(buffer));
}

