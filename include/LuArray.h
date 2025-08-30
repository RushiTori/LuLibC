#ifndef LU_ARRAY_H
#define LU_ARRAY_H

#include "LuUtils.h"

#define DeclareArrayType(type, name) \
	typedef struct name {            \
		type* data;                  \
		size_t size;                 \
		size_t capacity;             \
	} name

#define DeclareArrayMethods(type, name)                                            \
	name* name##Create(size_t capacity);                                           \
	void name##Clear(name* arr);                                                   \
	void name##Free(name* arr);                                                    \
                                                                                   \
	type* name##GetRef(const name* arr, size_t index);                             \
	type name##Get(const name* arr, size_t index);                                 \
	void name##Set(name* arr, size_t index, type data);                            \
                                                                                   \
	bool name##Push(name* arr, const type data);                                   \
	bool name##PushAll(name* arr, const type* data, size_t count);                 \
                                                                                   \
	bool name##Insert(name* arr, size_t index, const type data);                   \
	bool name##InsertAll(name* arr, size_t index, const type* data, size_t count); \
                                                                                   \
	bool name##Erase(name* arr, size_t index);                                     \
	bool name##EraseIfEqu(name* arr, const type data, CompareFunc func);           \
	bool name##EraseAll(name* arr, size_t index, size_t count);                    \
	bool name##EraseRange(name* arr, size_t start, size_t end);                    \
                                                                                   \
	bool name##ShrinkToFit(name* arr);                                             \
	bool name##Reserve(name* arr, size_t capacity);                                \
	bool name##Grow(name* container);                                              \
                                                                                   \
	void name##Map(name* arr, void (*func)(type*));                                \
	void name##Sort(name* arr, CompareFunc func);                                  \
                                                                                   \
	bool name##Contains(const name* arr, const type data, CompareFunc func);       \
	size_t name##IndexOf(const name* arr, const type data, CompareFunc func);      \
	size_t name##LastIndexOf(const name* arr, const type data, CompareFunc func)

