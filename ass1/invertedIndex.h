// Ass1 ... COMP2521 

#ifndef _INVERTEDINDEX_GUARD
#define _INVERTEDINDEX_GUARD


struct FileListNode {
    char *filename;
	double tf;    // relative tf
	struct FileListNode *next;
};
typedef struct FileListNode *FileList;

struct InvertedIndexNode {

	char  *word;  // key

	struct FileListNode  *fileList;

	struct InvertedIndexNode  *left;
	struct InvertedIndexNode  *right;

};
typedef struct InvertedIndexNode *InvertedIndexBST;

struct TfIdfNode {
    char *filename;
	double tfidf_sum;    // tfidf sum value
	struct TfIdfNode *next;
};
typedef struct TfIdfNode *TfIdfList;



// Functions for Part-1

/** Follow the instructions provided earlier in the specs to normalise 
    a given string. You need to modify a given string, do NOT create another copy.
*/
char * normaliseWord(char *str);

/** The function needs to read a given file with collection of file names, 
    read each of these files, generate inverted index as discussed in 
    the specs and return the inverted index. Do not modify invertedIndex.h file.
*/
InvertedIndexBST generateInvertedIndex(char *collectionFilename);


/** The function should output a give inverted index tree to a file named 
    invertedIndex.txt. One line per word, words should be alphabetically ordered, 
    using ascending order. Each list of filenames (for a single word) should be 
    alphabetically ordered, using ascending order.
*/
void printInvertedIndex(InvertedIndexBST tree); 

// Functions for Part-2


/** The function returns an ordered list where each node contains filename and the 
    corresponding tf-idf value for a given searchWord. You only need to return 
    documents (files) that contain a given searchWord. The list must be in descending 
    order of tf-idf values. If you have multple files with same tf-idf value, order 
    such files (documents) on their filenames using ascending order.
*/
TfIdfList calculateTfIdf(InvertedIndexBST tree, char *searchWord , int D);


/** The function returns an ordered list where each node contains filename and summation 
    of tf-idf values of all matching searchWords. You only need to return documents (files) 
    that contain one or more searchWords. The list must be in descending order of summation 
    of tf-idf values (tfidf_sum). If you have multple files with same tfidf_sum value, order 
    such files (documents) on their filenames using ascending order.
*/
TfIdfList retrieve(InvertedIndexBST tree, char* searchWords[] , int D);


#endif



