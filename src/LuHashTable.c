#include "LuHashTable.h"

HashKey StrToHashKey(const char* str, size_t size) {
	HashKey key;
	key.data = str;
	key.size = Min(size, strlen(str));
	key.hash = 0;

	RehashKey(&key);
	return key;
}

void RehashKey(HashKey* key) {
	// WIP
	if (!key->size) {
		key->hash = (size_t)key->data;
	} else {
		/*
		 * xor djb2 Hash function
		 * http://www.cse.yorku.ca/~oz/hash.html
		 */
		key->hash = 5381;

		for (size_t i = 0; key->data[i] && i < key->size; i++) {
			key->hash = ((key->hash << 5) + key->hash) ^ key->data[i];
		}
	}
}

bool HashKeyEqu(HashKey a, HashKey b) {
	if (a.hash != b.hash) return false;
	if (a.size != b.size) return false;

	return !strncmp(a.data, b.data, a.size);
}
