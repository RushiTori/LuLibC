#ifndef LU_LOGS_H
#define LU_LOGS_H

#include <errno.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "LuPlatforms.h"

#if defined(WINDOWS)

// Used by LuSocket.h
#include <winsock2.h>

// All purpose
#include <windows.h>

#endif	// Windows

// Default: stderr.
FILE* LuLogGetFile(void);

// Won't set the log file if "file" is NULL
void LuLogSetFile(FILE* file);

// Default: true
bool LuLogGetVerbose(void);

// Sets if the LuLog module should output or not
void LuLogSetVerbose(bool isVerbose);

#define LogMessage(fmt, ...)                                                            \
	{                                                                                   \
		if (LuLogGetVerbose()) fprintf(LuLogGetFile(), fmt __VA_OPT__(, ) __VA_ARGS__); \
	}

#define LogString(str) LogMessage("%s\n", str)

#define LogStringAndReturn(str, retVal) \
	{                                   \
		LogString(str);                 \
		return retVal;                  \
	}

#define LogStringAndExit(str, exitCode) \
	{                                   \
		LogString(str);                 \
		exit(exitCode);                 \
	}

#define LogErrnoValue(errCode) LogString(strerror(errCode))

#define LogErrnoValueAndReturn(errCode, retVal) \
	{                                           \
		LogErrnoValue(errCode);                 \
		return retVal;                          \
	}

#define LogErrnoValueAndExit(errCode, exitCode) \
	{                                           \
		LogErrnoValue(errCode);                 \
		exit(exitCode);                         \
	}

#define LogErrno() LogErrnoValue(errno)

#define LogErrnoAndReturn(retVal) \
	{                             \
		LogErrno();               \
		return retVal;            \
	}

#define LogErrnoAndExit(exitCode) \
	{                             \
		LogErrno();               \
		exit(exitCode);           \
	}

#if defined(WINDOWS)

void LogWSAErrorValue(DWORD errCode);

#define LogWSAErrorValueAndReturn(errCode, retVal) \
	{                                              \
		LogWSAErrorValue(errCode);                 \
		return retVal;                             \
	}

#define LogWSAErrorValueAndExit(errCode, exitCode) \
	{                                              \
		LogWSAErrorValue(errCode);                 \
		exit(exitCode);                            \
	}

#define LogWSAError() LogWSAErrorValue(WSAGetLastError())

#define LogWSAErrorAndReturn(retVal) \
	{                                \
		LogWSAError();               \
		return retVal;               \
	}

#define LogWSAErrorAndExit(exitCode) \
	{                                \
		LogWSAError();               \
		exit(exitCode);              \
	}

void LogWindowsErrorValue(DWORD errCode);

#define LogWindowsErrorValueAndReturn(errCode, retVal) \
	{                                                  \
		LogWindowsErrorValue(errCode);                 \
		return retVal;                                 \
	}

#define LogWindowsErrorValueAndExit(errCode, exitCode) \
	{                                                  \
		LogWindowsErrorValue(errCode);                 \
		exit(exitCode);                                \
	}

#define LogWindowsError() LogWindowsErrorValue(GetLastError())

#define LogWindowsErrorAndReturn(retVal) \
	{                                    \
		LogWindowsError();               \
		return retVal;                   \
	}

#define LogWindowsErrorAndExit(exitCode) \
	{                                    \
		LogWindowsError();               \
		exit(exitCode);                  \
	}

#endif	// Windows

#endif	// LU_LOGS_H
