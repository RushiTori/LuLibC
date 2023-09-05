#include <LuArray.h>

enum ARR_ErrorCodes {
	ARR_ArrIsNullptr = 0,
	ARR_OutOfBounds,
	ARR_EmptyDataSize,
	ARR_NoDataPassed,
	ARR_AllocFail,
	ARR_ReAllocFail,
	ARR_ErrorCount
};

static const string ARR_ErrorMessages[ARR_ErrorCount] = {
	"Array Passed Is Nullptr",
	"Array Out Of Bounds Exception",
	"Size Of Element Is 0 (Zero)",
	"Trying To Push/Insert/Remove Nullptr/Empty Memory",
	"Array Allocation (malloc/calloc) Failed",
	"Array Re-Allocation (realloc) Failed",
};

#define PRINT_ARR_ERROR(arr_err) fprintf(stderr, "Array Error %d: %s\n", arr_err, ARR_ErrorMessages[arr_err])

Array arr_Create(int dataSize, int initialCapacity, bool isDataArrays, bool isDataPointers) {
	Array temp;
	temp.data = NULL;
	temp.end = NULL;
	temp.dataSize = dataSize;
	temp.elementCount = 0;
	temp.containsArrays = isDataArrays;
	temp.containsPointers = isDataPointers;

	if (!dataSize) {
		PRINT_ARR_ERROR(ARR_EmptyDataSize);
		return temp;
	}

	if (!initialCapacity) return temp;

	temp.data = calloc(initialCapacity, dataSize);
	if (!temp.data) {
		PRINT_ARR_ERROR(ARR_AllocFail);
		return temp;
	}
	temp.end = ((char*)temp.data) + (initialCapacity * dataSize);
	return temp;
}

void arr_Clear(Array* arr, bool clear_recursive) {
	// WIP
	if (!arr) {
		PRINT_ARR_ERROR(ARR_ArrIsNullptr);
		return;
	}

	if (!arr->dataSize) {
		PRINT_ARR_ERROR(ARR_EmptyDataSize);
		return;
	}

	if (!arr->data || !arr->elementCount) return;

	if (clear_recursive) {
		if (arr->containsArrays) {
			if (arr->containsPointers) {
				for (uint i = 0; i < arr->elementCount; i++) {
					arr_Clear(((Array**)arr->data)[i], clear_recursive);
				}
			} else {
				for (uint i = 0; i < arr->elementCount; i++) {
					arr_Clear(&((Array*)arr->data)[i], clear_recursive);
				}
			}
		}

		if (arr->containsPointers) {
			for (uint i = 0; i < arr->elementCount; i++) {
				free(((void**)arr->data)[i]);
			}
		}
	}

	free(arr->data);
	arr->data = NULL;
	arr->end = NULL;
	arr->elementCount = 0;
}

void* arr_Get(Array* arr, uint index) {
	if (!arr) {
		PRINT_ARR_ERROR(ARR_ArrIsNullptr);
		return NULL;
	}

	if (!arr->dataSize) {
		PRINT_ARR_ERROR(ARR_EmptyDataSize);
		return NULL;
	}

	if (!arr->data || index >= arr->elementCount) {
		PRINT_ARR_ERROR(ARR_OutOfBounds);
		return NULL;
	}

	return ((char*)arr->data) + (index * arr->dataSize);
}

void arr_Set(Array* arr, uint index, const void* data_, uint count) {
	void* temp = arr_Get(arr, index);
	if (!temp) return;

	if (!data_ || !count) {
		PRINT_ARR_ERROR(ARR_NoDataPassed);
		return;
	}

	if ((index + count) > arr->elementCount) {
		PRINT_ARR_ERROR(ARR_OutOfBounds);
		return;
	}

	memcpy(temp, data_, count * arr->dataSize);
}

void arr_Push(Array* arr, const void* data_, uint count) {
	if (!arr) {
		PRINT_ARR_ERROR(ARR_ArrIsNullptr);
		return;
	}

	if (!data_ || !count) {
		PRINT_ARR_ERROR(ARR_NoDataPassed);
		return;
	}

	uint data_len = count * arr->dataSize;

	if (!arr->data) {
		arr->data = malloc(data_len);
		if (!arr->data) {
			PRINT_ARR_ERROR(ARR_AllocFail);
			return;
		}
		memcpy(arr->data, data_, data_len);
		arr->end = ((char*)arr->data) + data_len;
		arr->elementCount = count;
		return;
	}

	uint usedSize = arr->elementCount * arr->dataSize;
	uint maxSize = arr_GetAllocatedSize(arr);

	if ((usedSize + data_len) > maxSize) {
		uint newSize = maxSize * 2;
		void* tempRealloc = realloc(arr->data, newSize);
		if (!tempRealloc) {
			PRINT_ARR_ERROR(ARR_ReAllocFail);
			return;
		}
		arr->data = tempRealloc;
		arr->end = ((char*)arr->data) + newSize;
	}

	memcpy(((char*)arr->data) + usedSize, data_, data_len);
	arr->elementCount += count;
}

