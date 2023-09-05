#include <LuBinaryTree.h>

enum BT_ErrorCodes {
	BT_IsNullptr = 0,
	BT_KeyNotFound,
	BT_KeyAlreadyExists,
	BT_EmptyDataSize,
	BT_NoDataPassed,
	BT_AllocFail,
	BT_ErrorCount
};

static const string BT_ErrorMessages[BT_ErrorCount] = {
	"Binary Tree Passed Is Nullptr",
	"Key Not Found",
	"Key Already Exists",
	"Size Of Element Is 0 (Zero)",
	"Trying To Push/Insert/Remove Nullptr/Empty Memory",
	"Binary Tree Allocation (malloc/calloc) Failed",
};

#define PRINT_BT_ERROR(bt_err) fprintf(stderr, "Binary Tree Error %d: %s\n", bt_err, BT_ErrorMessages[bt_err])

BinaryNode* RecursivePush(BinaryNode* node, int key, void* data_, uint data_len);

BinaryNode* RecursiveDeletion(BinaryNode* root, int key, bool clear_recursive);

void DeleteAllNodes(BinaryNode* root, bool clear_recursive) {
	if (!root) return;
	DeleteAllNodes(root->left, clear_recursive);
	DeleteAllNodes(root->right, clear_recursive);

	// root->prev = NULL;
	root->left = NULL;
	root->right = NULL;
	root->key = 0;
	if (clear_recursive) {
		free(*((void**)root->data));
	}

	free(root->data);
	root->data = NULL;
	free(root);
}

BinaryNode* rightRotate(BinaryNode* node) {
	// BinaryNode* prev = node->prev;
	BinaryNode* left = node->left;
	BinaryNode* right = left->right;

	left->right = node;
	// node->prev = left;
	// left->prev = prev;

	node->left = right;
	// if (right) right->prev = node->left;

	return left;
}

BinaryNode* leftRotate(BinaryNode* node) {
	// BinaryNode* prev = node->prev;
	BinaryNode* right = node->right;
	BinaryNode* left = right->left;

	right->left = node;
	// node->prev = right;
	// right->prev = prev;

	node->right = left;
	// if (left) left->prev = node->right;

	return right;
}

int NodeHeight(BinaryNode* node) {
	if (!node) return -1;
	if (!node->left && !node->right) return 0;
	int leftHeight = NodeHeight(node->left);
	int rightHeight = NodeHeight(node->right);
	return max(leftHeight, rightHeight) + 1;
}

int NodeBalance(BinaryNode* node) {
	if (!node) return 0;
	int leftHeight = NodeHeight(node->left);
	int rightHeight = NodeHeight(node->right);
	return leftHeight - rightHeight;
}

BinaryTree bt_Create(uint dataSize, bool isDataPointers) {
	BinaryTree temp;
	temp.root = NULL;
	temp.dataSize = dataSize;
	temp.containsPointers = isDataPointers;
	temp.elementCount = 0;
	return temp;
}

void bt_Clear(BinaryTree* tree, bool clear_recursive) {
	if (!tree) {
		PRINT_BT_ERROR(BT_IsNullptr);
		return;
	}

	if (!tree->dataSize) {
		PRINT_BT_ERROR(BT_EmptyDataSize);
		return;
	}

	if (!tree->root || tree->elementCount == 0) return;

	/*while (tree->root) {
	  tree->root = RecursiveDeletion(tree->root, tree->root->key,
									 tree->containsPointers && clear_recursive);
	}*/
	DeleteAllNodes(tree->root, clear_recursive && tree->containsPointers);
	tree->root = NULL;
	tree->elementCount = 0;
}

BinaryNode* bt_Get(BinaryTree* tree, int key) {
	if (!bt_Contains(tree, key)) {
		PRINT_BT_ERROR(BT_KeyNotFound);
		return NULL;
	}

	BinaryNode* node = tree->root;

	while (node) {
		if (key < node->key) {
			node = node->left;
		} else if (key > node->key) {
			node = node->right;
		} else {
			break;
		}
	}

	return node;
}

void bt_Set(BinaryTree* tree, int key, void* data_) {
	if (!data_) {
		PRINT_BT_ERROR(BT_NoDataPassed);
		return;
	}

	if (!tree->dataSize) {
		PRINT_BT_ERROR(BT_EmptyDataSize);
		return;
	}

	BinaryNode* temp = bt_Get(tree, key);
	if (!temp) return;

	memcpy(temp->data, data_, tree->dataSize);
}

