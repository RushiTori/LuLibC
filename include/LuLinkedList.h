#ifndef LU_LINKED_LIST_H
#define LU_LINKED_LIST_H

#include "LuUtils.h"

#define DeclareLinkedListType(type, name) \
	typedef struct name##Node {           \
		struct name##Node* prev;          \
		struct name##Node* next;          \
		type data;                        \
	} name##Node;                         \
                                          \
	typedef struct name {                 \
		name##Node* nodes;                \
		size_t size;                      \
	} name

#define DeclareLinkedListMethods(type, name)                                        \
	name* name##Create();                                                           \
	void name##Clear(name* list);                                                   \
	void name##Free(name* list);                                                    \
                                                                                    \
	type* name##GetRef(const name* list, size_t index);                             \
	type name##Get(const name* list, size_t index);                                 \
	void name##Set(name* list, size_t index, type data);                            \
                                                                                    \
	bool name##Push(name* list, type data);                                         \
	bool name##PushAll(name* list, const type* data, size_t count);                 \
                                                                                    \
	bool name##Insert(name* list, size_t index, type data);                         \
	bool name##InsertAll(name* list, size_t index, const type* data, size_t count); \
                                                                                    \
	bool name##Erase(name* list, size_t index);                                     \
	bool name##EraseIfEqu(name* list, type data, CompareFunc func);                 \
	bool name##EraseAll(name* list, size_t index, size_t count);                    \
	bool name##EraseRange(name* list, size_t start, size_t end);                    \
                                                                                    \
	void name##Map(name* list, void (*func)(type*));                                \
	void name##Sort(name* list, CompareFunc func);                                  \
                                                                                    \
	bool name##Contains(const name* list, type data, CompareFunc func);             \
	size_t name##IndexOf(const name* list, type data, CompareFunc func);            \
	size_t name##LastIndexOf(const name* list, type data, CompareFunc func)

