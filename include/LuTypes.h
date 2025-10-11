#ifndef LU_TYPES
#define LU_TYPES

#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>

#include "LuPlatforms.h"

typedef uint8_t byte;

typedef uint16_t LuAngle;

typedef int32_t (*CompareFunc)(const void*, const void*);

#if defined(LINUX)
typedef int64_t SocketType;
#define LU_SOCKET_INVALID_SOCKET ((SocketType)(-1))
#endif	// Linux

#if defined(WINDOWS)
typedef SOCKET SocketType;
#define LU_SOCKET_INVALID_SOCKET ((SocketType)(INVALID_SOCKET))
#endif	// Windows

#endif	// LU_TYPES
