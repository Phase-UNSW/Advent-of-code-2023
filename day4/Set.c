// Implementation of the Set ADT
// COMP2521 - Assignment 1
// Paramvir Multani (z5479094)

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Set.h"
#include "SetStructs.h"

// Function Prototypes
struct node *SetInsertHelper(struct node *tree, int item, Set s);
struct node *SetFreeHelper(struct node *tree);
struct node *newNode(int item);
struct node *SetDeleteHelper(struct node *t, int item, Set s);
struct node *SetJoinHelper(struct node *t1, struct node *t2);
void SetPrintHelper(struct node *tree, FILE *out, bool *isFirst);
void SetUnionHelper(struct node *tree, Set new);
void SetIntersectionHelper(struct node *tree1, Set s, Set new);
void SetEqualsHelper(struct node *tree, Set s2, bool *SameSet);
void SetSubsetHelper(struct node *tree, Set s2, bool *isSubset);
struct node *avlRebalance(struct node *tree);
int balance(struct node *tree);
int height(struct node *tree);
struct node *rotateRight(struct node *tree);
struct node *rotateLeft(struct node *tree);
int SetAtIndexHelper(struct node *tree, int index);
void updateSize(struct node *tree);
int getSize(struct node *tree);
int SetIndexOfHelper(struct node *tree, int item, int *index);

////////////////////////////////////////////////////////////////////////
// Basic Set Operations

/**
 * Creates a new empty set
 */
Set SetNew(void) {
	Set s = malloc(sizeof(struct set));
	s->tree = NULL;
	s->size = 0;
	if (s == NULL) {
		fprintf(stderr, "Out of memory");
		exit(EXIT_FAILURE);
	}
	return s;
}

/**
 * Frees all memory allocated to the set
 */
void SetFree(Set s) {
	if (s == NULL) {
		return;
	}
	SetFreeHelper(s->tree);
	free(s);
	return;
}

// Uses Post Order traversal to free entire tree
struct node *SetFreeHelper(struct node *tree) {
	if (tree == NULL) {
		return NULL;
	}
	tree->left = SetFreeHelper(tree->left);
	tree->right = SetFreeHelper(tree->right);
	if (tree->right == NULL && tree->left == NULL) {
		free(tree);
		return NULL;
	}
	return tree;
}

/**
 * Inserts an item into the set
 */
void SetInsert(Set s, int item) {
	s->tree = SetInsertHelper(s->tree, item, s);
}

// Recursively traverses the binary seach tree by looking at branch values and then inserts the value
// then updates the set size and does an AVL rebalance to the tree
struct node *SetInsertHelper(struct node *tree, int item, Set s) {
	if (tree == NULL) {
		s->size++;
		return newNode(item);
	} else if (item < tree->item) {
		tree->left = SetInsertHelper(tree->left, item, s);
	} else if (item > tree->item) {
		tree->right = SetInsertHelper(tree->right, item, s);
	} else {
		return tree;
	}

	tree->size++;
	tree->height = height(tree);
	updateSize(tree);
	return avlRebalance(tree);
}
// returns the height of the tree by looking at the height of the nodes below
int height(struct node *tree) {
	if (tree == NULL) {
		return -1;
	}
	if (tree->left == NULL && tree->right == NULL) {
		return 0;
	} else if (tree->right == NULL) {
		return tree->left->height + 1;
	} else if (tree->left == NULL) {
		return tree->right->height + 1;
	} else if (tree->left->height > tree->right->height) {
		return tree->left->height + 1;
	} else {
		return tree->right->height + 1;
	}
}
// Rebalances the tree using the AVL rebalance
// method
struct node *avlRebalance(struct node *tree) {
	int bal = balance(tree);
	if (bal > 1) {
		if (balance(tree->left) < 0) {
			tree->left = rotateLeft(tree->left);
		}
		tree = rotateRight(tree);
	} else if (bal < -1) {
		if (balance(tree->right) > 0) {
			tree->right = rotateRight(tree->right);
		}
		tree = rotateLeft(tree);
	}
	return tree;
}

// Checks the heights of both branch nodes and returns the height
int balance(struct node *tree) {
	return height(tree->left) - height(tree->right);
}

// rotates the tree right and updates the size of both
// the tree and the new root node
struct node *rotateRight(struct node *tree) {
	if (tree == NULL || tree->left == NULL) {
		return tree;
	}
	struct node *newTree = tree->left;
	tree->left = newTree->right;
	newTree->right = tree;

	updateSize(tree);
	updateSize(newTree);
	return newTree;
}

// Rotates the tree left and updates the size of
// the tree and the new root node
struct node *rotateLeft(struct node *tree) {
	if (tree == NULL || tree->right == NULL) {
		return tree;
	}
	struct node *newTree = tree->right;
	tree->right = newTree->left;
	newTree->left = tree;
	updateSize(tree);
	updateSize(newTree);
	return newTree;
}

