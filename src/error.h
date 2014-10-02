#ifndef ERROR_H
#define ERROR_H

enum Error
{
	kErrorNone,

	kErrorInitCorrupt,

	kErrorMax
};

extern const char *gErrorStrings[kErrorMax];

#endif

