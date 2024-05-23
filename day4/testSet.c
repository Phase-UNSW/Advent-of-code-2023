// COMP2521 - Assignment 1

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Set.h"
#include "SetStructs.h"

void testSetInsert(void);
void testSetDelete(void);
void testSetSize(void);
void testSetContains(void);
void testSetPrint(void);
void checkFileContents(FILE *file, char *str);

void testSetUnion(void);
void testSetIntersection(void);
void testSetEquals(void);
void testSetSubset(void);

void testBalance1(void);
void testBalance2(void);
bool isHeightBalanced(struct node *t);
bool doIsHeightBalanced(struct node *t, int *height);

void testSetAtIndex(void);
void testSetIndexOf(void);

void testSetCursor1(void);
void testSetCursor2(void);

int main(int argc, char *argv[]) {
	testSetInsert();
	testSetDelete();
	testSetSize();
	testSetContains();
	testSetPrint();
	
	testSetUnion();
	testSetIntersection();
	testSetEquals();
	testSetSubset();

	testBalance1();
	testBalance2();

	testSetAtIndex();
	testSetIndexOf();
/*
	testSetCursor1();
	testSetCursor2();
	*/
}

void testSetInsert(void) {
	// NOTE: SetInsert can't be tested on its own unless we directly
	//       access the internal representation of the ADT

	Set s = SetNew();
	SetInsert(s, 5);
	SetInsert(s, 2);
	SetInsert(s, 8);
	SetInsert(s, 4);
	SetInsert(s, 5);
	SetInsert(s, -5);
	assert(SetSize(s) == 5);
	SetFree(s);
}

void testSetDelete(void) {
	// NOTE: SetDelete can't be tested without SetInsert

	Set s = SetNew();
	SetInsert(s, 5);
	SetInsert(s, 2);
	SetInsert(s, 8);
	SetInsert(s, 4);

	SetDelete(s, 2);
	assert(SetSize(s) == 3);
	SetDelete(s, 8);
	assert(SetSize(s) == 2);
	SetDelete(s, 3);
	assert(SetSize(s) == 2);
	SetDelete(s, 5);
	assert(SetSize(s) == 1);
	SetInsert(s,5);
	assert(SetSize(s) == 2);

	SetFree(s);
}

void testSetSize(void) {
	Set s = SetNew();
	assert(SetSize(s) == 0);
	SetInsert(s, 5);
	SetInsert(s, 2);
	SetInsert(s, 8);
	SetInsert(s, 4);
	assert(SetSize(s) == 4);
	SetInsert(s, 7);
	SetInsert(s, 2);
	assert(SetSize(s) == 5);
	SetDelete(s, 5);
	assert(SetSize(s) == 4);

	SetFree(s);
}

void testSetContains(void) {
	Set s = SetNew();
	SetInsert(s, 5);
	SetInsert(s, 2);
	SetInsert(s, 8);
	SetInsert(s, 4);
	assert(SetContains(s, 2));
	assert(SetContains(s, 4));
	assert(SetContains(s, 5));
	assert(SetContains(s, 8));
	assert(!SetContains(s, 3));

	SetFree(s);
}

void testSetPrint(void) {
	Set s = SetNew();
	SetInsert(s, 5);
	SetInsert(s, 2);
	SetInsert(s, 8);
	SetInsert(s, 4);

	FILE *out = tmpfile();
	SetPrint(s, out);
	checkFileContents(out, "{2, 4, 5, 8}");
	fclose(out);

	SetFree(s);

	s = SetNew();
	out = tmpfile();
	SetInsert(s, 5);
	SetPrint(s,out);
	checkFileContents(out, "{5}");
	fclose(out);
	out = tmpfile();
	SetFree(s);

	s = SetNew();
	
	SetPrint(s, out);
	checkFileContents(out, "{}");
	fclose(out);
	SetFree(s);

	s = SetNew();
	out = tmpfile();
	SetInsert(s, -10);
	SetInsert(s, -3);
	SetInsert(s, -20);
	
	SetPrint(s, out);
	checkFileContents(out, "{-20, -10, -3}");
	fclose(out);
	SetFree(s);

}