// Updates the size in a node by looking at the size of the
// branch nodes
void updateSize(struct node *tree) {
	if (tree->right != NULL && tree->left != NULL) {
		tree->size = tree->left->size + tree->right->size;
	} else if (tree->right == NULL && tree->left == NULL) {
		tree->size = 1;
	} else if (tree->right == NULL) {
		tree->size = 1 + tree->left->size;
	} else if (tree->left == NULL) {
		tree->size = 1 + tree->right->size;
	}
}

// Creates a new node with the given item
struct node *newNode(int item) {
	struct node *new = malloc(sizeof(struct node));
	new->item = item;
	new->left = NULL;
	new->right = NULL;
	new->height = 0;
	new->size = 1;
	return new;
}

/**
 * Deletes an item from the set
 */
void SetDelete(Set s, int item) {
	if (s->tree == NULL) {
		return;
	}
	s->tree = SetDeleteHelper(s->tree, item, s);
}
// Recursively traverses the tree comparing values to the item
// and then when it finds the item, deletes it, rejoins the tree
// if was not a leaf node, updates the size of the tree and
// then recursivley avl rebalances the tree
struct node *SetDeleteHelper(struct node *tree, int item, Set s) {
	if (tree == NULL) {
		return tree;
	} else if (item < tree->item) {
		tree->left = SetDeleteHelper(tree->left, item, s);
	} else if (item > tree->item) {
		tree->right = SetDeleteHelper(tree->right, item, s);
	} else {
		if (tree->left == NULL) {
			struct node *new = tree->right;
			free(tree);
			s->size--;
			return new;
		} else if (tree->right == NULL) {
			struct node *new = tree->left;
			free(tree);
			s->size--;
			return new;
		} else {
			struct node *new = SetJoinHelper(tree->left, tree->right);
			free(tree);
			s->size--;
			return new;
		}
	}
	updateSize(tree);
	return avlRebalance(tree);
}
// Rejoins the tree by adjusting pointers to reconnect the tree
// using the algorithim from lectures
struct node *SetJoinHelper(struct node *t1, struct node *t2) {
	struct node *curr = t2;
	struct node *parent = NULL;
	while (curr->left != NULL) {
		parent = curr;
		curr = curr->left;
	}
	if (parent != NULL) {
		parent->left = curr->right;
		curr->right = t2;
	}
	curr->left = t1;
	if (parent != NULL) {
		parent->height = height(parent);
		updateSize(parent);
	}

	curr->height = height(curr);
	updateSize(curr);
	return curr;
}

/**
 * Returns the number of elements in the set
 */
int SetSize(Set s) {
	return s->size;
}

// traverses the tree recursively and if the item is within
// the tree if true else returns false
bool SetcontainsHelper(struct node *tree, int item) {
	if (tree == NULL) {
		return false;
	}
	if (tree->item > item) {
		return SetcontainsHelper(tree->left, item);
	} else if (tree->item < item) {
		return SetcontainsHelper(tree->right, item);
	} else {
		return true;
	}
}

/**
 * Returns true if the set contains the given item, and false otherwise
 */
bool SetContains(Set s, int item) {
	return SetcontainsHelper(s->tree, item);
}

/**
 * Prints the elements in the set to the given file in ascending order
 * between curly braces, with items separated by a comma and space
 */
void SetPrint(Set s, FILE *out) {
	if (s == NULL) {
		fprintf(out, "{}");
		return;
	}
	fprintf(out, "{");
	// Used to do the equivalent of a do while loop with recursion
	bool isFirst = true;
	SetPrintHelper(s->tree, out, &isFirst);
	fprintf(out, "}");
}

// Uses in order traversal to print values
void SetPrintHelper(struct node *tree, FILE *out, bool *isFirst) {
	if (tree == NULL) {
		return;
	}
	SetPrintHelper(tree->left, out, isFirst);
	if (*isFirst == true) {
		fprintf(out, "%d", tree->item);
		*isFirst = false;
	} else {
		fprintf(out, ", %d", tree->item);
	}
	SetPrintHelper(tree->right, out, isFirst);
}
////////////////////////////////////////////////////////////////////////
// Common Set Operations

/**
 * Returns a new set representing the union of the two sets
 */
Set SetUnion(Set s1, Set s2) {
	Set new = SetNew();
	SetUnionHelper(s1->tree, new);
	SetUnionHelper(s2->tree, new);
	return new;
}

// adds elements to the new set by recursivley
// traveling to every node and inserting every value
// into the new tree
void SetUnionHelper(struct node *tree, Set new) {
	if (tree == NULL) {
		return;
	}
	SetInsert(new, tree->item);
	SetUnionHelper(tree->left, new);
	SetUnionHelper(tree->right, new);
}

/**
 * Returns a new set representing the intersection of the two sets
 */
Set SetIntersection(Set s1, Set s2) {
	Set new = SetNew();
	// Optimisation to reduce the number of nodes that must be traversed
	if (s1->size > s2->size) {
		SetIntersectionHelper(s1->tree, s2, new);
	} else {
		SetIntersectionHelper(s2->tree, s1, new);
	}

	return new;
}
// Uses SetContatains to determine if node in the tree is in the other set
// otherwise continues traversing the entire tree
void SetIntersectionHelper(struct node *tree, Set s, Set new) {
	if (tree == NULL) {
		return;
	}
	if (SetContains(s, tree->item)) {
		SetInsert(new, tree->item);
	}
	SetIntersectionHelper(tree->left, s, new);
	SetIntersectionHelper(tree->right, s, new);
}

