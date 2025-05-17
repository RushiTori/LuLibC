#ifndef LU_LOGS_H
#define LU_LOGS_H

#include <errno.h>
#include <stdio.h>
#include <string.h>

#define LogMessage(fmt, ...) fprintf(stderr, fmt __VA_OPT__(, ) __VA_ARGS__)

#define LogString(str) LogMessage("%s\n", str)

#define LogErrno(code) LogString(strerror(code))

#endif	// LU_LOGS_H