/**
 * This function checks if the contents of a file matches the expected
 * contents. If the contents do not match, the program will exit with an
 * assertion error.
 */
void checkFileContents(FILE *file, char *expectedContents) {
	fflush(file);
	fseek(file, 0, SEEK_SET);
	char *line = NULL;
	size_t n = 0;
	getline(&line, &n, file);
	assert(strcmp(line, expectedContents) == 0);
	free(line);
}

////////////////////////////////////////////////////////////////////////

void testSetUnion(void) {
	Set a = SetNew();
	SetInsert(a, 5);
	SetInsert(a, 2);
	SetInsert(a, 8);
	SetInsert(a, 4);

	Set b = SetNew();
	SetInsert(b, 3);
	SetInsert(b, 8);
	SetInsert(b, 7);
	SetInsert(b, 5);

	Set c = SetUnion(a, b);
	assert(SetContains(c, 2));
	assert(SetContains(c, 3));
	assert(SetContains(c, 4));
	assert(SetContains(c, 5));
	assert(SetContains(c, 7));
	assert(SetContains(c, 8));
	assert(SetSize(c) == 6);

	SetFree(a);
	SetFree(b);
	SetFree(c);

	a = SetNew();
	b = SetNew();
	c = SetUnion(a,b);
	assert(SetSize(c) == 0);

	SetFree(a);
	SetFree(b);
	SetFree(c);

	a = SetNew();
	b = SetNew();

	SetInsert(a, 5);
	SetInsert(a, 2);
	SetInsert(a, 8);
	SetInsert(a, 4);

	SetInsert(b, 3);
	SetInsert(b, 8);
	SetInsert(b, 7);

	SetDelete(a, 2);

	c = SetUnion(a,b);
	assert(SetContains(c, 2) == false);
	assert(SetSize(c) == 5);
	SetFree(a);
	SetFree(b);
	SetFree(c);


}

void testSetIntersection(void) {
	Set a = SetNew();
	SetInsert(a, 5);
	SetInsert(a, 2);
	SetInsert(a, 8);
	SetInsert(a, 4);

	Set b = SetNew();
	SetInsert(b, 3);
	SetInsert(b, 8);
	SetInsert(b, 7);
	SetInsert(b, 5);

	Set c = SetIntersection(a, b);
	assert(SetContains(c, 5));
	assert(SetContains(c, 8));
	assert(!SetContains(c, 2));
	assert(!SetContains(c, 3));
	assert(!SetContains(c, 4));
	assert(!SetContains(c, 7));
	assert(SetSize(c) == 2);

	SetFree(a);
	SetFree(b);
	SetFree(c);

	a = SetNew();
	b = SetNew();
	
	c = SetIntersection(a, b);
	assert(c->size == 0);

	SetFree(a);
	SetFree(b);
	SetFree(c);
}

void testSetEquals(void) {
	Set a = SetNew();
	SetInsert(a, 4);
	SetInsert(a, 2);
	SetInsert(a, 7);
	SetInsert(a, 1);

	Set b = SetNew();
	SetInsert(b, 2);
	SetInsert(b, 4);
	SetInsert(b, 1);
	SetInsert(b, 7);

	Set c = SetNew();
	SetInsert(c, 4);
	SetInsert(c, 2);
	SetInsert(c, 7);
	SetInsert(c, 3);

	Set d = SetNew();
	assert(SetEquals(a, b));
	assert(!SetEquals(a, c));
	assert(SetEquals(d,d));
	assert(!SetEquals(d,a));

	SetFree(a);
	SetFree(b);
	SetFree(c);
	SetFree(d);
}

