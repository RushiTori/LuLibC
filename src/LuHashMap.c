#include "LuHashMap.h"

int HashKey(const string key) {
	if (!key || !(*key)) return 0;
	int result = 0;
	uint idx = 0;
	int power = 1;

	while (idx < sizeof(int) && key[idx]) {
		result += key[idx] * power;
		idx++;
		power *= 256;
	}

	while (key[idx]) {
		result += key[idx] * idx * ((idx % 2) ? 17 : 11);
		idx++;
	}
	return result;
}

DEFINE_HASHMAP_FUNCTIONS(byte)
