// insert a new value into a Tree
InvertedIndexBST newBST (char *word, char *filename);

// create a new FileListNode
FileList newFileList (char *filename);

// create a new TfIdfNode
TfIdfList newTfIdfList (char *filename, double tfidf);

// insert a new value into a BSTree
InvertedIndexBST insertIntoBST (InvertedIndexBST tree, char *word, char *filename);

// insert a new value into FileList
FileList insertFilename (FileList head, char *filename);

// calculate the tf value
void count_tf (InvertedIndexBST tree);

// a help function for count_tf
void help_count_tf (InvertedIndexBST tree);

// loop through the Tree
TfIdfList help_calculateTfIdf (InvertedIndexBST tree, TfIdfList head, char *searchWord, int D);

// calculating the TfIdf value
TfIdfList calculating_TfIdf (InvertedIndexBST tree, TfIdfList head, int D);

// insert a new TfIdfNode
TfIdfList insertTfIdfList (TfIdfList new, TfIdfList old);

// help ordering the list
TfIdfList order_list (TfIdfList head);

// Helper: rotate tree left around root (from lab04/Tree.c)
InvertedIndexBST rotateL (InvertedIndexBST n2);

// Helper: rotate tree right around root (from lab04/Tree.c)
InvertedIndexBST rotateR (InvertedIndexBST n1);

// Helper: calculate the depth (from lab04/Tree.c)
int depth (InvertedIndexBST t);