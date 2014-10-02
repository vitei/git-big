#include "init.h"
#include "../db/init.h"
#include "../filters/init.h"
#include "../patterns/init.h"

int initRun(int argc, char *argv[])
{
	return    dbInit() == 0
	       && filtersInit() == 0
	       && patternsInit() == 0
	     ? 0
	     : 1;
}
