#ifndef LU_BINARY_TREE_H
#define LU_BINARY_TREE_H

#include "LuLinkedList.h"

// struct BinaryNode##type##_s* prev;
#define DECLARE_BINARYTREE_TYPE(type)           \
	typedef struct BinaryNode##type##_s {       \
		struct BinaryNode##type##_s* left;      \
		struct BinaryNode##type##_s* right;     \
		int key;                                \
		type data;                              \
	} BinaryNode##type, *BinaryNode##type##Ptr; \
                                                \
	typedef struct BinaryTree##type##_s {       \
		BinaryNode##type* root;                 \
		uint size;                              \
	} BinaryTree##type;                         \
                                                \
	DECLARE_LINKEDLIST_TYPE(BinaryNode##type##Ptr)

#define EmptyBinaryNode(type)                                \
	(BinaryNode##type) {                                     \
		.left = NULL, .right = NULL, .key = 0, .data = { 0 } \
	}

#define EmptyBinaryTree(type) \
	(BinaryTree##type) { .root = NULL, .size = 0 }

#define DECLARE_BINARYTREE_FUNCTIONS(type)                                                            \
                                                                                                      \
	DECLARE_LINKEDLIST_FUNCTIONS(BinaryNode##type##Ptr)                                               \
                                                                                                      \
	LinkedListBinaryNode##type##Ptr BinaryTree##type##AsList(BinaryTree##type* tree);                 \
                                                                                                      \
	BinaryNode##type* BinaryTree##type##RightRotate(BinaryNode##type* node);                          \
	BinaryNode##type* BinaryTree##type##LeftRotate(BinaryNode##type* node);                           \
                                                                                                      \
	int BinaryTree##type##NodeHeight(BinaryNode##type* node);                                         \
	int BinaryTree##type##NodeBalance(BinaryNode##type* node);                                        \
                                                                                                      \
	BinaryNode##type* BinaryTree##type##RecursivePush(BinaryNode##type* node, int key, type element); \
	BinaryNode##type* BinaryTree##type##RecursiveErase(BinaryNode##type* node, int key);              \
                                                                                                      \
	BinaryTree##type BinaryTree##type##Create();                                                      \
	void BinaryTree##type##Clear(BinaryTree##type* tree);                                             \
	BinaryNode##type* BinaryTree##type##Get(BinaryTree##type* tree, int key);                         \
	void BinaryTree##type##Set(BinaryTree##type* tree, int key, type element);                        \
	void BinaryTree##type##Push(BinaryTree##type* tree, int key, type element);                       \
	void BinaryTree##type##Erase(BinaryTree##type* tree, int key);                                    \
	bool BinaryTree##type##Contains(BinaryTree##type* tree, int key);

#define DEFINE_BINARYTREE_FUNCTIONS(type)                                                                              \
                                                                                                                       \
	DEFINE_LINKEDLIST_FUNCTIONS(BinaryNode##type##Ptr)                                                                 \
                                                                                                                       \
	LinkedListBinaryNode##type##Ptr BinaryTree##type##AsList(BinaryTree##type* tree) {                                 \
		if (!tree) {                                                                                                   \
			LOG_CONTAINER_ERROR(NullContainer);                                                                        \
			return EmptyLinkedList(BinaryNode##type##Ptr);                                                             \
		}                                                                                                              \
                                                                                                                       \
		LinkedListBinaryNode##type##Ptr list = EmptyLinkedList(BinaryNode##type##Ptr);                                 \
                                                                                                                       \
		LinkedListBinaryNode##type##Ptr stack = EmptyLinkedList(BinaryNode##type##Ptr);                                \
                                                                                                                       \
		BinaryNode##type* node = tree->root;                                                                           \
                                                                                                                       \
		while (node || stack.size) {                                                                                   \
			while (node) {                                                                                             \
				LinkedListBinaryNode##type##Ptr##Insert(&stack, 0, node);                                              \
                                                                                                                       \
				node = node->left;                                                                                     \
			}                                                                                                          \
                                                                                                                       \
			node = LinkedListBinaryNode##type##Ptr##Get(&stack, 0)->data;                                              \
                                                                                                                       \
			LinkedListBinaryNode##type##Ptr##Erase(&stack, 0);                                                         \
                                                                                                                       \
			LinkedListBinaryNode##type##Ptr##Push(&list, node);                                                        \
                                                                                                                       \
			node = node->right;                                                                                        \
		}                                                                                                              \
                                                                                                                       \
		return list;                                                                                                   \
	}                                                                                                                  \
                                                                                                                       \
	BinaryNode##type* BinaryTree##type##RightRotate(BinaryNode##type* node) {                                          \
		node->left->right = node;                                                                                      \
		node->left = node->right;                                                                                      \
		return node->left;                                                                                             \
	}                                                                                                                  \
                                                                                                                       \
	BinaryNode##type* BinaryTree##type##LeftRotate(BinaryNode##type* node) {                                           \
		node->right->left = node;                                                                                      \
		node->right = node->left;                                                                                      \
		return node->right;                                                                                            \
	}                                                                                                                  \
                                                                                                                       \
	int BinaryTree##type##NodeHeight(BinaryNode##type* node) {                                                         \
		if (!node) return -1;                                                                                          \
		if (!node->left && !node->right) return 0;                                                                     \
		int leftH = BinaryTree##type##NodeHeight(node->left);                                                          \
		int rightH = BinaryTree##type##NodeHeight(node->right);                                                        \
		return max(leftH, rightH) + 1;                                                                                 \
	}                                                                                                                  \
                                                                                                                       \
	int BinaryTree##type##NodeBalance(BinaryNode##type* node) {                                                        \
		if (!node) return 0;                                                                                           \
		int leftH = BinaryTree##type##NodeHeight(node->left);                                                          \
		int rightH = BinaryTree##type##NodeHeight(node->right);                                                        \
		return leftH - rightH;                                                                                         \
	}                                                                                                                  \
                                                                                                                       \
	BinaryNode##type* BinaryTree##type##RecursivePush(BinaryNode##type* node, int key, type element) {                 \
		if (node == NULL) {                                                                                            \
			BinaryNode##type* temp = calloc(1, sizeof(BinaryNode##type));                                              \
			if (!temp) {                                                                                               \
				LOG_CONTAINER_ERROR(MemAllocationFail);                                                                \
				return NULL;                                                                                           \
			}                                                                                                          \
                                                                                                                       \
			temp->key = key;                                                                                           \
			temp->data = element;                                                                                      \
			return temp;                                                                                               \
		}                                                                                                              \
                                                                                                                       \
		if (node->key == key) return node;                                                                             \
                                                                                                                       \
		if (key < node->key) node->left = BinaryTree##type##RecursivePush(node->left, key, element);                   \
                                                                                                                       \
		if (key > node->key) node->right = BinaryTree##type##RecursivePush(node->right, key, element);                 \
                                                                                                                       \
		int balance = BinaryTree##type##NodeBalance(node);                                                             \
		if (balance > 1 && key < node->left->key) return BinaryTree##type##RightRotate(node);                          \
                                                                                                                       \
		if (balance < -1 && key > node->right->key) return BinaryTree##type##LeftRotate(node);                         \
                                                                                                                       \
		if (balance > 1 && key > node->left->key) {                                                                    \
			node->left = BinaryTree##type##LeftRotate(node->left);                                                     \
			return BinaryTree##type##RightRotate(node);                                                                \
		}                                                                                                              \
                                                                                                                       \
		if (balance < -1 && key < node->right->key) {                                                                  \
			node->right = BinaryTree##type##RightRotate(node->right);                                                  \
			return BinaryTree##type##LeftRotate(node);                                                                 \
		}                                                                                                              \
                                                                                                                       \
		return node;                                                                                                   \
	}                                                                                                                  \
                                                                                                                       \
	BinaryNode##type* BinaryTree##type##RecursiveErase(BinaryNode##type* node, int key) {                              \
		if (!node) return NULL;                                                                                        \
                                                                                                                       \
		if (key < node->key) {                                                                                         \
			node->left = BinaryTree##type##RecursiveErase(node->left, key);                                            \
		} else if (key > node->key) {                                                                                  \
			node->right = BinaryTree##type##RecursiveErase(node->right, key);                                          \
		} else {                                                                                                       \
			if ((node->left == NULL) || (node->right == NULL)) {                                                       \
				BinaryNode##type* temp = node->left ? node->left : node->right;                                        \
                                                                                                                       \
				if (!temp) {                                                                                           \
					temp = node;                                                                                       \
					node = NULL;                                                                                       \
				}                                                                                                      \
                                                                                                                       \
				free(temp);                                                                                            \
			} else {                                                                                                   \
				BinaryNode##type* temp = node->right;                                                                  \
				while (temp->left) {                                                                                   \
					temp = temp->left;                                                                                 \
				}                                                                                                      \
				node->key = temp->key;                                                                                 \
				node->right = BinaryTree##type##RecursiveErase(node->right, temp->key);                                \
			}                                                                                                          \
		}                                                                                                              \
                                                                                                                       \
		if (!node) return NULL;                                                                                        \
                                                                                                                       \
		int balance = BinaryTree##type##NodeBalance(node);                                                             \
		if (balance > 1 && BinaryTree##type##NodeBalance(node->left) >= 0) return BinaryTree##type##RightRotate(node); \
                                                                                                                       \
		if (balance > 1 && BinaryTree##type##NodeBalance(node->left) < 0) {                                            \
			node->left = BinaryTree##type##LeftRotate(node->left);                                                     \
			return BinaryTree##type##RightRotate(node);                                                                \
		}                                                                                                              \
                                                                                                                       \
		if (balance < -1 && BinaryTree##type##NodeBalance(node->right) <= 0)                                           \
			return BinaryTree##type##LeftRotate(node);                                                                 \
                                                                                                                       \
		if (balance < -1 && BinaryTree##type##NodeBalance(node->right) > 0) {                                          \
			node->right = BinaryTree##type##RightRotate(node->right);                                                  \
			return BinaryTree##type##LeftRotate(node);                                                                 \
		}                                                                                                              \
                                                                                                                       \
		return node;                                                                                                   \
	}                                                                                                                  \
                                                                                                                       \
	BinaryTree##type BinaryTree##type##Create() { return EmptyBinaryTree(type); }                                      \
                                                                                                                       \
	void BinaryTree##type##Clear(BinaryTree##type* tree) {                                                             \
		if (!tree) {                                                                                                   \
			LOG_CONTAINER_ERROR(NullContainer);                                                                        \
			return;                                                                                                    \
		}                                                                                                              \
                                                                                                                       \
		LinkedListBinaryNode##type##Ptr stack = EmptyLinkedList(BinaryNode##type##Ptr);                                \
                                                                                                                       \
		BinaryNode##type* node = tree->root;                                                                           \
                                                                                                                       \
		while (node || stack.size) {                                                                                   \
			while (node) {                                                                                             \
				LinkedListBinaryNode##type##Ptr##Insert(&stack, 0, node);                                              \
                                                                                                                       \
				node = node->left;                                                                                     \
			}                                                                                                          \
                                                                                                                       \
			node = LinkedListBinaryNode##type##Ptr##Get(&stack, 0)->data;                                              \
                                                                                                                       \
			LinkedListBinaryNode##type##Ptr##Erase(&stack, 0);                                                         \
                                                                                                                       \
			BinaryNode##type* temp = node;                                                                             \
			node = node->right;                                                                                        \
                                                                                                                       \
			free(temp);                                                                                                \
		}                                                                                                              \
                                                                                                                       \
		*tree = EmptyBinaryTree(type);                                                                                 \
	}                                                                                                                  \
                                                                                                                       \
	BinaryNode##type* BinaryTree##type##Get(BinaryTree##type* tree, int key) {                                         \
		if (!tree) {                                                                                                   \
			LOG_CONTAINER_ERROR(NullContainer);                                                                        \
			return NULL;                                                                                               \
		}                                                                                                              \
                                                                                                                       \
		BinaryNode##type* node = tree->root;                                                                           \
                                                                                                                       \
		while (node) {                                                                                                 \
			if (node->key == key) {                                                                                    \
				return node;                                                                                           \
			}                                                                                                          \
                                                                                                                       \
			if (key < node->key) {                                                                                     \
				node = node->left;                                                                                     \
			} else {                                                                                                   \
				node = node->right;                                                                                    \
			}                                                                                                          \
		}                                                                                                              \
                                                                                                                       \
		LOG_CONTAINER_ERROR(OutOfBounds);                                                                              \
		return NULL;                                                                                                   \
	}                                                                                                                  \
                                                                                                                       \
	void BinaryTree##type##Set(BinaryTree##type* tree, int key, type element) {                                        \
		BinaryNode##type* node = BinaryTree##type##Get(tree, key);                                                     \
		if (!node) return;                                                                                             \
                                                                                                                       \
		node->data = element;                                                                                          \
	}                                                                                                                  \
                                                                                                                       \
	void BinaryTree##type##Push(BinaryTree##type* tree, int key, type element) {                                       \
		if (BinaryTree##type##Contains(tree, key)) {                                                                   \
			LOG_CONTAINER_ERROR(KeyAlreadyExists);                                                                     \
			return;                                                                                                    \
		}                                                                                                              \
                                                                                                                       \
		tree->root = BinaryTree##type##RecursivePush(tree->root, key, element);                                        \
		tree->size++;                                                                                                  \
	}                                                                                                                  \
                                                                                                                       \
	void BinaryTree##type##Erase(BinaryTree##type* tree, int key) {                                                    \
		if (!BinaryTree##type##Contains(tree, key)) {                                                                  \
			LOG_CONTAINER_ERROR(OutOfBounds);                                                                          \
			return;                                                                                                    \
		}                                                                                                              \
                                                                                                                       \
		tree->root = BinaryTree##type##RecursiveErase(tree->root, key);                                                \
		tree->size--;                                                                                                  \
	}                                                                                                                  \
                                                                                                                       \
	bool BinaryTree##type##Contains(BinaryTree##type* tree, int key) {                                                 \
		if (!tree) {                                                                                                   \
			LOG_CONTAINER_ERROR(NullContainer);                                                                        \
			return false;                                                                                              \
		}                                                                                                              \
                                                                                                                       \
		BinaryNode##type* node = tree->root;                                                                           \
                                                                                                                       \
		while (node) {                                                                                                 \
			if (node->key == key) return true;                                                                         \
                                                                                                                       \
			if (key < node->key) {                                                                                     \
				node = node->left;                                                                                     \
			} else {                                                                                                   \
				node = node->right;                                                                                    \
			}                                                                                                          \
		}                                                                                                              \
                                                                                                                       \
		return false;                                                                                                  \
	}

DECLARE_BINARYTREE_TYPE(byte)
DECLARE_BINARYTREE_FUNCTIONS(byte)

#endif	// LU_BINARY_TREE_H
