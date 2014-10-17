#ifndef DB_H
#define DB_H

#include <stdio.h>

#include "errors.h"

enum Error db_init(void);

enum Error db_file_query(char *id, FILE *output);
enum Error db_file_insert(FILE *input, char *id);

#endif
