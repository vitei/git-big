#ifndef BOOL_H
#define BOOL_H

#if !defined(_MSC_VER)
	#include <stdbool.h>
#else
	typedef enum _bool
	{
		false = 0,
		true
	} bool;
#endif

#endif

