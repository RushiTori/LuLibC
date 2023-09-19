#ifndef LU_HASH_MAP_H
#define LU_HASH_MAP_H

#include "LuBinaryTree.h"

int HashKey(const string key);

#define DECLARE_HASHMAP_TYPE(type)                    \
	typedef struct HashNode##type##_s {               \
		const string key;                             \
		type data;                                    \
	} HashNode##type;                                 \
                                                      \
	DECLARE_LINKEDLIST_TYPE(HashNode##type)           \
	DECLARE_BINARYTREE_TYPE(LinkedListHashNode##type) \
                                                      \
	typedef struct HashMap##type##_s {                \
		BinaryTreeLinkedListHashNode##type* data;     \
		uint size;                                    \
	} HashMap##type;

#define EmptyHashNode(type)        \
	(HashNode##type) {             \
		.key = NULL, .data = { 0 } \
	}

#define EmptyHashMap(type) \
	(HashMap##type) { .data = NULL, .size = 0 }

#define DECLARE_HASHMAP_FUNCTIONS(type)                                           \
                                                                                  \
	DECLARE_LINKEDLIST_FUNCTIONS(HashNode##type)                                  \
	DECLARE_BINARYTREE_FUNCTIONS(LinkedListHashNode##type)                        \
                                                                                  \
	LinkedListHashNode##type HashMap##type##AsList(HashMap##type* map);           \
                                                                                  \
	HashMap##type HashMap##type##Create();                                        \
	void HashMap##type##Clear(HashMap##type* map);                                \
	HashNode##type* HashMap##type##Get(HashMap##type* map, const string key);     \
	void HashMap##type##Set(HashMap##type* map, const string key, type element);  \
	void HashMap##type##Push(HashMap##type* map, const string key, type element); \
	void HashMap##type##Erase(HashMap##type* map, const string key);              \
	bool HashMap##type##Contains(HashMap##type* map, const string key);

#define DEFINE_HASHMAP_FUNCTIONS(type)                                                                                \
                                                                                                                      \
	DEFINE_LINKEDLIST_FUNCTIONS(HashNode##type)                                                                       \
	DEFINE_BINARYTREE_FUNCTIONS(LinkedListHashNode##type)                                                             \
                                                                                                                      \
	LinkedListHashNode##type HashMap##type##AsList(HashMap##type* map) {                                              \
		if (!map) {                                                                                                   \
			LOG_CONTAINER_ERROR(NullContainer);                                                                       \
			return EmptyLinkedList(HashNode##type);                                                                   \
		}                                                                                                             \
                                                                                                                      \
		if (!map->data) return EmptyLinkedList(HashNode##type);                                                       \
                                                                                                                      \
		LinkedListBinaryNodeLinkedListHashNode##type##Ptr temp =                                                      \
			BinaryTreeLinkedListHashNode##type##AsList(map->data);                                                    \
                                                                                                                      \
		LinkedListHashNode##type list = EmptyLinkedList(HashNode##type);                                              \
                                                                                                                      \
		LinkedNodeBinaryNodeLinkedListHashNode##type##Ptr* node = temp.head;                                          \
                                                                                                                      \
		while (node) {                                                                                                \
			LinkedNodeHashNode##type* tempNode = node->data->data.head;                                               \
			while (tempNode) {                                                                                        \
				LinkedListHashNode##type##Push(&list, tempNode->data);                                                \
				tempNode = tempNode->next;                                                                            \
			}                                                                                                         \
		}                                                                                                             \
                                                                                                                      \
		LinkedListBinaryNodeLinkedListHashNode##type##Ptr##Clear(&temp);                                              \
                                                                                                                      \
		return list;                                                                                                  \
	}                                                                                                                 \
                                                                                                                      \
	HashMap##type HashMap##type##Create() { return EmptyHashMap(type); }                                              \
                                                                                                                      \
	void HashMap##type##Clear(HashMap##type* map) {                                                                   \
		if (!map) {                                                                                                   \
			LOG_CONTAINER_ERROR(NullContainer);                                                                       \
			return;                                                                                                   \
		}                                                                                                             \
                                                                                                                      \
		map->size = 0;                                                                                                \
                                                                                                                      \
		if (!map->data) return;                                                                                       \
                                                                                                                      \
		LinkedListBinaryNodeLinkedListHashNode##type##Ptr temp =                                                      \
			BinaryTreeLinkedListHashNode##type##AsList(map->data);                                                    \
                                                                                                                      \
		LinkedNodeBinaryNodeLinkedListHashNode##type##Ptr* node = temp.head;                                          \
                                                                                                                      \
		while (node) {                                                                                                \
			LinkedListHashNode##type##Clear(&node->data->data);                                                       \
			node = node->next;                                                                                        \
		}                                                                                                             \
                                                                                                                      \
		LinkedListBinaryNodeLinkedListHashNode##type##Ptr##Clear(&temp);                                              \
                                                                                                                      \
		BinaryTreeLinkedListHashNode##type##Clear(map->data);                                                         \
                                                                                                                      \
		free(map->data);                                                                                              \
		map->data = NULL;                                                                                             \
	}                                                                                                                 \
                                                                                                                      \
	HashNode##type* HashMap##type##Get(HashMap##type* map, const string key) {                                        \
		if (!map) {                                                                                                   \
			LOG_CONTAINER_ERROR(NullContainer);                                                                       \
			return NULL;                                                                                              \
		}                                                                                                             \
                                                                                                                      \
		if (!map->data) {                                                                                             \
			LOG_CONTAINER_ERROR(OutOfBounds);                                                                         \
			return NULL;                                                                                              \
		}                                                                                                             \
                                                                                                                      \
		int hashedKey = HashKey(key);                                                                                 \
                                                                                                                      \
		BinaryNodeLinkedListHashNode##type* tempNode = BinaryTreeLinkedListHashNode##type##Get(map->data, hashedKey); \
                                                                                                                      \
		if (!tempNode) return NULL;                                                                                   \
                                                                                                                      \
		LinkedNodeHashNode##type* node = tempNode->data.head;                                                         \
                                                                                                                      \
		while (node) {                                                                                                \
			if (strcmp(node->data.key, key) == 0) return &node->data;                                                 \
			node = node->next;                                                                                        \
		}                                                                                                             \
                                                                                                                      \
		LOG_CONTAINER_ERROR(OutOfBounds);                                                                             \
		return NULL;                                                                                                  \
	}                                                                                                                 \
                                                                                                                      \
	void HashMap##type##Set(HashMap##type* map, const string key, type element) {                                     \
		HashNode##type* node = HashMap##type##Get(map, key);                                                          \
		if (!node) return;                                                                                            \
		node->data = element;                                                                                         \
	}                                                                                                                 \
                                                                                                                      \
	void HashMap##type##Push(HashMap##type* map, const string key, type element) {                                    \
		if (!map) {                                                                                                   \
			LOG_CONTAINER_ERROR(NullContainer);                                                                       \
			return;                                                                                                   \
		}                                                                                                             \
                                                                                                                      \
		if (!map->data) {                                                                                             \
			map->data = calloc(1, sizeof(BinaryTreeLinkedListHashNode##type));                                        \
			if (!map->data) {                                                                                         \
				LOG_CONTAINER_ERROR(MemAllocationFail);                                                               \
				return;                                                                                               \
			}                                                                                                         \
		}                                                                                                             \
                                                                                                                      \
		int hashedKey = HashKey(key);                                                                                 \
                                                                                                                      \
		if (BinaryTreeLinkedListHashNode##type##Contains(map->data, hashedKey)) {                                     \
			BinaryNodeLinkedListHashNode##type* temp = BinaryTreeLinkedListHashNode##type##Get(map->data, hashedKey); \
			LinkedListHashNode##type##Push(&temp->data, (HashNode##type){.key = key, .data = element});               \
		} else {                                                                                                      \
			LinkedListHashNode##type temp = EmptyLinkedList(HashNode##type);                                          \
			LinkedListHashNode##type##Push(&temp, (HashNode##type){.key = key, .data = element});                     \
			BinaryTreeLinkedListHashNode##type##Push(map->data, hashedKey, temp);                                     \
		}                                                                                                             \
	}                                                                                                                 \
                                                                                                                      \
	void HashMap##type##Erase(HashMap##type* map, const string key) {                                                 \
		if (!map) {                                                                                                   \
			LOG_CONTAINER_ERROR(NullContainer);                                                                       \
			return;                                                                                                   \
		}                                                                                                             \
                                                                                                                      \
		if (!map->data) {                                                                                             \
			LOG_CONTAINER_ERROR(OutOfBounds);                                                                         \
			return;                                                                                                   \
		}                                                                                                             \
                                                                                                                      \
		int hashedKey = HashKey(key);                                                                                 \
                                                                                                                      \
		BinaryNodeLinkedListHashNode##type* tempNode = BinaryTreeLinkedListHashNode##type##Get(map->data, hashedKey); \
                                                                                                                      \
		if (!tempNode) return;                                                                                        \
                                                                                                                      \
		LinkedNodeHashNode##type* node = tempNode->data.head;                                                         \
                                                                                                                      \
		uint idx = 0;                                                                                                 \
                                                                                                                      \
		while (node) {                                                                                                \
			if (strcmp(node->data.key, key) == 0) break;                                                              \
			node = node->next;                                                                                        \
			idx++;                                                                                                    \
		}                                                                                                             \
                                                                                                                      \
		if (!node) {                                                                                                  \
			LOG_CONTAINER_ERROR(OutOfBounds);                                                                         \
			return;                                                                                                   \
		}                                                                                                             \
                                                                                                                      \
		LinkedListHashNode##type##Erase(&tempNode->data, idx);                                                        \
                                                                                                                      \
		if (!tempNode->data.size) {                                                                                   \
			BinaryTreeLinkedListHashNode##type##Erase(map->data, hashedKey);                                          \
		}                                                                                                             \
	}                                                                                                                 \
                                                                                                                      \
	bool HashMap##type##Contains(HashMap##type* map, const string key) {                                              \
		if (!map) {                                                                                                   \
			LOG_CONTAINER_ERROR(NullContainer);                                                                       \
			return false;                                                                                             \
		}                                                                                                             \
                                                                                                                      \
		if (!map->data) {                                                                                             \
			LOG_CONTAINER_ERROR(OutOfBounds);                                                                         \
			return false;                                                                                             \
		}                                                                                                             \
                                                                                                                      \
		int hashedKey = HashKey(key);                                                                                 \
                                                                                                                      \
		if (!BinaryTreeLinkedListHashNode##type##Contains(map->data, hashedKey)) return false;                        \
                                                                                                                      \
		BinaryNodeLinkedListHashNode##type* tempNode = BinaryTreeLinkedListHashNode##type##Get(map->data, hashedKey); \
                                                                                                                      \
		if (!tempNode) return false;                                                                                  \
                                                                                                                      \
		LinkedNodeHashNode##type* node = tempNode->data.head;                                                         \
                                                                                                                      \
		while (node) {                                                                                                \
			if (strcmp(node->data.key, key) == 0) return true;                                                        \
			node = node->next;                                                                                        \
		}                                                                                                             \
                                                                                                                      \
		return false;                                                                                                 \
	}

DECLARE_HASHMAP_TYPE(byte)
DECLARE_HASHMAP_FUNCTIONS(byte)

#endif	// LU_HASH_MAP_H
