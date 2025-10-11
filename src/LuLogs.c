#include "LuLogs.h"

static FILE* logFile = NULL;
static bool shouldLog = true;

FILE* LuLogGetFile(void) {
	if (!logFile) logFile = stderr;
	return logFile;
}

void LuLogSetFile(FILE* file) {
	if (file) logFile = file;
}

bool LuLogGetVerbose(void) { return shouldLog; }

void LuLogSetVerbose(bool isVerbose) { shouldLog = isVerbose; }

#if defined(WINDOWS)

static void LogWindowsErrorValueBase(DWORD errCode) {
	// Search for the message in the system and allocate the buffer
	DWORD flags = FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER;

	LPCVOID source = NULL;	// No source since we search in the system

	DWORD langID = 0;  // Use user locale/system locale

	LPTSTR buffer = NULL;

	DWORD minBufferSize = 1024;

	va_list* args = NULL;  // We ignore the arguments with the flags already

	DWORD formatLen = FormatMessage(flags, source, errCode, langID, ((LPTSTR)(&buffer)), minBufferSize, args);
	if (formatLen == 0) {
		DWORD formatErr = GetLastError();
		if (formatErr == ERROR_RESOURCE_LANG_NOT_FOUND) {
			LogMessage("Couldn't find a suitable message for WSA Error %" PRIu64 " in your language.", ((uint64_t)(errCode)));
		} else if (formatErr == ERROR_MORE_DATA) {
			LogMessage("The error message for WSA Error %" PRIu64 " was too long to be allocated.", ((uint64_t)(errCode)));
		} else {
			LogMessage("Couldn't print the error message for WSA Error %" PRIu64, ((uint64_t)(errCode)));
		}
	}

	LogString(buffer);
	LocalFree(buffer);
}

void LogWSAErrorValue(DWORD errCode) { LogWindowsErrorValueBase(errCode); }

void LogWindowsErrorValue(DWORD errCode) { LogWindowsErrorValueBase(errCode); }

#endif	// Windows
