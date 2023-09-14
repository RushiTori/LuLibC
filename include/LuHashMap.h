#ifndef L_HASH_MAP_H
#define L_HASH_MAP_H

#include "LuBinaryTree.h"

typedef struct HashNode {
	const string key;
	void* data;
} HashNode;

#define EmptyHashNode \
	(HashNode) { .key = NULL, .data = NULL }

typedef struct HashMap {
	BinaryTree* data;
	uint dataSize;
	uint elementCount;
	bool containsPointers;
} HashMap;

#define EmptyHashMap \
	(HashMap) { .data = NULL, .dataSize = 0, .elementCount = 0, .containsPointers = false }

int hm_HashString(const string str);

HashMap hm_Create(uint dataSize, bool isDataPointers);
void hm_Clear(HashMap* map, bool clear_recursive);

void* hm_Get(HashMap* map, const string key);
void hm_Set(HashMap* map, const string key, void* data_);

void hm_Push(HashMap* map, const string key, void* data_);
void hm_Remove(HashMap* map, const string key, bool clear_recursive);

bool hm_Contains(HashMap* map, const string key);

#endif	// L_HASH_MAP_H