void testSetSubset(void) {
	Set a = SetNew();
	SetInsert(a, 5);
	SetInsert(a, 3);
	SetInsert(a, 6);

	Set b = SetNew();
	SetInsert(b, 3);
	SetInsert(b, 9);
	SetInsert(b, 5);
	SetInsert(b, 6);

	Set c = SetNew();
	SetInsert(c, 4);
	SetInsert(c, 6);
	SetInsert(c, 3);
	SetInsert(c, 7);

	Set d = SetNew();

	assert(SetSubset(a, b));
	assert(!SetSubset(a, c));
	assert(SetSubset(d,c));
	assert(SetSubset(a,a));

	SetFree(a);
	SetFree(b);
	SetFree(c);
	SetFree(d);
}

////////////////////////////////////////////////////////////////////////

void testBalance1(void) {
	Set s = SetNew();
	// Left Left case
	SetInsert(s, 8);
	SetInsert(s, 5);
	SetInsert(s, 2);
	// The tree should have been rebalanced after inserting 2
	// NOTE: Normally, a user should not have access to the concrete
	//       representation of an ADT, but since we have #included
	//       SetStructs.h, we have access for testing purposes.
	assert(isHeightBalanced(s->tree));
	// Right Right case
	SetInsert(s, 10);
	SetInsert(s, 12);
	assert(isHeightBalanced(s->tree));
	// Left Right case
	SetInsert(s, -1);
	SetInsert(s, 1);
	assert(isHeightBalanced(s->tree));
	// Right Left Case
	SetInsert(s, 20);
	SetInsert(s, 15);
	assert(isHeightBalanced(s->tree));

	SetFree(s);

}

void testBalance2(void) {
	Set s = SetNew();

	SetInsert(s, 4);
    SetInsert(s, 2);
    SetInsert(s, 6);
    SetInsert(s, 1);
    SetInsert(s, 3);
    SetDelete(s, 1); // Deleting a leaf node
    assert(isHeightBalanced(s->tree));

    // Test case 2: Deleting a node with one child
    SetInsert(s, 5);
    SetDelete(s, 6); // Deleting a node with one child
    assert(isHeightBalanced(s->tree));

    // Test case 3: Deleting a node with two children
    SetInsert(s, 7);
    SetDelete(s, 2); // Deleting a node with two children
    assert(isHeightBalanced(s->tree));

    // Test case 4: Deleting the root node
    SetDelete(s, 4); // Deleting the root node
    assert(isHeightBalanced(s->tree));

    // Test case 5: Deleting non-existent node
    SetDelete(s, 10); // Trying to delete a node not in the set
    assert(isHeightBalanced(s->tree));

    // Freeing the set
	SetFree(s);
}

/**
 * Checks if the given tree is height-balanced
 */
bool isHeightBalanced(struct node *t) {
	int height = -1;
	return doIsHeightBalanced(t, &height);
}

bool doIsHeightBalanced(struct node *t, int *height) {
	if (t == NULL) {
		*height = -1;
		return true;
	}

	int lHeight = -1;
	int rHeight = -1;
	if (doIsHeightBalanced(t->left, &lHeight)
			&& doIsHeightBalanced(t->right, &rHeight)
			&& abs(lHeight - rHeight) <= 1) {
		*height = (lHeight > rHeight ? lHeight : rHeight) + 1;
		return true;
	} else {
		return false;
	}
}

////////////////////////////////////////////////////////////////////////

void testSetAtIndex(void) {
	Set s = SetNew();

	SetInsert(s, 5);
	SetInsert(s, 2);
	SetInsert(s, 8);
	SetInsert(s, 4);

	assert(SetAtIndex(s, 0) == 2);
	assert(SetAtIndex(s, 1) == 4);
	assert(SetAtIndex(s, 2) == 5);
	assert(SetAtIndex(s, 3) == 8);

	SetInsert(s, 3);

	assert(SetAtIndex(s, 0) == 2);
	assert(SetAtIndex(s, 1) == 3);
	assert(SetAtIndex(s, 2) == 4);
	assert(SetAtIndex(s, 3) == 5);
	assert(SetAtIndex(s, 4) == 8);

	SetFree(s);
}

