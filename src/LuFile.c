#include "LuFile.h"

static LuFileEndian GetSystemEndian(void) {
	union {
		uint16_t num;
		char bytes[sizeof(uint16_t)];
	} a = {.num = 1};
	return (a.bytes[0] == 1) ? LU_LITTLE_ENDIAN : LU_BIG_ENDIAN;
}

#define ShouldSwap(endian) (((endian) != LU_SYSTEM_ENDIAN) && ((endian) != GetSystemEndian()))

#define SwapBase(a, b)       \
	{                        \
		tmp = bytes[a];      \
		bytes[a] = bytes[b]; \
		bytes[b] = tmp;      \
	}

static void Swap16(void* data) {
	char* bytes = data;
	char tmp;
	SwapBase(0, 1);
}

static void Swap32(void* data) {
	char* bytes = data;
	char tmp;
	SwapBase(0, 3);
	SwapBase(1, 2);
}

static void Swap64(void* data) {
	char* bytes = data;
	char tmp;
	SwapBase(0, 7);
	SwapBase(1, 6);
	SwapBase(2, 5);
	SwapBase(3, 4);
}

FILE* TryOpenFile(const char* path, const char* flags) {
	FILE* f = fopen(path, flags);
	if (!f) LogMessage("%s (%s) : %s\n", path, flags, strerror(errno));
	return f;
}

int64_t LuFileGetLine(char** strRef, size_t* n, FILE* stream) { return LuFileGetDelim(strRef, n, '\n', stream); }

#define GET_DELIM_ALLOC_SIZE (64 * sizeof(char))

int64_t LuFileGetDelim(char** strRef, size_t* n, char delim, FILE* stream) {
	if (!strRef || !n) {
		errno = EINVAL;
		return LU_FILE_ERROR;
	}

	size_t capacity = *n;

	char* ptr = *strRef;
	if (!ptr) {
		if (!Malloc(ptr, GET_DELIM_ALLOC_SIZE)) return LU_FILE_ERROR;
		capacity = GET_DELIM_ALLOC_SIZE;
	}
	*ptr = '\0';

	size_t len = 0;
	while (true) {
		int tmp = fgetc(stream);
		if (tmp == EOF) return LU_FILE_ERROR;

		if ((len + 1) >= capacity) {
			if (!Realloc(ptr, capacity + GET_DELIM_ALLOC_SIZE)) FreeAndReturn(ptr, LU_FILE_ERROR);
			capacity += GET_DELIM_ALLOC_SIZE;
		}

		ptr[len++] = tmp;
		if (tmp == delim) break;
	}
	ptr[len] = '\0';

	*n = capacity;
	*strRef = ptr;
	return len - 1;
}

// LuFile Read functions

static int64_t LuFileReadSString(FILE* stream, char** strRef, size_t* n, size_t len) {
	if (!strRef || !n) {
		errno = EINVAL;
		return LU_FILE_ERROR;
	}

	size_t capacity = *n;

	char* ptr = *strRef;
	if (!ptr) {
		if (!Malloc(ptr, sizeof(char) * len)) return LU_FILE_ERROR;
		capacity = len;
	}
	*ptr = '\0';

	if (capacity < len) {
		if (!Realloc(ptr, sizeof(char) * len)) FreeAndReturn(ptr, LU_FILE_ERROR);
	}

	size_t readLen = fread(ptr, sizeof(char), len, stream);
	ptr[readLen] = '\0';

	*n = capacity;
	*strRef = ptr;

	if (readLen != len) {
		if (ferror(stream)) LogMessage("While reading string : %s", strerror(errno));
		return -1;
	}

	return len - 1;
}

int64_t LuFileReadSString8(FILE* stream, char** strRef, size_t* n) {
	uint8_t len = 0;
	if (!LuFileReadUInt8(stream, &len)) return LU_FILE_ERROR;
	return LuFileReadSString(stream, strRef, n, len);
}

int64_t LuFileReadSString16(FILE* stream, char** strRef, size_t* n) {
	uint16_t len = 0;
	if (!LuFileReadUInt16(stream, &len)) return LU_FILE_ERROR;
	return LuFileReadSString(stream, strRef, n, len);
}

int64_t LuFileReadSString32(FILE* stream, char** strRef, size_t* n) {
	uint32_t len = 0;
	if (!LuFileReadUInt32(stream, &len)) return LU_FILE_ERROR;
	return LuFileReadSString(stream, strRef, n, len);
}

