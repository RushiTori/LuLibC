#include <LuLinkedList.h>

enum LL_ErrorCodes {
	LL_LLIsNullptr = 0,
	LL_OutOfBounds,
	LL_EmptyDataSize,
	LL_NoDataPassed,
	LL_AllocFail,
	LL_ErrorCount
};

static const string LL_ErrorMessages[LL_ErrorCount] = {
	"Linked List Passed Is Nullptr",
	"Linked List Out Of Bounds Exception",
	"Size Of Element Is 0 (Zero)",
	"Trying To Push/Insert/Remove Nullptr/Empty Memory",
	"Linked List Allocation (malloc/calloc) Failed",
};

#define PRINT_LL_ERROR(ll_err) \
	fprintf(stderr, "Linked List Error %d: %s\nAt : %s:%d\n", ll_err, LL_ErrorMessages[ll_err], __FILE__, __LINE__)

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
		PRINT_LL_ERROR(LL_LLIsNullptr);
		return;
	}
	if (!list->dataSize) {
		PRINT_LL_ERROR(LL_EmptyDataSize);
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
		PRINT_LL_ERROR(LL_LLIsNullptr);
		return NULL;
	}

	if (!list->dataSize) {
		PRINT_LL_ERROR(LL_EmptyDataSize);
		return NULL;
	}

	if (index >= list->elementCount) {
		PRINT_LL_ERROR(LL_OutOfBounds);
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
	PRINT_LL_ERROR(LL_OutOfBounds);
	return NULL;
}

void ll_Set(LinkedList* list, uint index, void* data_) {
	LinkedNode* temp = ll_Get(list, index);

	if (!temp) return;

	if (!data_) {
		PRINT_LL_ERROR(LL_NoDataPassed);
		return;
	}

	if (!list->dataSize) {
		PRINT_LL_ERROR(LL_EmptyDataSize);
		return;
	}

	memcpy(temp->data, data_, list->dataSize);
}

void ll_Push(LinkedList* list, void* data_) {
	if (!list) {
		PRINT_LL_ERROR(LL_LLIsNullptr);
		return;
	}

	if (!list->dataSize) {
		PRINT_LL_ERROR(LL_EmptyDataSize);
		return;
	}

	if (!data_) {
		PRINT_LL_ERROR(LL_NoDataPassed);
		return;
	}

	if (!list->dataSize) {
		PRINT_LL_ERROR(LL_EmptyDataSize);
		return;
	}

	LinkedNode* tempNode = calloc(1, sizeof(LinkedNode));
	if (!tempNode) {
		PRINT_LL_ERROR(LL_AllocFail);
		return;
	}

	tempNode->data = malloc(list->dataSize);
	if (!tempNode->data) {
		PRINT_LL_ERROR(LL_AllocFail);
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
		PRINT_LL_ERROR(LL_LLIsNullptr);
		return;
	}

	if ((list->elementCount == 0) || (list->elementCount == 1) || (index == list->elementCount)) {
		ll_Push(list, data_);
		return;
	}

	if (!list->dataSize) {
		PRINT_LL_ERROR(LL_EmptyDataSize);
		return;
	}

	if (!data_ || !list->dataSize) {
		PRINT_LL_ERROR(LL_NoDataPassed);
		return;
	}

	if (index > list->elementCount) {
		PRINT_LL_ERROR(LL_OutOfBounds);
	}

	LinkedNode* tempNode = calloc(1, sizeof(LinkedNode));
	if (!tempNode) {
		PRINT_LL_ERROR(LL_AllocFail);
		return;
	}

	tempNode->data = malloc(list->dataSize);
	if (!tempNode->data) {
		PRINT_LL_ERROR(LL_AllocFail);
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
		PRINT_LL_ERROR(LL_LLIsNullptr);
		return;
	}

	if (!list->dataSize) {
		PRINT_LL_ERROR(LL_EmptyDataSize);
		return;
	}

	if (index >= list->elementCount) {
		PRINT_LL_ERROR(LL_OutOfBounds);
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
