#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "errors.h"
#include "patterns.h"
#include "repo.h"
#include "commands/init.h"
#include "filters/clean.h"
#include "filters/smudge.h"

enum RequirementBehaviour
{
	kRequirementUnrequired,
	kRequirementRequired,
	kRequirementPassthrough
};

struct Command
{
	const char *command;
	enum Error (*function)(int argc, char *argv[]);
	enum RequirementBehaviour requiresGitBig;
};

static const struct Command commands[] =
{
	{"gc", NULL, kRequirementRequired},
	{"init", commandInitRun, kRequirementUnrequired},
	{"sync", NULL, kRequirementRequired},

	{"filter-clean", filterCleanRun, kRequirementPassthrough},
	{"filter-smudge", filterSmudgeRun, kRequirementPassthrough},
};

static void usageInstructions(void);
static void passthrough(void);

int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		usageInstructions();
		return 0;
	}
	else
	{
		const char *command = argv[1];

		argc -= 2;
		argv = &argv[2];

		for(int i = 0; i < sizeof(commands) / sizeof(commands[0]); ++i)
		{
			const struct Command *cmpCommand = &commands[i];

			if(strcmp(cmpCommand->command, command) == 0)
			{
				int error = 0;

				error = git_repository_open_ext(&gRepoHandle, ".", 0, NULL);

				if(error == 0)
				{
					if(   cmpCommand->requiresGitBig == kRequirementUnrequired
					   || patternsIsFilePresent())
					{
						enum Error error = kErrorNone;

						error = cmpCommand->function(argc, argv);

						git_repository_free(gRepoHandle);
						gRepoHandle = NULL;

						if(error != kErrorNone)
						{
							fprintf(stderr, "git-big encountered an error\n"
							                "%s\n", gErrorStrings[error]);
						}

						return error == kErrorNone ? 0 : 1;
					}
					else if(cmpCommand->requiresGitBig == kRequirementPassthrough)
					{
						passthrough();
						return 0;
					}
					else
					{
						fprintf(stderr, "This repository is not managed using git-big\n");
						return 1;
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

static void usageInstructions(void)
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
	size_t readSize;

	do
	{
		readSize = fread(buffer, 1, sizeof(buffer), stdin);
		fwrite(buffer, 1, readSize, stdout);
	}
	while(readSize == sizeof(buffer));
}

