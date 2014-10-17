#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>

#if defined(TARGET_OS_OSX)
	#include <CommonCrypto/CommonDigest.h>

	#define SHA_CTX CC_SHA1_CTX
	#define SHA1_Init CC_SHA1_Init
	#define SHA1_Update CC_SHA1_Update
	#define SHA1_Final CC_SHA1_Final
	#define SHA_DIGEST_LENGTH CC_SHA1_DIGEST_LENGTH
#elif defined(TARGET_OS_LINUX)
	#include <openssl/sha.h>
#else
	#error Unsupported platform
#endif

#include "db.h"
#include "repo.h"

static const char * const DB_DIRECTORY = "big";

static const char *get_path(void);

enum Error db_init(void)
{
	int error = 0;

	error = mkdir(get_path(), 0777);

	if(error == 0 || errno == EEXIST)
	{
		return ERROR_NONE;
	}
	else
	{
		return ERROR_DB_INIT_COULD_NOT_CREATE_DIRECTORY;
	}
}

enum Error db_file_query(char *id, FILE *output)
{
	char path[1024] = { '\0' };
	FILE *file = NULL;

	snprintf(path, sizeof(path), "%s%s", get_path(), id);

	file = fopen(path, "r");

	if(file)
	{
		char buffer[1024];
		size_t read_size;

		do
		{
			read_size = fread(buffer, 1, sizeof(buffer), file);
			fwrite(buffer, 1, read_size, output);
		}
		while(read_size == sizeof(buffer));

		fclose(file);

		return ERROR_NONE;
	}
	else
	{
		return ERROR_DB_FILE_QUERY_COULD_NOT_FIND_FILE;
	}
}

enum Error db_file_insert(FILE *input, char *id)
{
	char tmp_path[1024] = { '\0' };
	FILE *file = NULL;
	SHA_CTX ctx;
	char buffer[1024];
	size_t size = 0;

	SHA1_Init(&ctx);

	snprintf(tmp_path, sizeof(tmp_path), "%stmp", get_path());
	file = fopen(tmp_path, "wb");

	if(file)
	{
		char path[1024] = { '\0' };

		do
		{
			size = fread(buffer, 1, sizeof(buffer), input);
			fwrite(buffer, 1, size, file);
			SHA1_Update(&ctx, (unsigned char *)buffer, size);
		}
		while(size == sizeof(buffer));

		fclose(file);

		SHA1_Final((unsigned char *)buffer, &ctx);

		for(int i = 0, j = 0; i < SHA_DIGEST_LENGTH; ++i, j += 2)
		{
			sprintf(&id[j], "%02x", (unsigned char)buffer[i]);
		}

		snprintf(path, sizeof(path), "%s%s", get_path(), id);
		rename(tmp_path, path);

		return ERROR_NONE;
	}
	else
	{
		return ERROR_DB_FILE_INSERT_COULD_NOT_CREATE_FILE;
	}
}

static const char *get_path(void)
{
	static char path[1024] = { '\0' };

	if(!*path)
	{
		const char *dot_git_dir = git_repository_path(repo_handle);

		// FIXME: unsafe for large paths
		snprintf(path, sizeof(path), "%s%s/", dot_git_dir, DB_DIRECTORY);
	}

	return (const char *)path;
}

