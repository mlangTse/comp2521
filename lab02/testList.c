// testList.c - testing DLList data type
// Written by John Shepherd, March 2013

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "DLList.h"

int main (void)
{
	
	printf("\n========= Test 1 =========\n");
	printf("    ... Testing newDLList\n");
	DLList myList = newDLList ();
	//assert(DLListCurrent (myList) == myList->curr->value)
	
    printf("       --> Checking that the list is NULL\n");
    assert(DLListLength (myList) ==  0);
	assert (validDLList (myList));
	
    printf(">> Passed newDLList tests!\n");
	
	printf("\n========= Test 2 =========\n");
	printf("    ... Testing DLListBefore\n");
    printf("       --> Intsert \"This is the first line\" before the current item\n");
	DLListBefore(myList, "This is the first line");
	
    printf("       --> Checking that number of elements in a list\n");
    assert(DLListLength (myList) == 1);
	assert (validDLList (myList));
	
    printf("       --> Intsert \"first line, now\" before the current item\n");
	DLListBefore(myList, "first line, now");
	DLListMove (myList, 1);
	
    printf("       --> Checking that number of elements in a list\n");
    assert(DLListLength (myList) == 2);
	assert (validDLList (myList));
	
    printf("       --> Checking the current line is \"This is the first line\"\n");
    assert(strcmp(DLListCurrent (myList), "This is the first line") == 0);
    
    printf("       --> Delete the list\n");
    DLListDelete (myList);
    DLListDelete (myList);
    assert(DLListLength (myList) ==  0);
	assert (validDLList (myList));
    
    printf(">> Passed DLListBefore tests!\n");
	
	printf("\n========= Test 3 =========\n");
	printf("    ... Testing DLListAfter\n");
    printf("       --> Intsert \"This is the first line\" after the current item\n");
	DLListAfter(myList, "This is the first line");
	
    printf("       --> Checking that number of elements in a list\n");
    assert(DLListLength (myList) == 1);
	assert (validDLList (myList));
	
    printf("       --> Intsert \"second line\" after the current item\n");
	DLListAfter(myList, "second line");
	DLListMove (myList, -1);
	
    printf("       --> Checking that number of elements in a list\n");
    assert(DLListLength (myList) == 2);
	assert (validDLList (myList));
	
    printf("       --> Checking the current line is \"This is the first line\"\n");
    assert(strcmp(DLListCurrent (myList), "This is the first line") == 0);
    
    printf("       --> Delete the list\n");
    DLListDelete (myList);
    DLListDelete (myList);
    assert(DLListLength (myList) ==  0);
	assert (validDLList (myList));
    
    printf(">> Passed DLListAfter tests!\n");
    
	printf("\n========= Test 4 =========\n");
	printf("    ... Testing DLListDelete\n");
    printf("    ... Testing DLListBefore\n");
    printf("       --> Intsert \"This is the first line\" before the current item\n");
	DLListBefore(myList, "This is the first line");
	
    printf("       --> Checking that number of elements in a list\n");
    assert(DLListLength (myList) == 1);
	assert (validDLList (myList));
	
    printf("       --> Intsert \"first line, now\" before the current item\n");
	DLListBefore(myList, "first line, now");
	
    printf("       --> Checking that number of elements in a list\n");
    assert(DLListLength (myList) == 2);
	assert (validDLList (myList));
	
    printf("       --> Checking the current line is \"first line, now\"\n");
    assert(strcmp(DLListCurrent (myList), "first line, now") == 0);
    
    printf("       --> Delete the current item\n");
    DLListDelete (myList);
    
    printf("       --> Checking the current line is \"This is the first line\"\n");
    assert(DLListLength (myList) == 1);
	assert (validDLList (myList));
    assert(strcmp(DLListCurrent (myList), "This is the first line") == 0);
    
    printf("       --> Intsert \"second line\" after the current item\n");
	DLListAfter(myList, "second line");
	
    printf("       --> Checking that number of elements in a list\n");
    assert(DLListLength (myList) == 2);
	assert (validDLList (myList));
	
    printf("       --> Checking the current line is \"second line\"\n");
    assert(strcmp(DLListCurrent (myList), "second line") == 0);
    
    printf("       --> Delete the current item\n");
    DLListDelete (myList);
    
    printf("       --> Checking the current line is \"This is the first line\"\n");
    assert(DLListLength (myList) == 1);
	assert (validDLList (myList));
    assert(strcmp(DLListCurrent (myList), "This is the first line") == 0);
    
    printf("       --> Delete the list\n");
    DLListDelete (myList);
    assert(DLListLength (myList) ==  0);
	assert (validDLList (myList));
    
    printf(">> Passed DLListDelete tests!\n");
	
	freeDLList (myList);
	return EXIT_SUCCESS;
}
