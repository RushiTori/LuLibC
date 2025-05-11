#ifndef LU_LOGS_H
#define LU_LOGS_H

#include <errno.h>
#include <stdio.h>
#include <string.h>

#define __FILENAME__ (strrchr("/"__FILE__, '/') + 1)
#define __FILENAME_LENGTH__                                                   \
	(strrchr(__FILENAME__, '.') ? (strrchr(__FILENAME__, '.') - __FILENAME__) \
								: strlen(__FILENAME__))

#define LogMessage(fmt, ...) fprintf(stderr, fmt __VA_OPT__(, ) __VA_ARGS__)

#define LogString(str) LogMessage("%s\n", str)

#define LogErrno(code) LogString(strerror(code))

#endif	// LU_LOGS_H
