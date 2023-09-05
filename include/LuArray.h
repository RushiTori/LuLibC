#ifndef L_ARRAY_H
#define L_ARRAY_H

#include "LuUtils.h"

typedef struct Array {
	void* data;
	void* end;

	uint dataSize;
	uint elementCount;

	// Will free the contained Arrays if clear_recursive is true
	bool containsArrays;
	// Will free the contained pointers if clear_recursive is true
	bool containsPointers;
} Array;

Array arr_Create(int dataSize, int initialCapacity, bool isDataArrays, bool isDataPointers);
void arr_Clear(Array* arr, bool clear_recursive);

void* arr_Get(Array* arr, uint index);
void arr_Set(Array* arr, uint index, const void* data_, uint count);

void arr_Push(Array* arr, const void* data_, uint count);
void arr_Insert(Array* arr, uint index, const void* data_, uint count);
void arr_Remove(Array* arr, uint index, uint count, bool clear_recursive);

uint arr_GetAllocatedSize(const Array* arr);
void arr_ShrinkToFit(Array* arr);
bool arr_Reserve(Array* arr, uint count);

bool arr_Equals(const Array* arr, const Array* other);

#endif	// L_ARRAY_H
