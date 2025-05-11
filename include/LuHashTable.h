#ifndef LU_HASH_TABLE_H
#define LU_HASH_TABLE_H

#include "LuUtils.h"

typedef struct HashKey {
	const char* data;
	size_t size;
	size_t hash;
} HashKey;

/*
 * if "size" is 0 the hash will be equal to "str" itself (the pointer)
 * else if "size" is greater than strlen("str") size will be truncated to strlen("str")
 */
HashKey StrToHashKey(const char* str, size_t size);

/*
 * recomputes the hash of the provided key and overwrites the previous one
 */
void RehashKey(HashKey* key);

/*
 * checks if two keys are equal
 */
bool HashKeyEqu(HashKey a, HashKey b);

typedef enum HashNodeState { NODE_UNUSED = 0, NODE_USED, NODE_DELETED } HashNodeState;

#define DeclareHashTableType(type, name) \
	typedef struct name##Node {          \
		HashKey key;                     \
		type data;                       \
		HashNodeState state;             \
	} name##Node;                        \
                                         \
	typedef struct name {                \
		name##Node* nodes;               \
		size_t size;                     \
		size_t capacity;                 \
	} name

#define DeclareHashTableMethods(type, name)                                  \
	name* name##Create(size_t capacity);                                     \
	void name##Clear(name* table);                                           \
	void name##Free(name* table);                                            \
                                                                             \
	type* name##GetRef(const name* table, HashKey key);                      \
	type name##Get(const name* table, HashKey key);                          \
	void name##Set(name* table, HashKey key, type data);                     \
                                                                             \
	bool name##Push(name* table, HashKey key, const type data);              \
                                                                             \
	bool name##Erase(name* table, HashKey key);                              \
	bool name##EraseIfEqu(name* table, const type data, __compar_fn_t func); \
                                                                             \
	bool name##ShrinkToFit(name* table);                                     \
	bool name##Reserve(name* table, size_t capacity);                        \
	bool name##Grow(name* container);                                        \
                                                                             \
	void name##Map(name* table, void (*func)(type*));                        \
                                                                             \
	bool name##Contains(const name* table, HashKey key)

