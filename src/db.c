#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#if defined(__APPLE__)
	#include <CommonCrypto/CommonDigest.h>
#elif defined(__linux)
	#include <openssl/sha.h>
#elif defined(_WIN32)
	#include <windows.h>
	#include <Wincrypt.h>
#else
	#error Unsupported platform
#endif

#include "db.h"
#include "repo.h"

#if defined(__APPLE__)
	#define SHA_DIGEST_LENGTH CC_SHA1_DIGEST_LENGTH
#elif defined(_WIN32)
	#define SHA_DIGEST_LENGTH 20
#endif

static const char * const DB_DIRECTORY = "big";
static const char ID_HEADER[DB_ID_HEADER_SIZE] = { 'G', 'I', 'T',
                                                   '-',
                                                   'B', 'I', 'G' };

static const char *get_path(void);
static void get_file_dir(char *const path, size_t path_size, char *hash);
static void get_file_path(char *const path, size_t path_size, char *hash);

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

	get_file_path(path, path_length, hash);

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

	get_file_path(buffer, sizeof(buffer), hash);

	file = fopen(buffer, "rb");

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
	int i = 0;
	int j = 0;

#if defined(__APPLE__)
	CC_SHA1_CTX ctx;
#elif defined(__linux)
	SHA_CTX ctx;
#elif defined(_WIN32)
	BOOL success;
	HCRYPTPROV crypto_provider = 0;
	HCRYPTHASH crypto_hash = 0;
	DWORD hash_length = SHA_DIGEST_LENGTH;
#endif

#if defined(__APPLE__)
	CC_SHA1_Init(&ctx);
#elif defined(__linux)
	SHA1_Init(&ctx);
#elif defined(_WIN32)
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

#if defined(__APPLE__)
		CC_SHA1_Update(&ctx, (unsigned char *)buffer, size);
#elif defined(__linux)
		SHA1_Update(&ctx, (unsigned char *)buffer, size);
#elif defined(_WIN32)
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

#if defined(__APPLE__)
	CC_SHA1_Final((unsigned char *)buffer, &ctx);
#elif defined(__linux)
	SHA1_Final((unsigned char *)buffer, &ctx);
#elif defined(_WIN32)
	success = CryptGetHashParam(crypto_hash, HP_HASHVAL, buffer, &hash_length, 0);

	if(!success)
	{
		r = ERROR_INTERNAL;
		goto error_crypt_get_hash_param;
	}
#endif

	for(i = 0, j = 0; i < SHA_DIGEST_LENGTH; ++i, j += 2)
	{
		sprintf(&hash[j], "%02x", (unsigned char)buffer[i]);
	}

	get_file_dir(buffer, sizeof(buffer), hash);
	error = mkdir(buffer, 0777);

	if(error != 0 && errno != EEXIST)
	{
		r = ERROR_DB_FILE_INSERT_COULD_NOT_CREATE_DIRECTORY;
		goto error_mkdir;
	}

	get_file_path(buffer, sizeof(buffer), hash);

	error = rename(tmp_path, buffer);

	if(error != 0)
	{
		r = ERROR_DB_FILE_INSERT_COULD_NOT_CREATE_FILE;
		goto error_rename;
	}

	db_id_generate(id, &version, hash);

error_rename:
error_mkdir:

#if defined(_WIN32)
error_crypt_get_hash_param:
error_crypt_hash_data:
    CryptDestroyHash(crypto_hash);
error_crypt_create_hash:
    CryptReleaseContext(crypto_provider, 0);
error_crypt_acquire_context:
#endif

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

static void get_file_dir(char *const path, size_t path_size, char *hash)
{
	char hash_prefix[3];

	hash_prefix[0] = hash[0];
	hash_prefix[1] = hash[1];
	hash_prefix[2] = '\0';

	snprintf(path, path_size, "%s%s/", get_path(), hash_prefix); // FIXME: buffer overflow
}

static void get_file_path(char *const path, size_t path_size, char *hash)
{
	char buffer[1024] = { '\0' };

	get_file_dir(buffer, sizeof(buffer), hash);

	hash = &hash[2];

	snprintf(path, path_size, "%s%s", buffer, hash); // FIXME: buffer overflow
}

