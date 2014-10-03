#ifndef ERROR_H
#define ERROR_H

enum Error
{
	kErrorNone,
	kErrorInternal,

	kErrorInitCorrupt,

	kErrorDBInitCouldNotCreateDirectory,

	kErrorPatternsInitCouldNotCreatePatternsFile,

	kErrorDBQueryFileCouldNotFindFile,
	kErrorDBInsertFileCouldNotCreateFile,

	kErrorMax
};

extern const char *gErrorStrings[kErrorMax];

#endif

