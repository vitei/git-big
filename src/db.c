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

static const char *kDBDirectory = "big";

static const char *getPath(void);

enum Error dbInit(void)
{
	int error = 0;

	error = mkdir(getPath(), 0777);

	if(error == 0 || errno == EEXIST)
	{
		return kErrorNone;
	}
	else
	{
		return kErrorDBInitCouldNotCreateDirectory;
	}
}

enum Error dbQueryFile(char *id, FILE *output)
{
	char path[1024] = { '\0' };
	FILE *file = NULL;

	snprintf(path, sizeof(path), "%s%s", getPath(), id);

	file = fopen(path, "r");

	if(file)
	{
		char buffer[1024];
		size_t readSize;

		do
		{
			readSize = fread(buffer, 1, sizeof(buffer), file);
			fwrite(buffer, 1, readSize, output);
		}
		while(readSize == sizeof(buffer));

		fclose(file);

		return kErrorNone;
	}
	else
	{
		return kErrorDBQueryFileCouldNotFindFile;
	}
}

enum Error dbInsertFile(FILE *input, char *id)
{
	char tmpPath[1024] = { '\0' };
	FILE *file = NULL;
	SHA_CTX ctx;
	char buffer[1024];
	size_t size = 0;

	SHA1_Init(&ctx);

	snprintf(tmpPath, sizeof(tmpPath), "%stmp", getPath());
	file = fopen(tmpPath, "wb");

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

		snprintf(path, sizeof(path), "%s%s", getPath(), id);
		rename(tmpPath, path);

		return kErrorNone;
	}
	else
	{
		return kErrorDBInsertFileCouldNotCreateFile;
	}
}

static const char *getPath(void)
{
	static char path[1024] = { '\0' };

	if(!*path)
	{
		const char *dotGitDir = git_repository_path(gRepoHandle);

		// FIXME: unsafe for large paths
		snprintf(path, sizeof(path), "%s%s/", dotGitDir, kDBDirectory);
	}

	return (const char *)path;
}

