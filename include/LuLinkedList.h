#ifndef LU_LINKED_LIST_H
#define LU_LINKED_LIST_H

#include "LuUtils.h"

#define DECLARE_LINKEDLIST_TYPE(type)      \
	typedef struct LinkedNode##type##_s {  \
		struct LinkedNode##type##_s* prev; \
		struct LinkedNode##type##_s* next; \
		type data;                         \
	} LinkedNode##type;                    \
                                           \
	typedef struct LinkedList##type##_s {  \
		LinkedNode##type* head; \
		LinkedNode##type* tail; \
		uint size;                         \
	} LinkedList##type;

#define EmptyLinkedNode(type)                     \
	(LinkedNode##type) {                          \
		.prev = NULL, .next = NULL, .data = { 0 } \
	}

#define EmptyLinkedList(type) \
	(LinkedList##type) { .head = NULL, .tail = NULL, .size = 0 }

#define DECLARE_LINKEDLIST_FUNCTIONS(type)                                         \
	LinkedList##type LinkedList##type##Create();                                   \
	void LinkedList##type##Clear(LinkedList##type* list);                          \
	LinkedNode##type* LinkedList##type##Get(LinkedList##type* list, uint idx);     \
	void LinkedList##type##Set(LinkedList##type* list, uint idx, type element);    \
	void LinkedList##type##Push(LinkedList##type* list, type element);             \
	void LinkedList##type##Insert(LinkedList##type* list, uint idx, type element); \
	void LinkedList##type##Erase(LinkedList##type* list, uint idx);

#define DEFINE_LINKEDLIST_FUNCTIONS(type)                                                      \
                                                                                               \
	LinkedList##type LinkedList##type##Create() { return EmptyLinkedList(type); } \
                                                                                               \
	void LinkedList##type##Clear(LinkedList##type* list) {                                     \
		if (!list) {                                                                           \
			LOG_CONTAINER_ERROR(NullContainer);                                                \
			return;                                                                            \
		}                                                                                      \
                                                                                               \
		LinkedNode##type* node = list->head;                                                   \
                                                                                               \
		while (node) {                                                                         \
			LinkedNode##type* temp = node->next;                                               \
			free(node);                                                                        \
			node = temp;                                                                       \
		}                                                                                      \
                                                                                               \
		list->head = NULL;                                                                     \
		list->tail = NULL;                                                                     \
		list->size = 0;                                                                        \
	}                                                                                          \
                                                                                               \
	LinkedNode##type* LinkedList##type##Get(LinkedList##type* list, uint idx) {                \
		if (!list) {                                                                           \
			LOG_CONTAINER_ERROR(NullContainer);                                                \
			return NULL;                                                                            \
		}                                                                                      \
                                                                                               \
		if (idx >= list->size) {                                                               \
			LOG_CONTAINER_ERROR(OutOfBounds);                                                  \
			return NULL;                                                                            \
		}                                                                                      \
                                                                                               \
		if (idx == 0) return list->head;                                                       \
		if (idx == (list->size - 1)) return list->tail;                                        \
                                                                                               \
		if (idx < (list->size / 2)) {                                                          \
			LinkedNode##type* actual = list->head;                                             \
			while (actual->next) {                                                             \
				if (idx == 0) return actual;                                                   \
				actual = actual->next;                                                         \
				idx--;                                                                         \
			}                                                                                  \
		} else {                                                                               \
			LinkedNode##type* actual = list->tail;                                             \
			while (actual->prev) {                                                             \
				if (idx == (list->size - 1)) return actual;                                    \
				actual = actual->prev;                                                         \
				idx++;                                                                         \
			}                                                                                  \
		}                                                                                      \
                                                                                               \
		LOG_CONTAINER_ERROR(OutOfBounds);                                                      \
		return NULL;                                                                           \
	}                                                                                          \
                                                                                               \
	void LinkedList##type##Set(LinkedList##type* list, uint idx, type element) {               \
		LinkedNode##type* node = LinkedList##type##Get(list, idx);                             \
                                                                                               \
		if (node) node->data = element;                                                        \
	}                                                                                          \
                                                                                               \
	void LinkedList##type##Push(LinkedList##type* list, type element) {                        \
		if (!list) {                                                                           \
			LOG_CONTAINER_ERROR(NullContainer);                                                \
			return;                                                                            \
		}                                                                                      \
                                                                                               \
		LinkedNode##type* node = calloc(1, sizeof(LinkedNode##type));                          \
                                                                                               \
		if (!node) {                                                                           \
			LOG_CONTAINER_ERROR(MemAllocationFail);                                            \
			return;                                                                            \
		}                                                                                      \
                                                                                               \
		list->size++;                                                                          \
                                                                                               \
		if (!list->head) {                                                                     \
			list->head = node;                                                                 \
			return;                                                                            \
		}                                                                                      \
                                                                                               \
		if (!list->tail) {                                                                     \
			list->head->next = node;                                                           \
			node->prev = list->head;                                                           \
			list->tail = node;                                                                 \
			return;                                                                            \
		}                                                                                      \
                                                                                               \
		list->tail->next = node;                                                               \
		node->prev = list->tail;                                                               \
		list->tail = node;                                                                     \
	}                                                                                          \
                                                                                               \
	void LinkedList##type##Insert(LinkedList##type* list, uint idx, type element) {            \
		if (!list) {                                                                           \
			LOG_CONTAINER_ERROR(NullContainer);                                                \
			return;                                                                            \
		}                                                                                      \
                                                                                               \
		if (idx == list->size) {                                                               \
			LinkedList##type##Push(list, element);                                             \
			return;                                                                            \
		}                                                                                      \
                                                                                               \
		if (idx > list->size) {                                                                \
			LOG_CONTAINER_ERROR(OutOfBounds);                                                  \
			return;                                                                            \
		}                                                                                      \
                                                                                               \
		LinkedNode##type* node = calloc(1, sizeof(LinkedNode##type));                          \
                                                                                               \
		if (!node) {                                                                           \
			LOG_CONTAINER_ERROR(MemAllocationFail);                                            \
			return;                                                                            \
		}                                                                                      \
                                                                                               \
		if (idx == 0) {                                                                        \
			list->head->prev = node;                                                           \
			node->next = list->head;                                                           \
			list->head = node;                                                                 \
		} else if (idx == list->size - 1) {                                                    \
			list->tail->next = node;                                                           \
			node->prev = list->tail;                                                           \
			list->tail = node;                                                                 \
		} else {                                                                               \
			LinkedNode##type* temp = LinkedList##type##Get(list, idx);                         \
			if (!temp) return;                                                                 \
			node->prev = temp->prev;                                                           \
			node->next = temp;                                                                 \
			temp->prev->next = node;                                                           \
			temp->prev = node;                                                                 \
		}                                                                                      \
	}                                                                                          \
                                                                                               \
	void LinkedList##type##Erase(LinkedList##type* list, uint idx) {                           \
		if (!list) {                                                                           \
			LOG_CONTAINER_ERROR(NullContainer);                                                \
			return;                                                                            \
		}                                                                                      \
                                                                                               \
		if (idx >= list->size) {                                                               \
			LOG_CONTAINER_ERROR(OutOfBounds);                                                  \
			return;                                                                            \
		}                                                                                      \
                                                                                               \
		if (idx == 0) {                                                                        \
			list->size--;                                                                      \
			LinkedNode##type* newHead = list->head->next;                                      \
			if (list->size == 1) {                                                             \
				newHead = list->tail;                                                          \
				list->tail = NULL;                                                             \
			}                                                                                  \
			free(list->head);                                                                  \
			list->head = newHead;                                                              \
			list->head->prev = NULL;                                                           \
			return;                                                                            \
		}                                                                                      \
                                                                                               \
		if (idx == list->size - 1) {                                                           \
			list->size--;                                                                      \
			LinkedNode##type* newTail = list->tail->prev;                                      \
			free(list->tail);                                                                  \
			list->tail = newTail;                                                              \
			list->tail->next = NULL;                                                           \
			return;                                                                            \
		}                                                                                      \
                                                                                               \
		LinkedNode##type* node = LinkedList##type##Get(list, idx);                             \
                                                                                               \
		if (!node) return;                                                                     \
                                                                                               \
		node->prev->next = node->next;                                                         \
		node->next->prev = node->prev;                                                         \
		free(node);                                                                            \
                                                                                               \
		list->size--;                                                                          \
	}

DECLARE_LINKEDLIST_TYPE(byte)
DECLARE_LINKEDLIST_FUNCTIONS(byte)

#endif	// LU_LINKED_LIST_H