void bt_Push(BinaryTree* tree, int key, void* data_) {
	if (bt_Contains(tree, key)) {
		PRINT_BT_ERROR(BT_KeyAlreadyExists);
		return;
	}

	if (!data_) {
		PRINT_BT_ERROR(BT_NoDataPassed);
		return;
	}

	if (!tree->dataSize) {
		PRINT_BT_ERROR(BT_EmptyDataSize);
		return;
	}

	tree->root = RecursivePush(tree->root, key, data_, tree->dataSize);
	tree->elementCount++;
}

void bt_Remove(BinaryTree* tree, int key, bool clear_recursive) {
	if (!bt_Contains(tree, key)) {
		PRINT_BT_ERROR(BT_KeyNotFound);
		return;
	}
	tree->root = RecursiveDeletion(tree->root, key, tree->containsPointers && clear_recursive);
	tree->elementCount--;
}

bool bt_Contains(BinaryTree* tree, int key) {
	if (!tree) {
		PRINT_BT_ERROR(BT_IsNullptr);
		return false;
	}

	if (!tree->dataSize) {
		PRINT_BT_ERROR(BT_EmptyDataSize);
		return false;
	}

	BinaryNode* node = tree->root;
	while (node) {
		if (key < node->key) {
			node = node->left;
		} else if (key > node->key) {
			node = node->right;
		} else {
			return true;
		}
	}

	return false;
}

BinaryNode* RecursivePush(BinaryNode* node, int key, void* data_, uint data_len) {
	if (node == NULL) {
		BinaryNode* temp = calloc(1, sizeof(BinaryNode));
		if (!temp) {
			PRINT_BT_ERROR(BT_AllocFail);
			return NULL;
		}
		// temp->prev = parent;
		temp->key = key;
		temp->data = malloc(data_len);
		if (!temp->data) {
			PRINT_BT_ERROR(BT_AllocFail);
			free(temp);
			return NULL;
		}
		memcpy(temp->data, data_, data_len);
		return temp;
	}
	if (key < node->key)
		node->left = RecursivePush(node->left, key, data_, data_len);
	else if (key > node->key)
		node->right = RecursivePush(node->right, key, data_, data_len);
	else
		return node;

	int balance = NodeBalance(node);
	if (balance > 1 && key < node->left->key) return rightRotate(node);

	if (balance < -1 && key > node->right->key) return leftRotate(node);

	if (balance > 1 && key > node->left->key) {
		node->left = leftRotate(node->left);
		return rightRotate(node);
	}

	if (balance < -1 && key < node->right->key) {
		node->right = rightRotate(node->right);
		return leftRotate(node);
	}

	return node;
}

BinaryNode* RecursiveDeletion(BinaryNode* root, int key, bool clear_recursive) {
	if (root == NULL) return root;

	if (key < root->key) {
		root->left = RecursiveDeletion(root->left, key, clear_recursive);
	} else if (key > root->key) {
		root->right = RecursiveDeletion(root->right, key, clear_recursive);
	} else {
		if ((root->left == NULL) || (root->right == NULL)) {
			BinaryNode* temp = root->left ? root->left : root->right;
			/* if (root->prev) {
			   if (root->prev->left == root) {
				 root->prev->left = temp;
			   } else {
				 root->prev->right = temp;
			   }
			 }*/

			if (temp == NULL) {
				temp = root;
				root = NULL;
			} else {
				void* tempData = root->data;
				*root = *temp;
				temp->data = tempData;
			}

			if (clear_recursive) {
				free(*((void**)temp->data));
			}
			free(temp->data);
			temp->data = NULL;
			// temp->prev = NULL;
			temp->left = NULL;
			temp->right = NULL;
			temp->key = 0;
			free(temp);
		} else {
			BinaryNode* temp = root->right;
			while (temp->left) {
				temp = temp->left;
			}
			root->key = temp->key;
			void* tempData = root->data;
			root->data = temp->data;
			temp->data = tempData;
			root->right = RecursiveDeletion(root->right, temp->key, clear_recursive);
		}
	}

	if (root == NULL) return root;

	int balance = NodeBalance(root);
	if (balance > 1 && NodeBalance(root->left) >= 0) return rightRotate(root);

	if (balance > 1 && NodeBalance(root->left) < 0) {
		root->left = leftRotate(root->left);
		return rightRotate(root);
	}

	if (balance < -1 && NodeBalance(root->right) <= 0) return leftRotate(root);

	if (balance < -1 && NodeBalance(root->right) > 0) {
		root->right = rightRotate(root->right);
		return leftRotate(root);
	}

	return root;
}
