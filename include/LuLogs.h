#ifndef LU_LOGS_H
#define LU_LOGS_H

#include <stdio.h>
#include <string.h>

#define LOG_FILE stdout

#define __FILENAME__ (strrchr("/"__FILE__, '/') + 1)
#define __FILENAME_LENGTH__ \
	(strrchr(__FILENAME__, '.') ? (strrchr(__FILENAME__, '.') - __FILENAME__) : strlen(__FILENAME__))

#define LOG_CONTAINER_ERROR(error)                                                                                     \
	fprintf(LOG_FILE, "Error caught in module \"%.*s\" : %s in %s\n", (uint)__FILENAME_LENGTH__, __FILENAME__, #error, \
			__func__)

typedef enum ContainerError {
	NullContainer,
	NoDataSize,
	NullDataPassed,
	MemAllocationFail,
	MemReallocationFail,
	OutOfBounds,
	KeyAlreadyExists,
	KeyNotFound
} __attribute((packed)) ContainerError;

#endif	// LU_LOGS_H
