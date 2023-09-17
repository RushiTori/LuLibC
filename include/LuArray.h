#ifndef L_ARRAY_H
#define L_ARRAY_H

#include "LuUtils.h"

#define DECLARE_ARRAY_TYPE(type)     \
	typedef struct Array##type##_s { \
		type* data;                  \
		uint size;                   \
		uint capacity;               \
	} Array##type;

#define EmptyArray(type) \
	(Array##type) { .data = NULL, .size = 0, .capacity = 0 }

#define DECLARE_ARRAY_FUNCTIONS(type)                                                          \
	Array##type Array##type##Create(uint capacity);                                            \
	void Array##type##Clear(Array##type* arr);                                                 \
	void Array##type##Push(Array##type* arr, type element);                                    \
	void Array##type##PushAll(Array##type* arr, const type* elements, uint count);             \
	void Array##type##Insert(Array##type* arr, uint idx, type element);                        \
	void Array##type##InsertAll(Array##type* arr, uint idx, const type* elements, uint count); \
	void Array##type##Erase(Array##type* arr, uint idx);                                       \
	void Array##type##EraseAll(Array##type* arr, uint idx, uint count);                        \
	void Array##type##ShrinkToFit(Array##type* arr);                                           \
	void Array##type##Reserve(Array##type* arr, uint capa);                                    \
	void Array##type##Sort(Array##type* arr, bool (*comp)(type * a, type * b));

#define DEFINE_ARRAY_FUNCTIONS(type)                                                                                  \
                                                                                                                      \
	Array##type Array##type##Create(uint capacity) {                                                                  \
		Array##type temp = EmptyArray(type);                                                                          \
		if (!capacity) return temp;                                                                                   \
                                                                                                                      \
		temp.data = calloc(capacity, sizeof(type));                                                                   \
		if (!temp.data) {                                                                                             \
			LOG_CONTAINER_ERROR(MemAllocationFail);                                                                   \
			return;                                                                                                   \
		}                                                                                                             \
                                                                                                                      \
		temp.capacity = capacity;                                                                                     \
		return temp;                                                                                                  \
	}                                                                                                                 \
                                                                                                                      \
	void Array##type##Clear(Array##type* arr) {                                                                       \
		if (!arr) {                                                                                                   \
			LOG_CONTAINER_ERROR(NullContainer);                                                                       \
			return;                                                                                                   \
		}                                                                                                             \
                                                                                                                      \
		free(arr->data);                                                                                              \
		*arr = EmptyArray(type);                                                                                      \
	}                                                                                                                 \
                                                                                                                      \
	void Array##type##Push(Array##type* arr, type element) {                                                          \
		if (!arr) {                                                                                                   \
			LOG_CONTAINER_ERROR(NullContainer);                                                                       \
			return;                                                                                                   \
		}                                                                                                             \
                                                                                                                      \
		if (!arr->data) {                                                                                             \
			type* temp = malloc(sizeof(type) * 1);                                                                    \
			if (!temp) {                                                                                              \
				LOG_CONTAINER_ERROR(MemAllocationFail);                                                               \
				return;                                                                                               \
			}                                                                                                         \
			temp[0] = element;                                                                                        \
			arr->data = temp;                                                                                         \
			arr->capacity = 1;                                                                                        \
			arr->size = 1;                                                                                            \
			return;                                                                                                   \
		}                                                                                                             \
                                                                                                                      \
		if (arr->size == arr->capacity) {                                                                             \
			type* temp = realloc(arr->data, sizeof(type) * arr->capacity * 2);                                        \
			if (!temp) {                                                                                              \
				LOG_CONTAINER_ERROR(MemReallocationFail);                                                             \
				return;                                                                                               \
			}                                                                                                         \
			arr->data = temp;                                                                                         \
			arr->capacity *= 2;                                                                                       \
		}                                                                                                             \
                                                                                                                      \
		arr->data[arr->size++] = element;                                                                             \
	}                                                                                                                 \
                                                                                                                      \
	void Array##type##PushAll(Array##type* arr, const type* elements, uint count) {                                   \
		if (!elements || !count) {                                                                                    \
			LOG_CONTAINER_ERROR(NullDataPassed);                                                                      \
			return;                                                                                                   \
		}                                                                                                             \
                                                                                                                      \
		if (count == 1) {                                                                                             \
			Array##type##Push(arr, *elements);                                                                        \
			return;                                                                                                   \
		}                                                                                                             \
                                                                                                                      \
		if (!arr) {                                                                                                   \
			LOG_CONTAINER_ERROR(NullContainer);                                                                       \
			return;                                                                                                   \
		}                                                                                                             \
                                                                                                                      \
		if (!arr->data) {                                                                                             \
			uint newCapa = 1;                                                                                         \
			while (count > newCapa) {                                                                                 \
				newCapa *= 2;                                                                                         \
			}                                                                                                         \
                                                                                                                      \
			arr->data = malloc(sizeof(type) * newCapa);                                                               \
			if (!arr->data) {                                                                                         \
				LOG_CONTAINER_ERROR(MemAllocationFail);                                                               \
				return;                                                                                               \
			}                                                                                                         \
                                                                                                                      \
			memcpy(arr->data, elements, sizeof(type) * count);                                                        \
			arr->size = count;                                                                                        \
			arr->capacity = newCapa;                                                                                  \
			return;                                                                                                   \
		}                                                                                                             \
                                                                                                                      \
		if (arr->size + count >= arr->capacity) {                                                                     \
			uint newCapa = arr->capacity;                                                                             \
			while (arr->size + count > newCapa) {                                                                     \
				newCapa *= 2;                                                                                         \
			}                                                                                                         \
                                                                                                                      \
			type* temp = realloc(arr->data, sizeof(type) * newCapa);                                                  \
			if (!temp) {                                                                                              \
				LOG_CONTAINER_ERROR(MemReallocationFail);                                                             \
				return;                                                                                               \
			}                                                                                                         \
			arr->data = temp;                                                                                         \
			arr->capacity = newCapa;                                                                                  \
		}                                                                                                             \
		memcpy(arr->data + arr->size, elements, sizeof(type) * count);                                                \
		arr->size += count;                                                                                           \
	}                                                                                                                 \
                                                                                                                      \
	void Array##type##Insert(Array##type* arr, uint idx, type element) {                                              \
		if (!arr) {                                                                                                   \
			LOG_CONTAINER_ERROR(NullContainer);                                                                       \
			return;                                                                                                   \
		}                                                                                                             \
                                                                                                                      \
		if (idx == arr->size) {                                                                                       \
			Array##type##Push(arr, element);                                                                          \
			return;                                                                                                   \
		}                                                                                                             \
                                                                                                                      \
		if (idx > arr->size) {                                                                                        \
			LOG_CONTAINER_ERROR(OutOfBounds);                                                                         \
			return;                                                                                                   \
		}                                                                                                             \
                                                                                                                      \
		if (!arr->data) {                                                                                             \
			LOG_CONTAINER_ERROR(OutOfBounds);                                                                         \
			return;                                                                                                   \
		}                                                                                                             \
                                                                                                                      \
		if (arr->size == arr->capacity) {                                                                             \
			type* temp = realloc(arr->data, sizeof(type) * arr->capacity * 2);                                        \
			if (!temp) {                                                                                              \
				LOG_CONTAINER_ERROR(MemReallocationFail);                                                             \
				return;                                                                                               \
			}                                                                                                         \
			arr->data = temp;                                                                                         \
			arr->capacity *= 2;                                                                                       \
		}                                                                                                             \
                                                                                                                      \
		memmove(arr->data + idx + 1, arr->data + idx, sizeof(type) * (arr->size - idx));                              \
                                                                                                                      \
		arr->size++;                                                                                                  \
		arr->data[idx] = element;                                                                                     \
	}                                                                                                                 \
                                                                                                                      \
	void Array##type##InsertAll(Array##type* arr, uint idx, const type* elements, uint count) {                       \
		if (!elements || !count) {                                                                                    \
			LOG_CONTAINER_ERROR(NullDataPassed);                                                                      \
			return;                                                                                                   \
		}                                                                                                             \
                                                                                                                      \
		if (count == 1) {                                                                                             \
			Array##type##Insert(arr, idx, *elements);                                                                 \
			return;                                                                                                   \
		}                                                                                                             \
                                                                                                                      \
		if (!arr) {                                                                                                   \
			LOG_CONTAINER_ERROR(NullContainer);                                                                       \
			return;                                                                                                   \
		}                                                                                                             \
                                                                                                                      \
		if (idx > arr->size) {                                                                                        \
			LOG_CONTAINER_ERROR(OutOfBounds);                                                                         \
			return;                                                                                                   \
		}                                                                                                             \
                                                                                                                      \
		if (idx == arr->size) {                                                                                       \
			if (!arr->size) {                                                                                         \
				Array##type##PushAll(arr, elements, count);                                                           \
				return;                                                                                               \
			}                                                                                                         \
                                                                                                                      \
			if (count > arr->capacity) {                                                                              \
				uint newCapa = arr->capacity;                                                                         \
				while (count > newCapa) {                                                                             \
					newCapa *= 2;                                                                                     \
				}                                                                                                     \
				type* temp = realloc(arr->data, sizeof(type) * newCapa);                                              \
				if (!temp) {                                                                                          \
					LOG_CONTAINER_ERROR(MemReallocationFail);                                                         \
					return;                                                                                           \
				}                                                                                                     \
				arr->data = temp;                                                                                     \
				arr->capacity = newCapa;                                                                              \
			}                                                                                                         \
                                                                                                                      \
			memcpy(arr->data + idx, elements, sizeof(type) * count);                                                  \
			arr->size += count;                                                                                       \
			return;                                                                                                   \
		}                                                                                                             \
                                                                                                                      \
		if (arr->size + count >= arr->capacity) {                                                                     \
			uint newCapa = arr->capacity;                                                                             \
			while (arr->size + count > newCapa) {                                                                     \
				newCapa *= 2;                                                                                         \
			}                                                                                                         \
                                                                                                                      \
			type* temp = realloc(arr->data, sizeof(type) * newCapa);                                                  \
			if (!temp) {                                                                                              \
				LOG_CONTAINER_ERROR(MemReallocationFail);                                                             \
				return;                                                                                               \
			}                                                                                                         \
			arr->data = temp;                                                                                         \
			arr->capacity = newCapa;                                                                                  \
		}                                                                                                             \
                                                                                                                      \
		memmove(arr->data + idx + count, arr->data + idx, sizeof(type) * (arr->size - idx));                          \
		memcpy(arr->data + idx, elements, sizeof(type) * count);                                                      \
		arr->size += count;                                                                                           \
	}                                                                                                                 \
                                                                                                                      \
	void Array##type##Erase(Array##type* arr, uint idx) {                                                             \
		if (!arr) {                                                                                                   \
			LOG_CONTAINER_ERROR(NullContainer);                                                                       \
			return;                                                                                                   \
		}                                                                                                             \
                                                                                                                      \
		if (!arr->data) {                                                                                             \
			LOG_CONTAINER_ERROR(OutOfBounds);                                                                         \
			return;                                                                                                   \
		}                                                                                                             \
                                                                                                                      \
		if (idx >= arr->size) {                                                                                       \
			LOG_CONTAINER_ERROR(OutOfBounds);                                                                         \
			return;                                                                                                   \
		}                                                                                                             \
                                                                                                                      \
		if (arr->size == 1) {                                                                                         \
			Array##type##Clear(arr);                                                                                  \
			return;                                                                                                   \
		}                                                                                                             \
                                                                                                                      \
		if (idx != arr->size - 1) memcpy(arr->data + idx, arr->data + idx + 1, sizeof(type) * (arr->size - 1 - idx)); \
		arr->size--;                                                                                                  \
                                                                                                                      \
		if (arr->size < arr->capacity / 2) {                                                                          \
			type* temp = realloc(arr->data, sizeof(type) * (arr->capacity / 2));                                      \
			if (!temp) {                                                                                              \
				LOG_CONTAINER_ERROR(MemReallocationFail);                                                             \
				return;                                                                                               \
			}                                                                                                         \
			arr->data = temp;                                                                                         \
			arr->capacity /= 2;                                                                                       \
		}                                                                                                             \
	}                                                                                                                 \
                                                                                                                      \
	void Array##type##EraseAll(Array##type* arr, uint idx, uint count) {                                              \
		if (count == 1) {                                                                                             \
			Array##type##Erase(arr, idx);                                                                             \
			return;                                                                                                   \
		}                                                                                                             \
                                                                                                                      \
		if (!arr) {                                                                                                   \
			LOG_CONTAINER_ERROR(NullContainer);                                                                       \
			return;                                                                                                   \
		}                                                                                                             \
                                                                                                                      \
		if (!arr->data) {                                                                                             \
			LOG_CONTAINER_ERROR(OutOfBounds);                                                                         \
			return;                                                                                                   \
		}                                                                                                             \
                                                                                                                      \
		if (idx + count > arr->size) {                                                                                \
			LOG_CONTAINER_ERROR(OutOfBounds);                                                                         \
			return;                                                                                                   \
		}                                                                                                             \
                                                                                                                      \
		if (arr->size == count) {                                                                                     \
			Array##type##Clear(arr);                                                                                  \
			return;                                                                                                   \
		}                                                                                                             \
                                                                                                                      \
		if (idx != arr->size - 1 - count)                                                                             \
			memcpy(arr->data + idx, arr->data + idx + count, sizeof(type) * (arr->size - idx - count));               \
		arr->size -= count;                                                                                           \
                                                                                                                      \
		if (arr->size < arr->capacity / 2) {                                                                          \
			type* temp = realloc(arr->data, sizeof(type) * (arr->capacity / 2));                                      \
			if (!temp) {                                                                                              \
				LOG_CONTAINER_ERROR(MemReallocationFail);                                                             \
				return;                                                                                               \
			}                                                                                                         \
			arr->data = temp;                                                                                         \
			arr->capacity /= 2;                                                                                       \
		}                                                                                                             \
	}                                                                                                                 \
                                                                                                                      \
	void Array##type##ShrinkToFit(Array##type* arr) {                                                                 \
		if (!arr) {                                                                                                   \
			LOG_CONTAINER_ERROR(NullContainer);                                                                       \
			return;                                                                                                   \
		}                                                                                                             \
                                                                                                                      \
		if (!arr->data) return;                                                                                       \
		if (arr->capacity == arr->size) return;                                                                       \
                                                                                                                      \
		type* temp = realloc(arr->data, sizeof(type) * arr->size);                                                    \
		if (!temp) {                                                                                                  \
			LOG_CONTAINER_ERROR(MemReallocationFail);                                                                 \
			return;                                                                                                   \
		}                                                                                                             \
		arr->data = temp;                                                                                             \
		arr->capacity = arr->size;                                                                                    \
	}                                                                                                                 \
                                                                                                                      \
	void Array##type##Reserve(Array##type* arr, uint capa) {                                                          \
		if (!arr) {                                                                                                   \
			LOG_CONTAINER_ERROR(NullContainer);                                                                       \
			return;                                                                                                   \
		}                                                                                                             \
                                                                                                                      \
		if (arr->capacity >= capa) return;                                                                            \
                                                                                                                      \
		type* temp;                                                                                                   \
		if (arr->data) {                                                                                              \
			temp = realloc(arr->data, sizeof(type) * capa);                                                           \
			if (!temp) {                                                                                              \
				LOG_CONTAINER_ERROR(MemReallocationFail);                                                             \
				return;                                                                                               \
			}                                                                                                         \
		} else {                                                                                                      \
			temp = malloc(sizeof(type) * capa);                                                                       \
			if (!temp) {                                                                                              \
				LOG_CONTAINER_ERROR(MemAllocationFail);                                                               \
				return;                                                                                               \
			}                                                                                                         \
		}                                                                                                             \
                                                                                                                      \
		arr->data = temp;                                                                                             \
		arr->capacity = capa;                                                                                         \
	}                                                                                                                 \
                                                                                                                      \
	void Array##type##Sort(Array##type* arr, bool (*comp)(type * a, type * b)) {                                      \
		if (!arr) {                                                                                                   \
			LOG_CONTAINER_ERROR(NullContainer);                                                                       \
			return;                                                                                                   \
		}                                                                                                             \
                                                                                                                      \
		if (!arr->size) return;                                                                                       \
                                                                                                                      \
		for (uint i = 0; i < arr->size; i++) {                                                                        \
			for (uint j = i + 1; j < arr->size; j++) {                                                                \
				bool lt = false;                                                                                      \
                                                                                                                      \
				if (comp) {                                                                                           \
					lt = comp(arr->data + j, arr->data + i);                                                          \
				} else {                                                                                              \
					lt = arr->data[j] < arr->data[i];                                                                 \
				}                                                                                                     \
                                                                                                                      \
				if (lt) {                                                                                             \
					type temp = arr->data[i];                                                                         \
					arr->data[i] = arr->data[j];                                                                      \
					arr->data[j] = temp;                                                                              \
				}                                                                                                     \
			}                                                                                                         \
		}                                                                                                             \
	}

DECLARE_ARRAY_TYPE(byte)
DECLARE_ARRAY_FUNCTIONS(byte)

#endif	// L_ARRAY_H