#define DefineLinkedListMethods(type, name)                                                            \
	name##Node* name##NodeCreate(name##Node* prev, name##Node* next, type data) {                      \
		name##Node* node = NULL;                                                                       \
		if (!Malloc(node, sizeof(name##Node))) return NULL;                                            \
                                                                                                       \
		node->prev = prev;                                                                             \
		node->next = next;                                                                             \
		node->data = data;                                                                             \
                                                                                                       \
		if (prev) prev->next = node;                                                                   \
		if (next) next->prev = node;                                                                   \
                                                                                                       \
		return node;                                                                                   \
	}                                                                                                  \
                                                                                                       \
	name##Node* name##NodeCreateFromArray(const type* data, size_t count) {                            \
		if (!count) return NULL;                                                                       \
                                                                                                       \
		name##Node* nodes = name##NodeCreate(NULL, NULL, *data);                                       \
		if (!nodes) return NULL;                                                                       \
		if (count == 1) return nodes;                                                                  \
                                                                                                       \
		name##Node* tmp = nodes;                                                                       \
		for (size_t i = 1; i < count; i++) {                                                           \
			tmp->next = name##NodeCreate(tmp, NULL, data[i]);                                          \
                                                                                                       \
			if (!tmp->next) {                                                                          \
				while (nodes) {                                                                        \
					tmp = nodes;                                                                       \
					nodes = nodes->next;                                                               \
					Free(tmp);                                                                         \
				}                                                                                      \
                                                                                                       \
				return NULL;                                                                           \
			}                                                                                          \
                                                                                                       \
			tmp = tmp->next;                                                                           \
		}                                                                                              \
                                                                                                       \
		nodes->prev = tmp;                                                                             \
		tmp->next = nodes;                                                                             \
                                                                                                       \
		return nodes;                                                                                  \
	}                                                                                                  \
                                                                                                       \
	void name##NodeFree(name##Node* node) {                                                            \
		name##Node* prev = node->prev;                                                                 \
		name##Node* next = node->next;                                                                 \
                                                                                                       \
		if (prev) prev->next = next;                                                                   \
		if (next) next->prev = prev;                                                                   \
                                                                                                       \
		Free(node);                                                                                    \
	}                                                                                                  \
                                                                                                       \
	name* name##Create() {                                                                             \
		name* list = NULL;                                                                             \
                                                                                                       \
		return Calloc(list, 1, sizeof(name));                                                          \
	}                                                                                                  \
                                                                                                       \
	void name##Clear(name* list) {                                                                     \
		name##Node* tail = NULL;                                                                       \
		if (list->nodes) tail = list->nodes->prev;                                                     \
                                                                                                       \
		while (list->nodes != tail) {                                                                  \
			name##Node* node = list->nodes;                                                            \
			list->nodes = list->nodes->next;                                                           \
			Free(node);                                                                                \
		}                                                                                              \
                                                                                                       \
		Free(tail);                                                                                    \
		*list = (name){0};                                                                             \
	}                                                                                                  \
                                                                                                       \
	void name##Free(name* list) {                                                                      \
		name##Clear(list);                                                                             \
		Free(list);                                                                                    \
	}                                                                                                  \
                                                                                                       \
	name##Node* name##GetNode(const name* list, size_t index) {                                        \
		name##Node* node = list->nodes;                                                                \
                                                                                                       \
		if (index > list->size / 2) {                                                                  \
			index -= list->size - 1;                                                                   \
                                                                                                       \
			do node = node->prev;                                                                      \
			while (index--);                                                                           \
		} else {                                                                                       \
			while (index--) node = node->next;                                                         \
		}                                                                                              \
                                                                                                       \
		return node;                                                                                   \
	}                                                                                                  \
                                                                                                       \
	type* name##GetRef(const name* list, size_t index) { return &(name##GetNode(list, index)->data); } \
                                                                                                       \
	type name##Get(const name* list, size_t index) { return *name##GetRef(list, index); }              \
                                                                                                       \
	void name##Set(name* list, size_t index, type data) { *name##GetRef(list, index) = data; }         \
                                                                                                       \
	bool name##Push(name* list, type data) {                                                           \
		if (!list->size) {                                                                             \
			list->nodes = name##NodeCreate(NULL, NULL, data);                                          \
			if (!list->nodes) return false;                                                            \
			list->size++;                                                                              \
			return true;                                                                               \
		}                                                                                              \
                                                                                                       \
		name##Node* prev = (list->size > 1) ? list->nodes->prev : list->nodes;                         \
		name##Node* next = list->nodes;                                                                \
		name##Node* node = name##NodeCreate(prev, next, data);                                         \
                                                                                                       \
		if (!node) return false;                                                                       \
		list->nodes->prev = node;                                                                      \
		list->size++;                                                                                  \
                                                                                                       \
		return true;                                                                                   \
	}                                                                                                  \
                                                                                                       \
	bool name##PushAll(name* list, const type* data, size_t count) {                                   \
		if (!count) return true;                                                                       \
		if (count == 1) return name##Push(list, *data);                                                \
                                                                                                       \
		name##Node* nodes = name##NodeCreateFromArray(data, count);                                    \
		if (!nodes) return false;                                                                      \
                                                                                                       \
		list->size += count;                                                                           \
                                                                                                       \
		if (!list->size) {                                                                             \
			list->nodes = nodes;                                                                       \
		} else {                                                                                       \
			name##Node* listHead = list->nodes;                                                        \
			name##Node* listTail = (listHead->prev) ? listHead->prev : listHead;                       \
			name##Node* nodesHead = nodes;                                                             \
			name##Node* nodesTail = nodesHead->prev;                                                   \
                                                                                                       \
			listTail->next = nodesHead;                                                                \
			nodesHead->prev = listTail;                                                                \
			listHead->prev = nodesTail;                                                                \
			nodesTail->next = listHead;                                                                \
		}                                                                                              \
                                                                                                       \
		return true;                                                                                   \
	}                                                                                                  \
                                                                                                       \
	bool name##Insert(name* list, size_t index, type data) {                                           \
		if (!list->size) {                                                                             \
			list->nodes = name##NodeCreate(NULL, NULL, data);                                          \
			if (!list->nodes) return false;                                                            \
			list->size++;                                                                              \
			return true;                                                                               \
		}                                                                                              \
                                                                                                       \
		if (index >= list->size) return name##Push(list, data);                                        \
                                                                                                       \
		name##Node* next = name##GetNode(list, index);                                                 \
		name##Node* prev = next->prev;                                                                 \
		name##Node* node = name##NodeCreate(prev, next, data);                                         \
                                                                                                       \
		if (!node) return false;                                                                       \
		if (!index) list->nodes = node;                                                                \
                                                                                                       \
		return true;                                                                                   \
	}                                                                                                  \
                                                                                                       \
	bool name##InsertAll(name* list, size_t index, const type* data, size_t count) {                   \
		if (!count) return true;                                                                       \
		if (count == 1) return name##Insert(list, index, *data);                                       \
		if (index >= list->size) return name##PushAll(list, data, count);                              \
                                                                                                       \
		name##Node* nodes = name##NodeCreateFromArray(data, count);                                    \
		if (!nodes) return false;                                                                      \
                                                                                                       \
		list->size += count;                                                                           \
                                                                                                       \
		name##Node* next = name##GetNode(list, index);                                                 \
		name##Node* prev = next->prev;                                                                 \
                                                                                                       \
		nodes->prev->next = next;                                                                      \
		next->prev = nodes->prev;                                                                      \
                                                                                                       \
		prev->next = nodes;                                                                            \
		nodes->prev = prev;                                                                            \
                                                                                                       \
		return true;                                                                                   \
	}                                                                                                  \
                                                                                                       \
	bool name##Erase(name* list, size_t index) {                                                       \
		if (index >= list->size) return false;                                                         \
                                                                                                       \
		name##Node* node = name##GetNode(list, index);                                                 \
		name##Node* next = node->next;                                                                 \
		name##NodeFree(node);                                                                          \
                                                                                                       \
		if (!index) list->nodes = next;                                                                \
		list->size--;                                                                                  \
                                                                                                       \
		return true;                                                                                   \
	}                                                                                                  \
                                                                                                       \
	bool name##EraseIfEqu(name* list, type data, CompareFunc func) {                                   \
		bool modifiedList = false;                                                                     \
                                                                                                       \
		while (list->nodes) {                                                                          \
			name##Node* node = list->nodes;                                                            \
			name##Node* next = node->next;                                                             \
                                                                                                       \
			if (!func(&data, &(node->data))) break;                                                    \
                                                                                                       \
			name##NodeFree(node);                                                                      \
			node = next;                                                                               \
			list->size--;                                                                              \
			modifiedList = true;                                                                       \
		}                                                                                              \
                                                                                                       \
		if (!list->size) return modifiedList;                                                          \
                                                                                                       \
		name##Node* node = list->nodes->next;                                                          \
		while (node && node != list->nodes) {                                                          \
			name##Node* next = node->next;                                                             \
                                                                                                       \
			if (func(&data, &(node->data))) {                                                          \
				name##NodeFree(node);                                                                  \
				list->size--;                                                                          \
				modifiedList = true;                                                                   \
			}                                                                                          \
                                                                                                       \
			node = next;                                                                               \
		}                                                                                              \
                                                                                                       \
		return modifiedList;                                                                           \
	}                                                                                                  \
                                                                                                       \
	bool name##EraseAll(name* list, size_t index, size_t count) {                                      \
		if (!count) return true;                                                                       \
		if (count == 1) return name##Erase(list, index);                                               \
		if (index >= list->size) return false;                                                         \
		if (!index && count >= list->size) {                                                           \
			name##Clear(list);                                                                         \
			return true;                                                                               \
		}                                                                                              \
		count = Min(count, list->size - index);                                                        \
                                                                                                       \
		name##Node* node = name##GetNode(list, index);                                                 \
                                                                                                       \
		while (count--) {                                                                              \
			name##Node* next = node->next;                                                             \
			name##NodeFree(node);                                                                      \
			node = next;                                                                               \
		}                                                                                              \
                                                                                                       \
		if (!index) list->nodes = node;                                                                \
                                                                                                       \
		list->size -= count;                                                                           \
		return true;                                                                                   \
	}                                                                                                  \
                                                                                                       \
	bool name##EraseRange(name* list, size_t start, size_t end) {                                      \
		size_t trueStart = Min(start, end);                                                            \
		size_t trueEnd = Max(start, end);                                                              \
		size_t count = trueEnd - trueStart;                                                            \
                                                                                                       \
		return name##EraseAll(list, trueStart, count);                                                 \
	}                                                                                                  \
                                                                                                       \
	void name##Map(name* list, void (*func)(type*)) {                                                  \
		name##Node* node = list->nodes;                                                                \
                                                                                                       \
		while (node) {                                                                                 \
			func(&(node->data));                                                                       \
			node = node->next;                                                                         \
			if (node == list->nodes) break;                                                            \
		}                                                                                              \
	}                                                                                                  \
                                                                                                       \
	name name##Merge(name a, name b, CompareFunc func) {                                               \
		name sorted = (name){0};                                                                       \
		sorted.size = a.size + b.size;                                                                 \
                                                                                                       \
		if (func(&(a.nodes->data), &(b.nodes->data)) <= 0) {                                           \
			sorted.nodes = a.nodes;                                                                    \
			a.nodes = a.nodes->next;                                                                   \
			a.size--;                                                                                  \
		} else {                                                                                       \
			sorted.nodes = b.nodes;                                                                    \
			b.nodes = b.nodes->next;                                                                   \
			b.size--;                                                                                  \
		}                                                                                              \
                                                                                                       \
		name##Node* node = sorted.nodes;                                                               \
                                                                                                       \
		while (a.size && b.size) {                                                                     \
			name##Node* next = a.nodes;                                                                \
                                                                                                       \
			if (b.size) {                                                                              \
				if (a.size) {                                                                          \
					if (func(&(a.nodes->data), &(b.nodes->data)) > 0) next = b.nodes;                  \
				} else {                                                                               \
					next = b.nodes;                                                                    \
				}                                                                                      \
			}                                                                                          \
                                                                                                       \
			if (next == a.nodes) {                                                                     \
				a.nodes = a.nodes->next;                                                               \
				a.size--;                                                                              \
			} else {                                                                                   \
				b.nodes = b.nodes->next;                                                               \
				b.size--;                                                                              \
			}                                                                                          \
                                                                                                       \
			node->next = next;                                                                         \
			next->prev = node;                                                                         \
		}                                                                                              \
                                                                                                       \
		sorted.nodes->prev = node;                                                                     \
		node->next = sorted.nodes->next;                                                               \
                                                                                                       \
		return sorted;                                                                                 \
	}                                                                                                  \
                                                                                                       \
	void name##Sort(name* list, CompareFunc func) {                                                    \
		if (list->size <= 1) return;                                                                   \
		size_t mid = list->size / 2;                                                                   \
                                                                                                       \
		name a = (name){.nodes = list->nodes, .size = mid};                                            \
		name b = (name){.nodes = name##GetNode(list, mid), .size = list->size - mid};                  \
                                                                                                       \
		name##Sort(&a, func);                                                                          \
		name##Sort(&b, func);                                                                          \
		*list = name##Merge(a, b, func);                                                               \
	}                                                                                                  \
                                                                                                       \
	bool name##Contains(const name* list, type data, CompareFunc func) {                               \
		if (!list->size) return false;                                                                 \
                                                                                                       \
		name##Node* node = list->nodes;                                                                \
		do {                                                                                           \
			if (!func(&data, &(node->data))) return true;                                              \
			node = node->next;                                                                         \
		} while (node && node != list->nodes);                                                         \
                                                                                                       \
		return false;                                                                                  \
	}                                                                                                  \
                                                                                                       \
	size_t name##IndexOf(const name* list, type data, CompareFunc func) {                              \
		if (!list->size) return list->size;                                                            \
                                                                                                       \
		size_t idx = 0;                                                                                \
		name##Node* node = list->nodes;                                                                \
                                                                                                       \
		do {                                                                                           \
			if (!func(&data, &(node->data))) return idx;                                               \
			node = node->next;                                                                         \
			idx++;                                                                                     \
		} while (node && node != list->nodes);                                                         \
                                                                                                       \
		return list->size;                                                                             \
	}                                                                                                  \
                                                                                                       \
	size_t name##LastIndexOf(const name* list, type data, CompareFunc func) {                          \
		if (!list->size) return list->size;                                                            \
                                                                                                       \
		if (list->size == 1) {                                                                         \
			if (!func(&data, &(list->nodes->data))) return 0;                                          \
			return list->size;                                                                         \
		}                                                                                              \
                                                                                                       \
		size_t idx = list->size - 1;                                                                   \
		name##Node* node = list->nodes->prev;                                                          \
                                                                                                       \
		do {                                                                                           \
			if (!func(&data, &(node->data))) return idx;                                               \
			node = node->prev;                                                                         \
			idx--;                                                                                     \
		} while (node && node != list->nodes->prev);                                                   \
                                                                                                       \
		return list->size;                                                                             \
	}

#endif	// LU_LINKED_LIST_H
