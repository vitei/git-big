#include <errno.h>
#include <stdio.h>
#include <string.h>
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
#elif defined(TARGET_OS_WINDOWS)
	#include <windows.h>
	#include <Wincrypt.h>

	#define SHA_DIGEST_LENGTH 20
#else
	#error Unsupported platform
#endif

#include "db.h"
#include "repo.h"

static const char * const DB_DIRECTORY = "big";
static const char ID_HEADER[DB_ID_HEADER_SIZE] = { 'G', 'I', 'T',
                                                   '-',
                                                   'B', 'I', 'G' };

static const char *get_path(void);

enum Error db_init(void)
{
	enum Error r = ERROR_NONE;
	int error = 0;

	error = mkdir(get_path(), 0777);

	if(error != 0 && errno != EEXIST)
	{
		r = ERROR_DB_INIT_COULD_NOT_CREATE_DIRECTORY;
		goto error_mkdir;
	}

error_mkdir:

	return r;
}

enum Error db_file_path(char *path, size_t path_length, const char *id)
{
	enum Error r = ERROR_NONE;
	char hash[DB_ID_HASH_SIZE + 1] = { '\0' }; // +1 for null

	r = db_id_parse(NULL, hash, id);

	if(r != ERROR_NONE)
	{
		goto error_db_id_parse;
	}

	hash[DB_ID_HASH_SIZE] = '\0';
	snprintf(path, path_length, "%s%s", get_path(), hash); // FIXME: buffer overflow

error_db_id_parse:

	return r;
}

enum Error db_file_query(FILE *output, char *id)
{
	enum Error r = ERROR_NONE;
	unsigned int version;
	char hash[DB_ID_HASH_SIZE + 1] = { '\0' }; // +1 for null
	char buffer[1024] = { '\0' };
	FILE *file = NULL;
	size_t read_size;

	r = db_id_parse(&version, hash, id);

	if(r != ERROR_NONE)
	{
		goto error_db_id_parse;
	}

	hash[DB_ID_HASH_SIZE] = '\0';

	snprintf(buffer, sizeof(buffer), "%s%s", get_path(), hash);

	file = fopen(buffer, "r");

	if(!file)
	{
		r = ERROR_DB_FILE_QUERY_COULD_NOT_FIND_FILE;
		goto error_fopen;
	}

	do
	{
		read_size = fread(buffer, 1, sizeof(buffer), file);
		fwrite(buffer, 1, read_size, output);
	}
	while(read_size == sizeof(buffer));

	fclose(file);

error_fopen:
error_db_id_parse:

	return r;
}

enum Error db_file_insert(char *id, FILE *input)
{
	enum Error r = ERROR_NONE;
	char tmp_path[1024] = { '\0' };
	FILE *file = NULL;
	char buffer[1024] = { '\0' };
	size_t size = 0;
	int error = 0;
	unsigned int version = DB_VERSION;
	char hash[DB_ID_HASH_SIZE + 1] = { '\0' }; // +1 for null
#if defined(TARGET_OS_OSX) || defined(TARGET_OS_LINUX)
	SHA_CTX ctx;
#elif defined(TARGET_OS_WINDOWS)
	BOOL success;
	HCRYPTPROV crypto_provider = 0;
	HCRYPTHASH crypto_hash = 0;
	DWORD hash_length = SHA_DIGEST_LENGTH;
#endif

#if defined(TARGET_OS_OSX) || defined(TARGET_OS_LINUX)
	SHA1_Init(&ctx);
#elif defined(TARGET_OS_WINDOWS)
	success = CryptAcquireContext(&crypto_provider, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT);

	if(!success)
	{
		r = ERROR_INTERNAL;
		goto error_crypt_acquire_context;
	}

	success = CryptCreateHash(crypto_provider, CALG_SHA1, 0, 0, &crypto_hash);

	if(!success)
	{
		r = ERROR_INTERNAL;
		goto error_crypt_create_hash;
	}
#endif

	snprintf(tmp_path, sizeof(tmp_path), "%stmp", get_path());
	file = fopen(tmp_path, "wb");

	if(!file)
	{
		r = ERROR_DB_FILE_INSERT_COULD_NOT_CREATE_FILE;
		goto error_fopen;
	}

	do
	{
		size = fread(buffer, 1, sizeof(buffer), input);
		fwrite(buffer, 1, size, file);

#if defined(TARGET_OS_OSX) || defined(TARGET_OS_LINUX)
		SHA1_Update(&ctx, (unsigned char *)buffer, size);
#elif defined(TARGET_OS_WINDOWS)
		success = CryptHashData(crypto_hash, buffer, size, 0);

		if(!success)
		{
			r = ERROR_INTERNAL;
			goto error_crypt_hash_data;
		}
#endif
	}
	while(size == sizeof(buffer));

	fclose(file);

#if defined(TARGET_OS_OSX) || defined(TARGET_OS_LINUX)
	SHA1_Final((unsigned char *)buffer, &ctx);
#elif defined(TARGET_OS_WINDOWS)
	success = CryptGetHashParam(crypto_hash, HP_HASHVAL, buffer, &hash_length, 0);

	if(!success)
	{
		r = ERROR_INTERNAL;
		goto error_crypt_get_hash_param;
	}
#endif

	for(int i = 0, j = 0; i < SHA_DIGEST_LENGTH; ++i, j += 2)
	{
		sprintf(&hash[j], "%02x", (unsigned char)buffer[i]);
	}

	snprintf(buffer, sizeof(buffer), "%s%s", get_path(), hash);

	error = rename(tmp_path, buffer);

	if(error != 0)
	{
		r = ERROR_DB_FILE_INSERT_COULD_NOT_CREATE_FILE;
		goto error_rename;
	}

	db_id_generate(id, &version, hash);

#if defined(TARGET_OS_WINDOWS)
error_crypt_get_hash_param:
error_crypt_hash_data:
    CryptDestroyHash(crypto_hash);
error_crypt_create_hash:
    CryptReleaseContext(crypto_provider, 0);
error_crypt_acquire_context:
#endif

error_rename:
error_fopen:

	return r;
}

void db_id_generate(char *id, unsigned int *version, const char *hash)
{
	memcpy(id, ID_HEADER, DB_ID_HEADER_SIZE);
	id += DB_ID_HEADER_SIZE;

	sprintf(id, "%04u", *version);
	id += DB_ID_VERSION_SIZE;

	memcpy(id, hash, DB_ID_HASH_SIZE);
}

enum Error db_id_parse(unsigned int *version, char *hash, const char *id)
{
	enum Error r = ERROR_NONE;
	char header[DB_ID_HEADER_SIZE] = { '\0' };

	memcpy(header, id, DB_ID_HEADER_SIZE);
	id += DB_ID_HEADER_SIZE;

	if(memcmp(header, ID_HEADER, DB_ID_HEADER_SIZE) != 0)
	{
		r = ERROR_DB_ID_PARSE_HEADER_INVALID;
		goto error_header_check;
	}

	if(version)
	{
		sscanf(id, "%04u", version);
	}

	id += DB_ID_VERSION_SIZE;

	if(hash)
	{
		memcpy(hash, id, DB_ID_HASH_SIZE);
	}

error_header_check:

	return r;
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