int64_t LuFileReadSString64(FILE* stream, char** strRef, size_t* n) {
	uint64_t len = 0;
	if (!LuFileReadUInt64(stream, &len)) return LU_FILE_ERROR;
	return LuFileReadSString(stream, strRef, n, len);
}

int64_t LuFileReadSString16Ex(FILE* stream, char** strRef, size_t* n, LuFileEndian endian) {
	uint16_t len = 0;
	if (!LuFileReadUInt16Ex(stream, &len, endian)) return LU_FILE_ERROR;
	return LuFileReadSString(stream, strRef, n, len);
}

int64_t LuFileReadSString32Ex(FILE* stream, char** strRef, size_t* n, LuFileEndian endian) {
	uint32_t len = 0;
	if (!LuFileReadUInt32Ex(stream, &len, endian)) return LU_FILE_ERROR;
	return LuFileReadSString(stream, strRef, n, len);
}

int64_t LuFileReadSString64Ex(FILE* stream, char** strRef, size_t* n, LuFileEndian endian) {
	uint64_t len = 0;
	if (!LuFileReadUInt64Ex(stream, &len, endian)) return LU_FILE_ERROR;
	return LuFileReadSString(stream, strRef, n, len);
}

#define ReadBase(type)                                                                     \
	{                                                                                      \
		if (fread(buf, sizeof(type), 1, stream) == 1) return true;                         \
		if (ferror(stream)) LogMessage("While reading " #type " : %s\n", strerror(errno)); \
		return false;                                                                      \
	}

bool LuFileReadBool(FILE* stream, bool* buf) { ReadBase(bool); }
bool LuFileReadInt8(FILE* stream, int8_t* buf) { ReadBase(int8_t); }
bool LuFileReadInt16(FILE* stream, int16_t* buf) { ReadBase(int16_t); }
bool LuFileReadInt32(FILE* stream, int32_t* buf) { ReadBase(int32_t); }
bool LuFileReadInt64(FILE* stream, int64_t* buf) { ReadBase(int64_t); }
bool LuFileReadUInt8(FILE* stream, uint8_t* buf) { ReadBase(uint8_t); }
bool LuFileReadUInt16(FILE* stream, uint16_t* buf) { ReadBase(uint16_t); }
bool LuFileReadUInt32(FILE* stream, uint32_t* buf) { ReadBase(uint32_t); }
bool LuFileReadUInt64(FILE* stream, uint64_t* buf) { ReadBase(uint64_t); }
bool LuFileReadFloat(FILE* stream, float* buf) { ReadBase(float); }
bool LuFileReadDouble(FILE* stream, double* buf) { ReadBase(double); }

#define ReadExBase(type, swap)                                                                     \
	{                                                                                              \
		bool res = false;                                                                          \
		if (fread(buf, sizeof(type), 1, stream) == 1) res = true;                                  \
		if (!res && ferror(stream)) LogMessage("While reading " #type " : %s\n", strerror(errno)); \
		if (ShouldSwap(endian)) swap(buf);                                                         \
		return false;                                                                              \
	}

bool LuFileReadInt16Ex(FILE* stream, int16_t* buf, LuFileEndian endian) { ReadExBase(int16_t, Swap16); }
bool LuFileReadInt32Ex(FILE* stream, int32_t* buf, LuFileEndian endian) { ReadExBase(int32_t, Swap32); }
bool LuFileReadInt64Ex(FILE* stream, int64_t* buf, LuFileEndian endian) { ReadExBase(int64_t, Swap64); }
bool LuFileReadUInt16Ex(FILE* stream, uint16_t* buf, LuFileEndian endian) { ReadExBase(uint16_t, Swap16); }
bool LuFileReadUInt32Ex(FILE* stream, uint32_t* buf, LuFileEndian endian) { ReadExBase(uint32_t, Swap32); }
bool LuFileReadUInt64Ex(FILE* stream, uint64_t* buf, LuFileEndian endian) { ReadExBase(uint64_t, Swap64); }
bool LuFileReadFloatEx(FILE* stream, float* buf, LuFileEndian endian) { ReadExBase(float, Swap32); }
bool LuFileReadDoubleEx(FILE* stream, double* buf, LuFileEndian endian) { ReadExBase(double, Swap64); }

// LuFile Write functions

#define WriteStringBase()                                                       \
	{                                                                           \
		if (LuFileWriteRaw(str, sizeof(char), len, stream) == len) return true; \
		LogMessage("While writing string : %s\n", strerror(errno));             \
		return false;                                                           \
	}

bool LuFileWriteCString(FILE* stream, const char* str) {
	size_t len = strlen(str) + 1;
	WriteStringBase();
}

#define WriteSStringBase(type, call)          \
	{                                         \
		type len = strlen(str);               \
		if (!call(stream, len)) return false; \
		WriteStringBase();                    \
	}

bool LuFileWriteSString8(FILE* stream, const char* str) { WriteSStringBase(uint8_t, LuFileWriteUInt8); }
bool LuFileWriteSString16(FILE* stream, const char* str) { WriteSStringBase(uint16_t, LuFileWriteUInt16); }
bool LuFileWriteSString32(FILE* stream, const char* str) { WriteSStringBase(uint32_t, LuFileWriteUInt32); }
bool LuFileWriteSString64(FILE* stream, const char* str) { WriteSStringBase(uint64_t, LuFileWriteUInt64); }

#define WriteSStringExBase(type, call)                \
	{                                                 \
		type len = strlen(str);                       \
		if (!call(stream, len, endian)) return false; \
		WriteStringBase();                            \
	}

bool LuFileWriteSString16Ex(FILE* stream, const char* str, LuFileEndian endian) { WriteSStringExBase(uint16_t, LuFileWriteUInt16Ex); }
bool LuFileWriteSString32Ex(FILE* stream, const char* str, LuFileEndian endian) { WriteSStringExBase(uint32_t, LuFileWriteUInt32Ex); }
bool LuFileWriteSString64Ex(FILE* stream, const char* str, LuFileEndian endian) { WriteSStringExBase(uint64_t, LuFileWriteUInt64Ex); }

#define WriteBase(type)                                                \
	{                                                                  \
		if (fwrite(&value, sizeof(type), 1, stream) == 1) return true; \
		LogMessage("While writing " #type " : %s\n", strerror(errno)); \
		return false;                                                  \
	}

bool LuFileWriteBool(FILE* stream, bool value) { WriteBase(bool); }
bool LuFileWriteInt8(FILE* stream, int8_t value) { WriteBase(int8_t); }
bool LuFileWriteInt16(FILE* stream, int16_t value) { WriteBase(int16_t); }
bool LuFileWriteInt32(FILE* stream, int32_t value) { WriteBase(int32_t); }
bool LuFileWriteInt64(FILE* stream, int64_t value) { WriteBase(int64_t); }
bool LuFileWriteUInt8(FILE* stream, uint8_t value) { WriteBase(uint8_t); }
bool LuFileWriteUInt16(FILE* stream, uint16_t value) { WriteBase(uint16_t); }
bool LuFileWriteUInt32(FILE* stream, uint32_t value) { WriteBase(uint32_t); }
bool LuFileWriteUInt64(FILE* stream, uint64_t value) { WriteBase(uint64_t); }
bool LuFileWriteFloat(FILE* stream, float value) { WriteBase(float); }
bool LuFileWriteDouble(FILE* stream, double value) { WriteBase(double); }

#define WriteExBase(type, swap)                                        \
	{                                                                  \
		if (ShouldSwap(endian)) swap(&value);                          \
		if (fwrite(&value, sizeof(type), 1, stream) == 1) return true; \
		LogMessage("While writing " #type " : %s\n", strerror(errno)); \
		return false;                                                  \
	}

bool LuFileWriteInt16Ex(FILE* stream, int16_t value, LuFileEndian endian) { WriteExBase(int16_t, Swap16); }
bool LuFileWriteInt32Ex(FILE* stream, int32_t value, LuFileEndian endian) { WriteExBase(int32_t, Swap32); }
bool LuFileWriteInt64Ex(FILE* stream, int64_t value, LuFileEndian endian) { WriteExBase(int64_t, Swap64); }
bool LuFileWriteUInt16Ex(FILE* stream, uint16_t value, LuFileEndian endian) { WriteExBase(uint16_t, Swap16); }
bool LuFileWriteUInt32Ex(FILE* stream, uint32_t value, LuFileEndian endian) { WriteExBase(uint32_t, Swap32); }
bool LuFileWriteUInt64Ex(FILE* stream, uint64_t value, LuFileEndian endian) { WriteExBase(uint64_t, Swap64); }
bool LuFileWriteFloatEx(FILE* stream, float value, LuFileEndian endian) { WriteExBase(float, Swap32); }
bool LuFileWriteDoubleEx(FILE* stream, double value, LuFileEndian endian) { WriteExBase(double, Swap64); }
