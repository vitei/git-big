#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "errors.h"
#include "repo.h"
#include "commands/init.h"
#include "filters/clean.h"
#include "filters/smudge.h"

struct Command
{
	const char *command;
	enum Error (*function)(int argc, char *argv[]);
};

static const struct Command commands[] =
{
	{"gc",            NULL},
	{"init",          command_init_run},
	{"sync",          NULL},

	{"filter-clean",  filter_clean_run},
	{"filter-smudge", filter_smudge_run},
};

static void usage_instructions(void);
static void passthrough(void);

int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		usage_instructions();

		return 0;
	}
	else
	{
		const char *command = argv[1];

		argc -= 2;
		argv = &argv[2];

		for(int i = 0; i < sizeof(commands) / sizeof(commands[0]); ++i)
		{
			const struct Command *cmp_command = &commands[i];

			if(strcmp(cmp_command->command, command) == 0)
			{
				int error = 0;

				error = git_repository_open_ext(&repo_handle, ".", 0, NULL);

				if(error == 0)
				{
					enum Error error = ERROR_NONE;

					error = cmp_command->function(argc, argv);

					git_repository_free(repo_handle);
					repo_handle = NULL;

					if(error == ERROR_RUN_PASSTHROUGH)
					{
						passthrough();

						return 0;
					}
					else if(error != ERROR_NONE)
					{
						fprintf(stderr, "git-big encountered an error\n"
						                "%s\n", error_string_table[error]);

						return 1;
					}
					else
					{
						return 0;
					}
				}
				else
				{
					fprintf(stderr, "Invalid git repository\n");

					return 1;
				}
			}
		}

		fprintf(stderr, "Unrecognised command \"%s\"\n", command);

		return 1;
	}
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

