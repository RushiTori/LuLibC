#ifndef L_LINKED_LIST_H
#define L_LINKED_LIST_H

#include "LuUtils.h"

typedef struct LinkedNode {
	struct LinkedNode* prev;
	struct LinkedNode* next;
	void* data;
} LinkedNode;

#define EmptyLinkedNode \
	(LinkedNode) { .prev = NULL, .next = NULL, data = NULL }

typedef struct LinkedList {
	LinkedNode* begin;
	LinkedNode* end;
	uint dataSize;
	uint elementCount;
	bool containsPointers;
} LinkedList;

#define EmptyLinkedList \
	(LinkedList) { .begin = NULL, .end = NULL, .dataSize = 0, .elementCount = 0, .containsPointers = false }

LinkedList ll_Create(uint dataSize, bool isDataPointers);
void ll_Clear(LinkedList* list, bool clear_recursive);

LinkedNode* ll_Get(LinkedList* list, uint index);
void ll_Set(LinkedList* list, uint index, void* data_);

void ll_Push(LinkedList* list, void* data_);
void ll_Insert(LinkedList* list, uint index, void* data_);
void ll_Remove(LinkedList* list, uint index, bool clear_recursive);

#endif	// L_LINKED_LIST_H
