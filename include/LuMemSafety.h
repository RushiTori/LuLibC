#ifndef LU_MEM_SAFETY_H
#define LU_MEM_SAFETY_H

#include <stdbool.h>
#include <stdlib.h>

#include "LuLogs.h"

#define Free(ptr) free((void*)(ptr))

#define FreeAndReturn(ptr, ...) \
	{                           \
		Free(ptr);              \
		return __VA_ARGS__;     \
	}

#define Malloc(ptr, size)              \
	({                                 \
		const size_t size_ = size;     \
                                       \
		if (size_ == 0) {              \
			ptr = NULL;                \
			LogErrno(ENOBUFS);         \
		} else {                       \
			ptr = malloc(size_);       \
			if (!ptr) LogErrno(errno); \
		}                              \
                                       \
		ptr;                           \
	})

#define Calloc(ptr, count, size)         \
	({                                   \
		const size_t count_ = count;     \
		const size_t size_ = size;       \
                                         \
		if (count_ == 0 || size_ == 0) { \
			ptr = NULL;                  \
			LogErrno(ENOBUFS);           \
		} else {                         \
			ptr = calloc(count_, size_); \
			if (!ptr) LogErrno(errno);   \
		}                                \
                                         \
		ptr;                             \
	})

#define Realloc(ptr, size)                   \
	({                                       \
		const size_t size_ = size;           \
		bool success = true;                 \
                                             \
		if (size_ == 0) {                    \
			Free(ptr);                       \
			ptr = NULL;                      \
		} else {                             \
			void* res = realloc(ptr, size_); \
			if (!res) {                      \
				success = false;             \
				LogErrno(errno);             \
			} else {                         \
				ptr = res;                   \
			}                                \
		}                                    \
                                             \
		success;                             \
	})

#endif	// LU_MEM_SAFETY_H