void arr_Insert(Array* arr, uint index, const void* data_, uint count) {
	if (!arr) {
		PRINT_ARR_ERROR(ARR_ArrIsNullptr);
		return;
	}

	if (index == arr->elementCount) {
		arr_Push(arr, data_, count);
		return;
	}

	if (!data_ || !count) {
		PRINT_ARR_ERROR(ARR_NoDataPassed);
		return;
	}

	if (index > arr->elementCount) {
		PRINT_ARR_ERROR(ARR_OutOfBounds);
		return;
	}

	uint data_len = count * arr->dataSize;
	uint usedSize = arr->elementCount * arr->dataSize;
	uint maxSize = arr_GetAllocatedSize(arr);

	if ((usedSize + data_len) > maxSize) {
		uint newSize = maxSize * 2;
		void* tempRealloc = realloc(arr->data, newSize);
		if (!tempRealloc) {
			PRINT_ARR_ERROR(ARR_ReAllocFail);
			return;
		}
		arr->data = tempRealloc;
		arr->end = ((char*)arr->data) + newSize;
	}

	uint rightDataOffset = index * arr->dataSize;
	uint rightDataSize = usedSize - rightDataOffset;
	memmove(((char*)(arr->data)) + rightDataOffset + data_len, ((char*)(arr->data)) + rightDataOffset, rightDataSize);
	memcpy(((char*)(arr->data)) + rightDataOffset, data_, data_len);
	arr->elementCount += count;
}

void arr_Remove(Array* arr, uint index, uint count, bool clear_recursive) {
	if (!arr) {
		PRINT_ARR_ERROR(ARR_ArrIsNullptr);
		return;
	}

	if (!count) return;

	if (!arr->data || (index + count) > arr->elementCount) {
		PRINT_ARR_ERROR(ARR_OutOfBounds);
		return;
	}

	uint usedSize = arr->elementCount * arr->dataSize;

	uint eraseSize = count * arr->dataSize;
	uint rightDataOffset = index * arr->dataSize;
	uint rightDataSize = usedSize - (index * arr->dataSize) - eraseSize;

	if (clear_recursive) {
		for (uint i = 0; i < count; i++) {
			if (arr->containsArrays) {
				if (arr->containsPointers) {
					arr_Clear(((Array**)arr->data)[index + i], clear_recursive);

				} else {
					arr_Clear(&((Array*)arr->data)[index + i], clear_recursive);
				}
			}
			if (arr->containsPointers) {
				free(((void**)arr->data)[index + i]);
			}
		}
	}

	memcpy(((char*)(arr->data)) + rightDataOffset, ((char*)(arr->data)) + rightDataOffset + eraseSize, rightDataSize);
	arr->elementCount -= count;

	usedSize = arr->elementCount * arr->dataSize;
	uint maxSize = arr_GetAllocatedSize(arr);
	if (arr->elementCount == 0) {
		free(arr->data);
		arr->data = NULL;
		arr->end = NULL;
	} else if (usedSize < (maxSize / 2)) {
		arr_ShrinkToFit(arr);
	}
}

uint arr_GetAllocatedSize(const Array* arr) {
	if (!arr) {
		PRINT_ARR_ERROR(ARR_ArrIsNullptr);
		return 0;
	}
	return ((char*)arr->end) - ((char*)arr->data);
}

void arr_ShrinkToFit(Array* arr) {
	if (!arr) {
		PRINT_ARR_ERROR(ARR_ArrIsNullptr);
		return;
	}

	if (!arr->dataSize) {
		PRINT_ARR_ERROR(ARR_EmptyDataSize);
		return;
	}

	uint maxSize = arr_GetAllocatedSize(arr);
	if (!arr->data || !maxSize) return;

	uint usedSize = arr->elementCount * arr->dataSize;
	if (!usedSize) {
		free(arr->data);
		arr->data = NULL;
		arr->end = NULL;
	} else if (maxSize > usedSize) {
		void* tempRealloc = realloc(arr->data, usedSize);
		if (!tempRealloc) {
			PRINT_ARR_ERROR(ARR_ReAllocFail);
			return;
		}
		arr->data = tempRealloc;
		arr->end = ((char*)arr->data) + usedSize;
	}
}

bool arr_Reserve(Array* arr, uint count) {
	if (!arr) {
		PRINT_ARR_ERROR(ARR_ArrIsNullptr);
		return false;
	}
	void* tempRealloc = NULL;

	if (!arr->data) {
		tempRealloc = calloc(count, arr->dataSize);
		if (!tempRealloc) {
			PRINT_ARR_ERROR(ARR_AllocFail);
			return false;
		}
	} else {
		tempRealloc = realloc(arr->data, count * arr->dataSize);
		if (!tempRealloc) {
			PRINT_ARR_ERROR(ARR_ReAllocFail);
			return false;
		}
	}

	arr->data = tempRealloc;
	arr->end = (((char*)arr->data)) + (count * arr->dataSize);
	return true;
}

bool arr_Equals(const Array* arr, const Array* other) {
	if (!arr || !other) return false;
	if (arr == other) return true;
	if (!arr->elementCount || !other->elementCount) return false;
	if (arr->elementCount != other->elementCount) return false;
	if (!arr->dataSize || !other->dataSize) return false;
	if (arr->dataSize != other->dataSize) return false;
	if (arr->containsArrays && !other->containsArrays) return false;
	if (arr->containsPointers && !other->containsPointers) return false;

	if (arr->containsArrays) {
		if (arr->containsPointers) {
			for (uint i = 0; i < arr->elementCount; i++) {
				Array* arrEle = ((Array*)arr->data) + i;
				Array* otherEle = ((Array*)other->data) + i;
				if (!arr_Equals(arrEle, otherEle)) return false;
			}
		} else {
			for (uint i = 0; i < arr->elementCount; i++) {
				Array* arrEle = ((Array**)arr->data)[i];
				Array* otherEle = ((Array**)other->data)[i];
				if (!arr_Equals(arrEle, otherEle)) return false;
			}
		}
	} else {
		const uint usedSize = arr->elementCount * arr->dataSize;
		for (uint i = 0; i < usedSize; i++) {
			if (((char*)arr->data)[i] != ((char*)other->data)[i]) return false;
		}
	}

	return true;
}
