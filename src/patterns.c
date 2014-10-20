#include <fnmatch.h>
#include <stdio.h>
#include <unistd.h>

#include "patterns.h"
#include "repo.h"

typedef char (*GetCharFunction)(void *);

static const char *const PATTERNS_FILE = ".gitbig";

static const char *get_path(void);
static bool pattern_match(char *filename, GetCharFunction get_char, void *data);

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

bool pattern_match_wc(char *filename)
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

static bool pattern_match(char *filename, GetCharFunction get_char, void *data)
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