#define DefineHashTableMethods(type, name)                                                    \
	name* name##Create(size_t capacity) {                                                     \
		name* table = NULL;                                                                   \
                                                                                              \
		if (!Malloc(table, sizeof(name))) return NULL;                                        \
		if (capacity && !Calloc(table->nodes, capacity, sizeof(name##Node))) {                \
			FreeAndReturn(table, NULL);                                                       \
		}                                                                                     \
                                                                                              \
		table->capacity = capacity;                                                           \
		table->size = 0;                                                                      \
                                                                                              \
		return table;                                                                         \
	}                                                                                         \
                                                                                              \
	void name##Clear(name* table) {                                                           \
		Free(table->nodes);                                                                   \
		*table = (name){0};                                                                   \
	}                                                                                         \
                                                                                              \
	void name##Free(name* table) {                                                            \
		name##Clear(table);                                                                   \
		Free(table);                                                                          \
	}                                                                                         \
                                                                                              \
	type* name##GetRef(const name* table, HashKey key) {                                      \
		size_t hashIdx = key.hash;                                                            \
                                                                                              \
		for (size_t i = 0; i < table->capacity; i++) {                                        \
			size_t idx = (hashIdx + i) % table->capacity;                                     \
                                                                                              \
			if (table->nodes[idx].state == NODE_UNUSED) return NULL;                          \
			if (table->nodes[idx].state == NODE_DELETED) continue;                            \
			if (HashKeyEqu(key, table->nodes[idx].key)) return &(table->nodes[idx].data);     \
		}                                                                                     \
                                                                                              \
		return NULL;                                                                          \
	}                                                                                         \
                                                                                              \
	type name##Get(const name* table, HashKey key) { return *name##GetRef(table, key); }      \
                                                                                              \
	void name##Set(name* table, HashKey key, type data) { *name##GetRef(table, key) = data; } \
                                                                                              \
	bool name##Push(name* table, HashKey key, const type data) {                              \
		if (table->size == table->capacity) {                                                 \
			if (!name##Grow(table)) return false;                                             \
		}                                                                                     \
                                                                                              \
		size_t hashIdx = key.hash;                                                            \
                                                                                              \
		for (size_t i = 0; i < table->capacity; i++) {                                        \
			size_t idx = (hashIdx + i) % table->capacity;                                     \
                                                                                              \
			if (table->nodes[idx].state == NODE_USED) {                                       \
				if (HashKeyEqu(key, table->nodes[idx].key)) return false;                     \
				continue;                                                                     \
			}                                                                                 \
                                                                                              \
			table->nodes[idx].key = key;                                                      \
			table->nodes[idx].data = data;                                                    \
			table->nodes[idx].state = NODE_USED;                                              \
                                                                                              \
			table->size++;                                                                    \
			return true;                                                                      \
		}                                                                                     \
                                                                                              \
		return false;                                                                         \
	}                                                                                         \
                                                                                              \
	bool name##Erase(name* table, HashKey key) {                                              \
		size_t hashIdx = key.hash;                                                            \
                                                                                              \
		for (size_t i = 0; i < table->capacity; i++) {                                        \
			size_t idx = (hashIdx + i) % table->capacity;                                     \
                                                                                              \
			if (table->nodes[idx].state == NODE_UNUSED) return true;                          \
			if (table->nodes[idx].state == NODE_DELETED) continue;                            \
			if (!HashKeyEqu(key, table->nodes[idx].key)) continue;                            \
                                                                                              \
			table->nodes[idx].state = NODE_DELETED;                                           \
                                                                                              \
			table->size--;                                                                    \
			return true;                                                                      \
		}                                                                                     \
                                                                                              \
		if (table->size <= table->capacity / 2) return name##ShrinkToFit(table);              \
                                                                                              \
		return true;                                                                          \
	}                                                                                         \
                                                                                              \
	bool name##EraseIfEqu(name* table, const type data, __compar_fn_t func) {                 \
		bool success = false;                                                                 \
                                                                                              \
		for (int i = table->capacity - 1; i >= 0; i--) {                                      \
			if (table->nodes[i].state != NODE_USED) continue;                                 \
			if (func(&data, &(table->nodes[i].data))) continue;                               \
                                                                                              \
			table->nodes[i].state = NODE_DELETED;                                             \
                                                                                              \
			table->size--;                                                                    \
		}                                                                                     \
                                                                                              \
		if (table->size <= table->capacity / 2) return name##ShrinkToFit(table);              \
                                                                                              \
		return success;                                                                       \
	}                                                                                         \
                                                                                              \
	bool name##ChangeCapacity(name* table, size_t capacity) {                                 \
		if (!capacity) {                                                                      \
			name##Clear(table);                                                               \
			return true;                                                                      \
		}                                                                                     \
                                                                                              \
		if (!table->size) {                                                                   \
			if (!Realloc(table->nodes, capacity * sizeof(name##Node))) return false;          \
                                                                                              \
			memset(table->nodes, 0, capacity * sizeof(name##Node));                           \
			return true;                                                                      \
		}                                                                                     \
                                                                                              \
		capacity = Max(capacity, table->size);                                                \
                                                                                              \
		name##Node* prevNodes = table->nodes;                                                 \
		size_t prevCapacity = table->capacity;                                                \
                                                                                              \
		if (!Calloc(table->nodes, capacity, sizeof(name##Node))) {                            \
			table->nodes = prevNodes;                                                         \
			return false;                                                                     \
		} else {                                                                              \
			table->capacity = capacity;                                                       \
		}                                                                                     \
                                                                                              \
		for (size_t i = 0; i < prevCapacity; i++) {                                           \
			if (prevNodes[i].state != NODE_USED) continue;                                    \
                                                                                              \
			size_t hashIdx = prevNodes[i].key.hash % capacity;                                \
			for (size_t j = 0; j < capacity; j++) {                                           \
				size_t idx = (hashIdx + j) % capacity;                                        \
                                                                                              \
				if (table->nodes[idx].state == NODE_USED) continue;                           \
                                                                                              \
				table->nodes[idx] = prevNodes[i];                                             \
				break;                                                                        \
			}                                                                                 \
		}                                                                                     \
                                                                                              \
		Free(prevNodes);                                                                      \
                                                                                              \
		return true;                                                                          \
	}                                                                                         \
                                                                                              \
	bool name##ShrinkToFit(name* table) { return name##ChangeCapacity(table, table->size); }  \
                                                                                              \
	bool name##Reserve(name* table, size_t capacity) {                                        \
		if (capacity <= table->capacity) return true;                                         \
                                                                                              \
		size_t newCapa = table->capacity * 2;                                                 \
		while (newCapa < capacity) newCapa *= 2;                                              \
                                                                                              \
		return name##ChangeCapacity(table, newCapa);                                          \
	}                                                                                         \
                                                                                              \
	bool name##Grow(name* table) {                                                            \
		size_t newCapa = table->capacity * 2;                                                 \
		if (!newCapa) newCapa = 8;                                                            \
                                                                                              \
		return name##ChangeCapacity(table, newCapa);                                          \
	}                                                                                         \
                                                                                              \
	void name##Map(name* table, void (*func)(type*)) {                                        \
		for (size_t i = 0; i < table->capacity; i++) {                                        \
			if (table->nodes[i].state != NODE_USED) continue;                                 \
			func(&(table->nodes[i].data));                                                    \
		}                                                                                     \
	}                                                                                         \
                                                                                              \
	bool name##Contains(const name* table, HashKey key) { return name##GetRef(table, key) != NULL; }

#endif	// LU_HASH_TABLE_H
