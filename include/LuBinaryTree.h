#ifndef L_BINARY_TREE_H
#define L_BINARY_TREE_H

#include "LuUtils.h"

typedef struct BinaryNode {
	// struct BinaryNode* prev;
	struct BinaryNode* left;
	struct BinaryNode* right;
	int key;
	void* data;
} BinaryNode;

typedef struct BinaryTree {
	BinaryNode* root;
	uint dataSize;
	uint elementCount;
	bool containsPointers;
} BinaryTree;

BinaryTree bt_Create(uint dataSize, bool isDataPointers);
void bt_Clear(BinaryTree* tree, bool clear_recursive);

BinaryNode* bt_Get(BinaryTree* tree, int key);
void bt_Set(BinaryTree* tree, int key, void* data_);

void bt_Push(BinaryTree* tree, int key, void* data_);
void bt_Remove(BinaryTree* tree, int key, bool clear_recursive);

bool bt_Contains(BinaryTree* tree, int key);

#endif	// L_BINARY_TREE_H
