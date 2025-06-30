#ifndef LU_FILE_H
#define LU_FILE_H

#include "LuUtils.h"

typedef enum LuFileEndian {
	LU_BIG_ENDIAN = 0,
	LU_LITTLE_ENDIAN = 1,
	LU_SYSTEM_ENDIAN = 2,
} LuFileEndian;

#define LU_FILE_ERROR -1

FILE* TryOpenFile(const char* path, const char* flags);
#define CloseFile fclose

int64_t LuFileGetLine(char** strRef, size_t* n, FILE* stream);
int64_t LuFileGetDelim(char** strRef, size_t* n, char delim, FILE* stream);

// LuFile Read functions

#define LuFileReadRaw fread

#define LuFileReadCString(stream, strRef, n) LuFileGetDelim(strRef, n, '\0', stream)

int64_t LuFileReadSString8(FILE* stream, char** strRef, size_t* n);
int64_t LuFileReadSString16(FILE* stream, char** strRef, size_t* n);
int64_t LuFileReadSString32(FILE* stream, char** strRef, size_t* n);
int64_t LuFileReadSString64(FILE* stream, char** strRef, size_t* n);

int64_t LuFileReadSString16Ex(FILE* stream, char** strRef, size_t* n, LuFileEndian endian);
int64_t LuFileReadSString32Ex(FILE* stream, char** strRef, size_t* n, LuFileEndian endian);
int64_t LuFileReadSString64Ex(FILE* stream, char** strRef, size_t* n, LuFileEndian endian);

bool LuFileReadBool(FILE* stream, bool* buf);

bool LuFileReadInt8(FILE* stream, int8_t* buf);
bool LuFileReadInt16(FILE* stream, int16_t* buf);
bool LuFileReadInt32(FILE* stream, int32_t* buf);
bool LuFileReadInt64(FILE* stream, int64_t* buf);

bool LuFileReadUInt8(FILE* stream, uint8_t* buf);
bool LuFileReadUInt16(FILE* stream, uint16_t* buf);
bool LuFileReadUInt32(FILE* stream, uint32_t* buf);
bool LuFileReadUInt64(FILE* stream, uint64_t* buf);

bool LuFileReadFloat(FILE* stream, float* buf);
bool LuFileReadDouble(FILE* stream, double* buf);

bool LuFileReadInt16Ex(FILE* stream, int16_t* buf, LuFileEndian endian);
bool LuFileReadInt32Ex(FILE* stream, int32_t* buf, LuFileEndian endian);
bool LuFileReadInt64Ex(FILE* stream, int64_t* buf, LuFileEndian endian);

bool LuFileReadUInt16Ex(FILE* stream, uint16_t* buf, LuFileEndian endian);
bool LuFileReadUInt32Ex(FILE* stream, uint32_t* buf, LuFileEndian endian);
bool LuFileReadUInt64Ex(FILE* stream, uint64_t* buf, LuFileEndian endian);

bool LuFileReadFloatEx(FILE* stream, float* buf, LuFileEndian endian);
bool LuFileReadDoubleEx(FILE* stream, double* buf, LuFileEndian endian);

#define LuFileReadByte LuFileReadUInt8

#define LuFileReadChar LuFileReadInt8
#define LuFileReadShort LuFileReadInt16
#define LuFileReadInt LuFileReadInt32
#define LuFileReadLong LuFileReadInt64

#define LuFileReadUChar LuFileReadUInt8
#define LuFileReadUShort LuFileReadUInt16
#define LuFileReadUInt LuFileReadUInt32
#define LuFileReadULong LuFileReadUInt64

#define LuFileReadShortEx LuFileReadInt16Ex
#define LuFileReadIntEx LuFileReadInt32Ex
#define LuFileReadLongEx LuFileReadInt64Ex

#define LuFileReadUShortEx LuFileReadUInt16Ex
#define LuFileReadUIntEx LuFileReadUInt32Ex
#define LuFileReadULongEx LuFileReadUInt64Ex

// LuFile Write functions

#define LuFileWriteRaw fwrite

bool LuFileWriteCString(FILE* stream, const char* strRef);

bool LuFileWriteSString8(FILE* stream, const char* strRef);
bool LuFileWriteSString16(FILE* stream, const char* strRef);
bool LuFileWriteSString32(FILE* stream, const char* strRef);
bool LuFileWriteSString64(FILE* stream, const char* strRef);

bool LuFileWriteSString16Ex(FILE* stream, const char* strRef, LuFileEndian endian);
bool LuFileWriteSString32Ex(FILE* stream, const char* strRef, LuFileEndian endian);
bool LuFileWriteSString64Ex(FILE* stream, const char* strRef, LuFileEndian endian);

bool LuFileWriteBool(FILE* stream, bool value);

bool LuFileWriteInt8(FILE* stream, int8_t value);
bool LuFileWriteInt16(FILE* stream, int16_t value);
bool LuFileWriteInt32(FILE* stream, int32_t value);
bool LuFileWriteInt64(FILE* stream, int64_t value);

bool LuFileWriteUInt8(FILE* stream, uint8_t value);
bool LuFileWriteUInt16(FILE* stream, uint16_t value);
bool LuFileWriteUInt32(FILE* stream, uint32_t value);
bool LuFileWriteUInt64(FILE* stream, uint64_t value);

bool LuFileWriteFloat(FILE* stream, float value);
bool LuFileWriteDouble(FILE* stream, double value);

bool LuFileWriteInt16Ex(FILE* stream, int16_t value, LuFileEndian endian);
bool LuFileWriteInt32Ex(FILE* stream, int32_t value, LuFileEndian endian);
bool LuFileWriteInt64Ex(FILE* stream, int64_t value, LuFileEndian endian);

bool LuFileWriteUInt16Ex(FILE* stream, uint16_t value, LuFileEndian endian);
bool LuFileWriteUInt32Ex(FILE* stream, uint32_t value, LuFileEndian endian);
bool LuFileWriteUInt64Ex(FILE* stream, uint64_t value, LuFileEndian endian);

bool LuFileWriteFloatEx(FILE* stream, float value, LuFileEndian endian);
bool LuFileWriteDoubleEx(FILE* stream, double value, LuFileEndian endian);

#define LuFileWriteByte LuFileWriteUInt8

#define LuFileWriteChar LuFileWriteInt8
#define LuFileWriteShort LuFileWriteInt16
#define LuFileWriteInt LuFileWriteInt32
#define LuFileWriteLong LuFileWriteInt64

#define LuFileWriteUChar LuFileWriteUInt8
#define LuFileWriteUShort LuFileWriteUInt16
#define LuFileWriteUInt LuFileWriteUInt32
#define LuFileWriteULong LuFileWriteUInt64

#define LuFileWriteShortEx LuFileWriteInt16Ex
#define LuFileWriteIntEx LuFileWriteInt32Ex
#define LuFileWriteLongEx LuFileWriteInt64Ex

#define LuFileWriteUShortEx LuFileWriteUInt16Ex
#define LuFileWriteUIntEx LuFileWriteUInt32Ex
#define LuFileWriteULongEx LuFileWriteUInt64Ex

#endif	// LU_FILE_H
