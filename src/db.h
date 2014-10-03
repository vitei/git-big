#ifndef DB_H
#define DB_H

#include <stdio.h>

#include "errors.h"

enum Error dbInit(void);

enum Error dbQueryFile(char *id, FILE *output);
enum Error dbInsertFile(FILE *access, char *id);

#endif
