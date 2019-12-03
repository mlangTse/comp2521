// set.c ... simple, inefficient Set of Strings
// Written by John Shepherd, September 2015

#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

#include "set.h"

#define strEQ(s, t) (strcmp ((s), (t)) == 0)
#define strLT(s, t) (strcmp ((s), (t)) < 0)

typedef struct Node *Link;

typedef struct Node {
	char *val;
	Link next;
} Node;

typedef struct SetRep {
	int nelems;
	Link elems;
} SetRep;

// Function signatures

static Link newNode (char *);
static void dropNode (Link);
static int findNode (Link, char *, Link *, Link *);

// newSet()
// - create an initially empty Set
Set newSet (void)
{
	Set new = malloc (sizeof *new);
	if (new == NULL)
		err (EX_OSERR, "couldn't allocate Set");
	*new = (SetRep){.nelems = 0, .elems = NULL};
	return new;
}

// dropSet(Set)
// - clean up memory associated with Set
void dropSet (Set s)
{
	if (s == NULL)
		return;
	Link curr = s->elems;
	while (curr != NULL) {
		Link next = curr->next;
		dropNode (curr);
		curr = next;
	}

	free (s);
}

// insertInto(Set,Str)
// - ensure that Str is in Set
void insertInto (Set s, char *str)
{
	assert (s != NULL);
	Link curr, prev;
	int found = findNode (s->elems, str, &curr, &prev);
	if (found)
		return; // already in Set

	Link new = newNode (str);
	s->nelems++;
	if (prev == NULL) {
		// add at start of list of elems
		new->next = s->elems;
		s->elems = new;
	} else {
		// add into list of elems
		new->next = prev->next;
		prev->next = new;
	}
}

// dropFrom(Set,Str)
// - ensure that Str is not in Set
void dropFrom (Set s, char *str)
{
	assert (s != NULL);
	Link curr, prev;
	int found = findNode (s->elems, str, &curr, &prev);
	if (!found)
		return;
	s->nelems--;
	if (prev == NULL)
		s->elems = curr->next;
	else
		prev->next = curr->next;
	dropNode (curr);
}

// isElem(Set,Str)
// - check whether Str is contained in Set
int isElem (Set s, char *str)
{
	assert (s != NULL);
	Link curr, prev;
	return findNode (s->elems, str, &curr, &prev);
}

// nElems(Set)
// - return # elements in Set
int nElems (Set s)
{
	assert (s != NULL);
	return s->nelems;
}

// showSet(Set)
// - display Set (for debugging)
void showSet (Set s)
{
	Link curr;
	if (s->nelems == 0)
		printf ("Set is empty\n");
	else {
		printf ("Set has %d elements:\n", s->nelems);
		int id = 0;
		curr = s->elems;
		while (curr != NULL) {
			printf ("[%03d] %s\n", id, curr->val);
			id++;
			curr = curr->next;
		}
	}
}

// Helper functions

static Link newNode (char *str)
{
	Link new = malloc (sizeof (Node));
	assert (new != NULL);
	new->val = strdup (str);
	new->next = NULL;
	return new;
}

static void dropNode (Link curr)
{
	assert (curr != NULL);
	free (curr->val);
	free (curr);
}

// findNode(L,Str)
// - finds where Str could be added into L
// - if already in L, curr->val == Str
// - if not already in L, curr and prev indicate where to insert
// - return value indicates whether Str found or not
static int findNode (Link list, char *str, Link *cur, Link *pre)
{
	Link curr = list, prev = NULL;
	while (curr != NULL && strLT (curr->val, str)) {
		prev = curr;
		curr = curr->next;
	}
	*cur = curr;
	*pre = prev;
	return (curr != NULL && strEQ (str, curr->val));
}