#define DefineArrayMethods(type, name)                                                                \
	name* name##Create(size_t capacity) {                                                             \
		name* arr = NULL;                                                                             \
                                                                                                      \
		if (!Malloc(arr, sizeof(name))) return NULL;                                                  \
		if (capacity && !Calloc(arr->data, capacity, sizeof(type))) FreeAndReturn(arr, NULL);         \
                                                                                                      \
		arr->capacity = capacity;                                                                     \
		arr->size = 0;                                                                                \
                                                                                                      \
		return arr;                                                                                   \
	}                                                                                                 \
                                                                                                      \
	void name##Clear(name* arr) {                                                                     \
		Free(arr->data);                                                                              \
		*arr = (name){0};                                                                             \
	}                                                                                                 \
                                                                                                      \
	void name##Free(name* arr) {                                                                      \
		name##Clear(arr);                                                                             \
		Free(arr);                                                                                    \
	}                                                                                                 \
                                                                                                      \
	type* name##GetRef(const name* arr, size_t index) { return arr->data + index; }                   \
                                                                                                      \
	type name##Get(const name* arr, size_t index) { return arr->data[index]; }                        \
                                                                                                      \
	void name##Set(name* arr, size_t index, type data) { arr->data[index] = data; }                   \
                                                                                                      \
	bool name##Push(name* arr, const type data) {                                                     \
		if (arr->size == arr->capacity) {                                                             \
			if (!name##Grow(arr)) return false;                                                       \
		}                                                                                             \
                                                                                                      \
		arr->data[arr->size++] = data;                                                                \
                                                                                                      \
		return true;                                                                                  \
	}                                                                                                 \
                                                                                                      \
	bool name##PushAll(name* arr, const type* data, size_t count) {                                   \
		if ((arr->size + count) == arr->capacity) {                                                   \
			if (!name##Grow(arr)) return false;                                                       \
		}                                                                                             \
                                                                                                      \
		memcpy(arr->data + arr->size, data, count * sizeof(type));                                    \
		arr->size += count;                                                                           \
                                                                                                      \
		return true;                                                                                  \
	}                                                                                                 \
                                                                                                      \
	bool name##Insert(name* arr, size_t index, const type data) {                                     \
		if (index == arr->size) return name##Push(arr, data);                                         \
                                                                                                      \
		if (arr->size == arr->capacity) {                                                             \
			if (!name##Grow(arr)) return false;                                                       \
		}                                                                                             \
                                                                                                      \
		type* dataIdx = arr->data + index;                                                            \
		memcpy(dataIdx + 1, dataIdx, (arr->size - index) * sizeof(type));                             \
                                                                                                      \
		arr->data[index] = data;                                                                      \
                                                                                                      \
		arr->size++;                                                                                  \
                                                                                                      \
		return true;                                                                                  \
	}                                                                                                 \
                                                                                                      \
	bool name##InsertAll(name* arr, size_t index, const type* data, size_t count) {                   \
		if (index == arr->size) return name##PushAll(arr, data, count);                               \
                                                                                                      \
		if (arr->size == arr->capacity) {                                                             \
			if (!name##Grow(arr)) return false;                                                       \
		}                                                                                             \
                                                                                                      \
		type* dataIdx = arr->data + index;                                                            \
		memcpy(dataIdx + 1, dataIdx, (arr->size - index) * sizeof(type));                             \
                                                                                                      \
		memcpy(dataIdx, data, count * sizeof(type));                                                  \
		arr->size += count;                                                                           \
                                                                                                      \
		return true;                                                                                  \
	}                                                                                                 \
                                                                                                      \
	bool name##Erase(name* arr, size_t index) {                                                       \
		type* dataIdx = arr->data + index;                                                            \
		memcpy(dataIdx, dataIdx + 1, (arr->size - index - 1) * sizeof(type));                         \
		arr->size--;                                                                                  \
                                                                                                      \
		if (arr->size <= arr->capacity / 2) return name##ShrinkToFit(arr);                            \
                                                                                                      \
		return true;                                                                                  \
	}                                                                                                 \
                                                                                                      \
	bool name##EraseIfEqu(name* arr, const type data, CompareFunc func) {                             \
		for (int i = arr->size - 1; i >= 0; i--) {                                                    \
			if (func((const void*)&data, (const void*)(arr->data + i))) continue;                     \
                                                                                                      \
			type* dataIdx = arr->data + i;                                                            \
			memcpy(dataIdx, dataIdx + 1, (arr->size - i - 1) * sizeof(type));                         \
			arr->size--;                                                                              \
		}                                                                                             \
                                                                                                      \
		if (arr->size <= arr->capacity / 2) return name##ShrinkToFit(arr);                            \
                                                                                                      \
		return true;                                                                                  \
	}                                                                                                 \
                                                                                                      \
	bool name##EraseAll(name* arr, size_t index, size_t count) {                                      \
		type* dataIdx = arr->data + index;                                                            \
		memcpy(dataIdx, dataIdx + count, (arr->size - index - count) * sizeof(type));                 \
		arr->size -= count;                                                                           \
                                                                                                      \
		if (arr->size <= arr->capacity / 2) return name##ShrinkToFit(arr);                            \
                                                                                                      \
		return true;                                                                                  \
	}                                                                                                 \
                                                                                                      \
	bool name##EraseRange(name* arr, size_t start, size_t end) {                                      \
		size_t trueStart = Min(start, end);                                                           \
		size_t trueEnd = Max(start, end);                                                             \
		size_t count = trueEnd - trueStart;                                                           \
                                                                                                      \
		return name##EraseAll(arr, trueStart, count);                                                 \
	}                                                                                                 \
                                                                                                      \
	bool name##ShrinkToFit(name* arr) {                                                               \
		if (Realloc(arr->data, arr->size * sizeof(type))) {                                           \
			arr->capacity = arr->size;                                                                \
			return true;                                                                              \
		}                                                                                             \
                                                                                                      \
		return false;                                                                                 \
	}                                                                                                 \
                                                                                                      \
	bool name##Reserve(name* arr, size_t capacity) {                                                  \
		if (capacity <= arr->capacity) return true;                                                   \
                                                                                                      \
		size_t newCapa = arr->capacity * 2;                                                           \
		while (newCapa < capacity) newCapa *= 2;                                                      \
                                                                                                      \
		if (Realloc(arr->data, newCapa * sizeof(type))) {                                             \
			arr->capacity = newCapa;                                                                  \
			return true;                                                                              \
		}                                                                                             \
                                                                                                      \
		return false;                                                                                 \
	}                                                                                                 \
                                                                                                      \
	bool name##Grow(name* arr) {                                                                      \
		size_t newCapa = arr->capacity * 2;                                                           \
		if (!newCapa) newCapa = 8;                                                                    \
                                                                                                      \
		if (Realloc(arr->data, newCapa * sizeof(type))) {                                             \
			arr->capacity = newCapa;                                                                  \
			return true;                                                                              \
		}                                                                                             \
                                                                                                      \
		return false;                                                                                 \
	}                                                                                                 \
                                                                                                      \
	void name##Map(name* arr, void (*func)(type*)) {                                                  \
		for (size_t i = 0; i < arr->size; i++) {                                                      \
			func(arr->data + i);                                                                      \
		}                                                                                             \
	}                                                                                                 \
                                                                                                      \
	void name##Sort(name* arr, CompareFunc func) { qsort(arr->data, arr->size, sizeof(type), func); } \
                                                                                                      \
	bool name##Contains(const name* arr, const type data, CompareFunc func) {                         \
		for (size_t i = 0; i < arr->size; i++) {                                                      \
			if (!func(&data, arr->data + i)) return true;                                             \
		}                                                                                             \
                                                                                                      \
		return false;                                                                                 \
	}                                                                                                 \
                                                                                                      \
	size_t name##IndexOf(const name* arr, const type data, CompareFunc func) {                        \
		for (size_t i = 0; i < arr->size; i++) {                                                      \
			if (!func(&data, arr->data + i)) return i;                                                \
		}                                                                                             \
                                                                                                      \
		return arr->size;                                                                             \
	}                                                                                                 \
                                                                                                      \
	size_t name##LastIndexOf(const name* arr, const type data, CompareFunc func) {                    \
		for (int i = arr->size - 1; i >= 0; i--) {                                                    \
			if (!func(&data, arr->data + i)) return i;                                                \
		}                                                                                             \
                                                                                                      \
		return arr->size;                                                                             \
	}

#endif	// LU_ARRAY_H