/**
 * Returns true if the two sets are equal, and false otherwise
 */
bool SetEquals(Set s1, Set s2) {
	// Optimisation to return false if not same size
	if (s1->size != s2->size) {
		return false;
	}
	bool SameSize = true;

	SetEqualsHelper(s1->tree, s2, &SameSize);
	return SameSize;
}
// Traverses the set and determines if each elements is in 
// the set or not with SetContains
void SetEqualsHelper(struct node *tree, Set s2, bool *SameSize) {
	if (tree == NULL) {
		return;
	}
	// SameSize set to false if there is an item which is in one tree
	// but not the other
	if (!SetContains(s2, tree->item)) {
		*SameSize = false;
	}

	SetEqualsHelper(tree->left, s2, SameSize);
	SetEqualsHelper(tree->right, s2, SameSize);
}

/**
 * Returns true if set s1 is a subset of set s2, and false otherwise
 */
bool SetSubset(Set s1, Set s2) {
	// If the set checked to be a subset has
	// a greater size, it returns false
	if (s1->size > s2->size) {
		return false;
	}
	bool isSubset = true;
	SetSubsetHelper(s1->tree, s2, &isSubset);
	return isSubset;
}
// Traverses set and determines if every element in the set is in the
// other set
void SetSubsetHelper(struct node *tree, Set s2, bool *isSubset) {
	if (tree == NULL) {
		return;
	}
	//isSubset set to false if it contains an item not in the larger Set
	if (!SetContains(s2, tree->item)) {
		*isSubset = false;
	}
	SetSubsetHelper(tree->left, s2, isSubset);
	SetSubsetHelper(tree->right, s2, isSubset);
}

////////////////////////////////////////////////////////////////////////
// Index Operations

/**
 * Returns the element at the given index, or UNDEFINED if the given
 * index is outside the range [0, n - 1] where n is the size of the set
 */
int SetAtIndex(Set s, int index) {
	if (index > s->size - 1 || index < 0) {
		return UNDEFINED;
	}
	return SetAtIndexHelper(s->tree, index);
}

// Traversal to the left occurs when the size of the node is larger
// than the index and when it is smaller, it will traverse to the right
// subtracting the index by the leftsize and by 1
int SetAtIndexHelper(struct node *tree, int index) {
	int leftsize = getSize(tree->left);
	if (index == leftsize) {
		return tree->item;
	} else if (index < leftsize) {
		return SetAtIndexHelper(tree->left, index);
	} else {
		return SetAtIndexHelper(tree->right, index - leftsize - 1);
	}
}

// returns 0 if the node is null otherwise returns the
// size of the tree
int getSize(struct node *tree) {
	if (tree == NULL) {
		return 0;
	} else {
		return tree->size;
	}
}

/**
 * Returns the index of the given element in the set if it exists, and
 * -1 otherwise
 */
int SetIndexOf(Set s, int elem) {
	int index = 0;
	if (SetIndexOfHelper(s->tree, elem, &index)) {
		return index;
	}
	return -1;
}

// Function finds index by returning 0 if it reaches a leaf
// node without finding the item. When it goes right, it
// adds the size of the the node on the left branch to
// increase the index and then +1 to account for the previous
// parent node. Returns true if it finds the item.
int SetIndexOfHelper(struct node *tree, int item, int *index) {
	if (tree == NULL) {
		return 0;
	}

	if (tree->item == item) {
		*index = *index + getSize(tree->left);
		return 1;
	}
	if (item < tree->item) {
		return SetIndexOfHelper(tree->left, item, index);
	} else {
		*index = *index + 1 + getSize(tree->left);
		return SetIndexOfHelper(tree->right, item, index);
	}
}

////////////////////////////////////////////////////////////////////////
// Cursor Operations

/**
 * Creates a new cursor positioned at the *start* of the set
 * (see the spec for details)
 */
SetCursor SetCursorNew(Set s) {
	// TODO
	return NULL;
}

/**
 * Frees all memory allocated to the given cursor
 */
void SetCursorFree(SetCursor cur) {
	// TODO
}

/**
 * Returns the element at the cursor's position, or UNDEFINED if the
 * cursor is positioned at the *start* or *end* of the set
 */
int SetCursorGet(SetCursor cur) {
	// TODO
	return UNDEFINED;
}

/**
 * Moves the cursor to the next greatest element, or to the end of the
 * set if there is no next greatest element. Does not move the cursor if
 * it is already at the end. Returns false if the cursor is at the end
 * after this operation, and true otherwise.
 */
bool SetCursorNext(SetCursor cur) {
	// TODO
	return false;
}

/**
 * Moves the cursor to the next smallest element, or to the start of the
 * set if there is no next smallest element. Does not move the cursor if
 * it is already at the start. Returns false if the cursor is at the
 * start after this operation, and true otherwise.
 */
bool SetCursorPrev(SetCursor cur) {
	// TODO
	return false;
}

////////////////////////////////////////////////////////////////////////
