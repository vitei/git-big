#ifndef ERROR_H
#define ERROR_H

enum Error
{
	kErrorNone,
	kErrorInternal,

	kErrorInitCorrupt,

	kErrorDBInitCouldNotCreateDirectory,

	kErrorPatternsInitCouldNotCreatePatternsFile,

	kErrorDBInsertFileCouldNotCreateFile,

	kErrorMax
};

extern const char *gErrorStrings[kErrorMax];

#endif

