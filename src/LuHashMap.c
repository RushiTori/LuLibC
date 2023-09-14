#include "LuHashMap.h"

#include "LuLinkedList.h"
#include "LuLogs.h"

int hm_HashString(const string str) {
	if (!str || !(*str)) return 0;
	int result = 0;
	uint idx = 0;
	int power = 1;

	while (idx < sizeof(int) && str[idx]) {
		result += str[idx] * power;
		idx++;
		power *= 256;
	}

	while (str[idx]) {
		result += str[idx] * idx * ((idx % 2) ? 17 : 11);
		idx++;
	}
	return result;
}

HashMap hm_Create(uint dataSize, bool isDataPointers) {
	HashMap temp;
	temp.data = NULL;
	temp.dataSize = dataSize;
	temp.elementCount = 0;
	temp.containsPointers = isDataPointers;
	return temp;
}

void hm_Clear(HashMap* map, bool clear_recursive) {
	if (!map) {
		LOG_CONTAINER_ERROR(NullContainer);
		return;
	}

	if (!map->dataSize) {
		LOG_CONTAINER_ERROR(NoDataSize);
	}

	if (!map->data || !map->elementCount) return;

	while (map->data->root) {
		LinkedList* tempList = map->data->root->data;
		if (tempList) {
			LinkedNode* tempNode = tempList->begin;
			while (tempNode) {
				HashNode* tempData = tempNode->data;
				if (tempData) {
					if (clear_recursive) {
						if (map->containsPointers) {
							free(*((void**)tempData->data));
						}
					}
					free(tempData->data);
				} else {
					// Unknown Pointer Issue
				}
				tempNode = tempNode->next;
			}
			ll_Clear(tempList, false);
		} else {
			// Unknown Pointer Issue
		}
		bt_Remove(map->data, map->data->root->key, false);
	}

	free(map->data);
	map->data = NULL;
	map->elementCount = 0;
}

void* hm_Get(HashMap* map, const string key) {
	if (!hm_Contains(map, key)) {
		LOG_CONTAINER_ERROR(KeyNotFound);
		return NULL;
	}

	int hashedKey = hm_HashString(key);
	if (!bt_Contains(map->data, hashedKey)) return NULL;

	BinaryNode* tempNode = bt_Get(map->data, hashedKey);
	if (!tempNode) return NULL;	 // Unknown Pointer Issue

	LinkedList* tempList = tempNode->data;
	if (!tempList) return NULL;	 // Unknown Pointer Issue

	LinkedNode* node = tempList->begin;
	while (node) {
		HashNode* tempData = node->data;
		if (tempData) {
			if (strcmp(tempData->key, key) == 0) return tempData->data;
		}
		node = node->next;
	}

	return NULL;
}

void hm_Set(HashMap* map, const string key, void* data_) {
	if (!data_) {
		LOG_CONTAINER_ERROR(NullDataPassed);
		return;
	}

	if (!map->dataSize) {
		LOG_CONTAINER_ERROR(NoDataSize);
		return;
	}

	void* baseData = hm_Get(map, key);
	if (!baseData) return;	// Unknown Pointer Issue
	memcpy(baseData, data_, map->dataSize);
}

void hm_Push(HashMap* map, const string key, void* data_) {
	if (!data_) {
		LOG_CONTAINER_ERROR(NullDataPassed);
		return;
	}

	if (!map->dataSize) {
		LOG_CONTAINER_ERROR(NoDataSize);
		return;
	}

	if (hm_Contains(map, key)) {
		LOG_CONTAINER_ERROR(KeyAlreadyExists);
		return;
	}

	int hashedKey = hm_HashString(key);

	if (!map->data) {
		map->data = malloc(sizeof(BinaryTree));
		if (!map->data) {
			LOG_CONTAINER_ERROR(MemAllocationFail);
			return;
		}
		*(map->data) = bt_Create(sizeof(LinkedList), false);
		HashNode tempNode = (HashNode){key, malloc(map->dataSize)};
		if (!tempNode.data) {
			LOG_CONTAINER_ERROR(MemAllocationFail);
			free(map->data);
			return;
		}
		memcpy(tempNode.data, data_, map->dataSize);
		LinkedList tempList = ll_Create(sizeof(HashNode), map->containsPointers);
		ll_Push(&tempList, &tempNode);
		bt_Push(map->data, hashedKey, &tempList);
		map->elementCount++;
		return;
	}

	HashNode tempNode = (HashNode){key, malloc(map->dataSize)};
	if (!tempNode.data) {
		LOG_CONTAINER_ERROR(MemAllocationFail);
		return;
	}
	memcpy(tempNode.data, data_, map->dataSize);

	if (bt_Contains(map->data, hashedKey)) {
		LinkedList* tempList = bt_Get(map->data, hashedKey)->data;
		if (!tempList) return;	// Unknown Pointer Issue
		ll_Push(tempList, &tempNode);
	} else {
		LinkedList tempList = ll_Create(sizeof(HashNode), map->containsPointers);
		ll_Push(&tempList, &tempNode);
		bt_Push(map->data, hashedKey, &tempList);
	}

	map->elementCount++;
}

void hm_Remove(HashMap* map, const string key, bool clear_recursive) {
	if (!hm_Contains(map, key)) {
		LOG_CONTAINER_ERROR(KeyNotFound);
		return;
	}

	int hashedKey = hm_HashString(key);

	BinaryNode* tempNode = bt_Get(map->data, hashedKey);
	if (!tempNode) return;	// Unknown Pointer Issue

	LinkedList* tempList = tempNode->data;
	if (!tempList) return;	// Unknown Pointer Issue

	uint keyIdx = 0;
	LinkedNode* node = tempList->begin;
	while (node) {
		HashNode* tempData = node->data;
		if (tempData) {
			if (strcmp(tempData->key, key) == 0) {
				if (clear_recursive) {
					if (map->containsPointers) {
						free(*((void**)tempData->data));
					}
				}
				free(tempData->data);
				break;
			}
		} else {
			// Unknown Pointer Issue
		}
		node = node->next;
		keyIdx++;
	}

	if (keyIdx >= tempList->elementCount) {
		LOG_CONTAINER_ERROR(KeyNotFound);  // The user messed up on this one
		return;
	}

	ll_Remove(tempList, keyIdx, false);

	if (tempList->elementCount == 0) {
		bt_Remove(map->data, hashedKey, false);
	}

	map->elementCount--;
	if (map->elementCount == 0) {
		free(map->data);
		map->data = NULL;
	}
}

bool hm_Contains(HashMap* map, const string key) {
	if (!map) {
		LOG_CONTAINER_ERROR(NullContainer);
		return false;
	}

	if (!map->dataSize) {
		LOG_CONTAINER_ERROR(NoDataSize);
		return false;
	}

	if (!map->data || !map->elementCount) return false;

	int hashedKey = hm_HashString(key);
	if (!bt_Contains(map->data, hashedKey)) return false;

	BinaryNode* tempNode = bt_Get(map->data, hashedKey);
	if (!tempNode) return false;  // Unknown Pointer Issue

	LinkedList* tempList = tempNode->data;
	if (!tempList) return false;  // Unknown Pointer Issue

	LinkedNode* node = tempList->begin;
	while (node) {
		HashNode* tempData = node->data;
		if (tempData) {
			if (strcmp(tempData->key, key) == 0) return true;
		} else {
			// Unknown Pointer Issue
		}
		node = node->next;
	}

	return false;
}
