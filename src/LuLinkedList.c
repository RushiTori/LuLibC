#include "LuLinkedList.h"

#include "LuLogs.h"

LinkedList ll_Create(uint dataSize, bool isDataPointers) {
	LinkedList temp;
	temp.begin = NULL;
	temp.end = NULL;
	temp.dataSize = dataSize;
	temp.elementCount = 0;
	temp.containsPointers = isDataPointers;
	return temp;
}

void ll_Clear(LinkedList* list, bool clear_recursive) {
	if (!list) {
		LOG_CONTAINER_ERROR(NullContainer);
		return;
	}
	if (!list->dataSize) {
		LOG_CONTAINER_ERROR(NoDataSize);
		return;
	}
	if ((!list->begin && !list->end) || !list->elementCount) {
		return;
	}
	LinkedNode* temp = list->begin;
	while (temp) {
		LinkedNode* nextTemp = temp->next;
		if (clear_recursive) {
			if (list->containsPointers) {
				free(*((void**)(temp->data)));
			}
		}
		free(temp->data);
		temp->data = NULL;
		temp->prev = NULL;
		temp->next = NULL;
		free(temp);
		temp = nextTemp;
	}

	list->begin = NULL;
	list->end = NULL;
	list->elementCount = 0;
}

LinkedNode* ll_Get(LinkedList* list, uint index) {
	if (!list) {
		LOG_CONTAINER_ERROR(NullContainer);
		return NULL;
	}

	if (!list->dataSize) {
		LOG_CONTAINER_ERROR(NoDataSize);
		return NULL;
	}

	if (index >= list->elementCount) {
		LOG_CONTAINER_ERROR(OutOfBounds);
		return NULL;
	}

	if (index == 0) return list->begin;
	if (index == (list->elementCount - 1)) return list->end;

	if ((list->elementCount - index) >= index) {
		LinkedNode* actual = list->begin;
		while (actual->next) {
			if (index == 0) return actual;
			actual = actual->next;
			index--;
		}
	} else {
		LinkedNode* actual = list->end;
		while (actual->prev) {
			if (index == (list->elementCount - 1)) return actual;
			actual = actual->prev;
			index++;
		}
	}

	// To be honest I don't know what happened but
	// the user did something wrong with the number of elements
	LOG_CONTAINER_ERROR(OutOfBounds);
	return NULL;
}

void ll_Set(LinkedList* list, uint index, void* data_) {
	LinkedNode* temp = ll_Get(list, index);

	if (!temp) return;

	if (!data_) {
		LOG_CONTAINER_ERROR(NullDataPassed);
		return;
	}

	if (!list->dataSize) {
		LOG_CONTAINER_ERROR(NoDataSize);
		return;
	}

	memcpy(temp->data, data_, list->dataSize);
}

void ll_Push(LinkedList* list, void* data_) {
	if (!list) {
		LOG_CONTAINER_ERROR(NullContainer);
		return;
	}

	if (!list->dataSize) {
		LOG_CONTAINER_ERROR(NoDataSize);
		return;
	}

	if (!data_) {
		LOG_CONTAINER_ERROR(NullDataPassed);
		return;
	}

	if (!list->dataSize) {
		LOG_CONTAINER_ERROR(NoDataSize);
		return;
	}

	LinkedNode* tempNode = calloc(1, sizeof(LinkedNode));
	if (!tempNode) {
		LOG_CONTAINER_ERROR(MemAllocationFail);
		return;
	}

	tempNode->data = malloc(list->dataSize);
	if (!tempNode->data) {
		LOG_CONTAINER_ERROR(MemAllocationFail);
		free(tempNode);
		return;
	}

	memcpy(tempNode->data, data_, list->dataSize);

	if (list->elementCount == 0) {
		list->begin = tempNode;
		list->end = NULL;
	} else if (list->elementCount == 1) {
		list->end = tempNode;
		list->begin->next = list->end;
		list->end->prev = list->begin;
	} else {
		tempNode->prev = list->end;
		list->end->next = tempNode;
		list->end = tempNode;
	}

	list->elementCount++;
}

void ll_Insert(LinkedList* list, uint index, void* data_) {
	if (!list) {
		LOG_CONTAINER_ERROR(NullContainer);
		return;
	}

	if ((list->elementCount == 0) || (list->elementCount == 1) || (index == list->elementCount)) {
		ll_Push(list, data_);
		return;
	}

	if (!list->dataSize) {
		LOG_CONTAINER_ERROR(NoDataSize);
		return;
	}

	if (!data_ || !list->dataSize) {
		LOG_CONTAINER_ERROR(NullDataPassed);
		return;
	}

	if (index > list->elementCount) {
		LOG_CONTAINER_ERROR(OutOfBounds);
	}

	LinkedNode* tempNode = calloc(1, sizeof(LinkedNode));
	if (!tempNode) {
		LOG_CONTAINER_ERROR(MemAllocationFail);
		return;
	}

	tempNode->data = malloc(list->dataSize);
	if (!tempNode->data) {
		LOG_CONTAINER_ERROR(MemAllocationFail);
		free(tempNode);
		return;
	}

	memcpy(tempNode->data, data_, list->dataSize);

	if (index == 0) {
		tempNode->next = list->begin;
		tempNode->next->prev = tempNode;
		list->begin = tempNode;
	} else {
		LinkedNode* gotNode = ll_Get(list, index);
		tempNode->prev = gotNode->prev;
		tempNode->next = gotNode;
		gotNode->prev->next = tempNode;
		gotNode->prev = tempNode;
	}

	list->elementCount++;
}

void ll_Remove(LinkedList* list, uint index, bool clear_recursive) {
	if (!list) {
		LOG_CONTAINER_ERROR(NullContainer);
		return;
	}

	if (!list->dataSize) {
		LOG_CONTAINER_ERROR(NoDataSize);
		return;
	}

	if (index >= list->elementCount) {
		LOG_CONTAINER_ERROR(OutOfBounds);
		return;
	}

	LinkedNode* toRemove = NULL;

	if (index == 0) {
		toRemove = list->begin;
		list->begin = list->begin->next;
	} else if (index == (list->elementCount - 1)) {
		toRemove = list->end;
		list->end = list->end->prev;
	} else {
		toRemove = ll_Get(list, index);
	}

	if (!toRemove) return;	// Unknown Pointer Issue

	LinkedNode* prev = toRemove->prev;
	LinkedNode* next = toRemove->next;

	if (clear_recursive) {
		if (list->containsPointers) {
			free(*((void**)toRemove->data));
		}
	}

	free(toRemove->data);
	toRemove->data = NULL;
	toRemove->prev = NULL;
	toRemove->next = NULL;
	free(toRemove);

	if (prev) prev->next = next;
	if (next) next->prev = prev;

	list->elementCount--;

	if (list->elementCount <= 1) {
		list->end = NULL;
	}

	if (list->elementCount == 0) {
		list->begin = NULL;
	}
}
