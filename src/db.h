#ifndef DB_H
#define DB_H

#include <stdio.h>

#include "errors.h"

enum
{
	DB_VERSION = 1,

	DB_ID_HEADER_SIZE = 7,
	DB_ID_VERSION_SIZE = sizeof(unsigned int),
	DB_ID_HASH_SIZE = 40,

	DB_ID_SIZE = DB_ID_HEADER_SIZE
	           + DB_ID_VERSION_SIZE
	           + DB_ID_HASH_SIZE,
};

enum Error db_init(void);

enum Error db_file_query(FILE *output, char *id);
enum Error db_file_insert(char *id, FILE *input);

void db_id_generate(char *id, unsigned int *version, const char *hash);
enum Error db_id_parse(unsigned int *version, char *hash, const char *id);

#endif

