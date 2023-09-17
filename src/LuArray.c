#include "LuArray.h"

/*#include "LuLogs.h"

Array arr_Create(int dataSize, int initialCapacity, bool isDataArrays, bool isDataPointers) {
	Array temp = EmptyArray;
	temp.dataSize = dataSize;
	temp.containsArrays = isDataArrays;
	temp.containsPointers = isDataPointers;

	if (!dataSize) {
		LOG_CONTAINER_ERROR(NoDataSize);
		return temp;
	}

	if (!initialCapacity) return temp;

	temp.data = calloc(initialCapacity, dataSize);
	if (!temp.data) {
		LOG_CONTAINER_ERROR(MemAllocationFail);
		return temp;
	}
	temp.end = ((char*)temp.data) + (initialCapacity * dataSize);
	return temp;
}

void arr_Clear(Array* arr, bool clear_recursive) {
	if (!arr) {
		LOG_CONTAINER_ERROR(NullContainer);
		return;
	}

	if (!arr->dataSize) {
		LOG_CONTAINER_ERROR(NoDataSize);
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
		LOG_CONTAINER_ERROR(NullContainer);
		return NULL;
	}

	if (!arr->dataSize) {
		LOG_CONTAINER_ERROR(NoDataSize);
		return NULL;
	}

	if (!arr->data || index >= arr->elementCount) {
		LOG_CONTAINER_ERROR(OutOfBounds);
		return NULL;
	}

	return ((char*)arr->data) + (index * arr->dataSize);
}

void arr_Set(Array* arr, uint index, const void* data_, uint count) {
	void* temp = arr_Get(arr, index);
	if (!temp) return;

	if (!data_ || !count) {
		LOG_CONTAINER_ERROR(NullDataPassed);
		return;
	}

	if ((index + count) > arr->elementCount) {
		LOG_CONTAINER_ERROR(OutOfBounds);
		return;
	}

	memcpy(temp, data_, count * arr->dataSize);
}

void arr_Push(Array* arr, const void* data_, uint count) {
	if (!arr) {
		LOG_CONTAINER_ERROR(NullContainer);
		return;
	}

	if (!data_ || !count) {
		LOG_CONTAINER_ERROR(NullDataPassed);
		return;
	}

	uint data_len = count * arr->dataSize;

	if (!arr->data) {
		arr->data = malloc(data_len);
		if (!arr->data) {
			LOG_CONTAINER_ERROR(MemAllocationFail);
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
			LOG_CONTAINER_ERROR(MemReallocationFail);
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
		LOG_CONTAINER_ERROR(NullContainer);
		return;
	}

	if (index == arr->elementCount) {
		arr_Push(arr, data_, count);
		return;
	}

	if (!data_ || !count) {
		LOG_CONTAINER_ERROR(NullDataPassed);
		return;
	}

	if (index > arr->elementCount) {
		LOG_CONTAINER_ERROR(OutOfBounds);
		return;
	}

	uint data_len = count * arr->dataSize;
	uint usedSize = arr->elementCount * arr->dataSize;
	uint maxSize = arr_GetAllocatedSize(arr);

	if ((usedSize + data_len) > maxSize) {
		uint newSize = maxSize * 2;
		void* tempRealloc = realloc(arr->data, newSize);
		if (!tempRealloc) {
			LOG_CONTAINER_ERROR(MemReallocationFail);
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
		LOG_CONTAINER_ERROR(NullContainer);
		return;
	}

	if (!count) return;

	if (!arr->data || (index + count) > arr->elementCount) {
		LOG_CONTAINER_ERROR(OutOfBounds);
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
		LOG_CONTAINER_ERROR(NullContainer);
		return 0;
	}
	return ((char*)arr->end) - ((char*)arr->data);
}

void arr_ShrinkToFit(Array* arr) {
	if (!arr) {
		LOG_CONTAINER_ERROR(NullContainer);
		return;
	}

	if (!arr->dataSize) {
		LOG_CONTAINER_ERROR(NoDataSize);
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
			LOG_CONTAINER_ERROR(MemReallocationFail);
			return;
		}
		arr->data = tempRealloc;
		arr->end = ((char*)arr->data) + usedSize;
	}
}

bool arr_Reserve(Array* arr, uint count) {
	if (!arr) {
		LOG_CONTAINER_ERROR(NullContainer);
		return false;
	}
	void* tempRealloc = NULL;

	if (!arr->data) {
		tempRealloc = calloc(count, arr->dataSize);
		if (!tempRealloc) {
			LOG_CONTAINER_ERROR(MemAllocationFail);
			return false;
		}
	} else {
		tempRealloc = realloc(arr->data, count * arr->dataSize);
		if (!tempRealloc) {
			LOG_CONTAINER_ERROR(MemReallocationFail);
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
*/

DEFINE_ARRAY_FUNCTIONS(byte)
