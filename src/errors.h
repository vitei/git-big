#ifndef ERROR_H
#define ERROR_H

enum Error
{
	kErrorNone,
	kErrorInternal,

	kErrorInitCorrupt,

	kErrorDBInitCouldNotCreateDirectory,

	kErrorMax
};

extern const char *gErrorStrings[kErrorMax];

#endif

