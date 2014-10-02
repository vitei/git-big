#include <stdio.h>
#include <string.h>

void usageInstructions(void)
{
	fprintf(stdout, "usage: git big <command> [<args>]\n\n"
	                "The following are git-big commands:\n"
	                "   gc     Clean up file cache\n"
	                "   init   Initialise git-big for this repository\n"
	                "   sync   Synchronise big files\n");
}

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

		if(strcmp(command, "gc") == 0)
		{
			return 0;
		}
		else if(strcmp(command, "init") == 0)
		{
			return 0;
		}
		else if(strcmp(command, "sync") == 0)
		{
			return 0;
		}
		else if(strcmp(command, "filter-clean") == 0)
		{
			return 0;
		}
		else if(strcmp(command, "filter-smudge") == 0)
		{
			return 0;
		}
		else
		{
			fprintf(stderr, "Unrecognised command \"%s\"\n", command);
			return 1;
		}
	}

}