void testSetIndexOf(void) {
	Set s = SetNew();

	SetInsert(s, 5);
	SetInsert(s, 2);
	SetInsert(s, 8);
	SetInsert(s, 4);
	
	assert(SetIndexOf(s, 2) == 0);
	assert(SetIndexOf(s, 4) == 1);
	assert(SetIndexOf(s, 5) == 2);
	assert(SetIndexOf(s, 8) == 3);

	SetInsert(s, 3);

	assert(SetIndexOf(s, 2) == 0);
	assert(SetIndexOf(s, 3) == 1);
	assert(SetIndexOf(s, 4) == 2);
	assert(SetIndexOf(s, 5) == 3);
	assert(SetIndexOf(s, 8) == 4);

	SetFree(s);
}

////////////////////////////////////////////////////////////////////////

void testSetCursor1(void) {
	Set s = SetNew();

	SetInsert(s, 5);
	SetInsert(s, 2);
	SetInsert(s, 8);
	SetInsert(s, 4);

	SetCursor cur = SetCursorNew(s);
	// start  2  4  5  8  end
	//   ^
	assert(SetCursorGet(cur) == UNDEFINED);

	assert(SetCursorNext(cur));
	// start  2  4  5  8  end
	//        ^
	assert(SetCursorGet(cur) == 2);

	assert(SetCursorNext(cur));
	// start  2  4  5  8  end
	//           ^
	assert(SetCursorGet(cur) == 4);

	assert(SetCursorNext(cur));
	// start  2  4  5  8  end
	//              ^
	assert(SetCursorGet(cur) == 5);

	assert(SetCursorNext(cur));
	// start  2  4  5  8  end
	//                 ^
	assert(SetCursorGet(cur) == 8);

	assert(!SetCursorNext(cur));
	// start  2  4  5  8  end
	//                     ^
	assert(SetCursorGet(cur) == UNDEFINED);

	assert(SetCursorPrev(cur));
	// start  2  4  5  8  end
	//                 ^
	assert(SetCursorGet(cur) == 8);

	assert(SetCursorPrev(cur));
	// start  2  4  5  8  end
	//              ^
	assert(SetCursorGet(cur) == 5);

	assert(SetCursorPrev(cur));
	// start  2  4  5  8  end
	//           ^
	assert(SetCursorGet(cur) == 4);

	assert(SetCursorPrev(cur));
	// start  2  4  5  8  end
	//        ^
	assert(SetCursorGet(cur) == 2);

	assert(!SetCursorPrev(cur));
	// start  2  4  5  8  end
	//   ^
	assert(SetCursorGet(cur) == UNDEFINED);

	SetCursorFree(cur);
	SetFree(s);
}

void testSetCursor2(void) {
	Set s = SetNew();

	SetInsert(s, 5);
	SetInsert(s, 2);
	SetInsert(s, 8);
	SetInsert(s, 4);

	SetCursor cur = SetCursorNew(s);
	// start  2  4  5  8  end
	//   ^
	assert(SetCursorGet(cur) == UNDEFINED);

	assert(SetCursorNext(cur));
	// start  2  4  5  8  end
	//        ^
	assert(SetCursorGet(cur) == 2);

	SetInsert(s, 3);
	// start  2  3  4  5  8  end
	//        ^

	assert(SetCursorNext(cur));
	// start  2  3  4  5  8  end
	//           ^
	assert(SetCursorGet(cur) == 3);

	SetDelete(s, 4);
	// start  2  3  5  8  end
	//           ^

	assert(SetCursorNext(cur));
	// start  2  3  5  8  end
	//              ^
	assert(SetCursorGet(cur) == 5);

	SetCursorFree(cur);
	SetFree(s);
}

