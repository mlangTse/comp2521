#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "invertedIndex.h"
#include "Tree.h"

InvertedIndexBST newBST (char *word, char *filename) {
    InvertedIndexBST new = malloc (sizeof (*new));
    assert(new != NULL);
    new->word = malloc(100 * (sizeof (char)));
    strcpy(new->word, word);
    new->fileList = newFileList(filename);
    new->left = new->right = NULL;
    return new;
}

FileList newFileList (char *filename) {
    FileList new = malloc (sizeof (*new));
    assert(new != NULL);
    new->filename = malloc(100 * (sizeof (char)));
    strcpy(new->filename, filename);
    new->tf = 1;
    new->next = NULL;
    return new;
}

TfIdfList newTfIdfList (char *filename, double tfidf) {
    TfIdfList new = malloc (sizeof (struct TfIdfNode));
    assert(new != NULL);
    new->filename = malloc(100 * (sizeof (char)));
    strcpy(new->filename, filename);
    new->tfidf_sum = tfidf;
    new->next = NULL;
    return new;
}

InvertedIndexBST insertIntoBST (InvertedIndexBST tree, char *word, char *filename) {
    if (tree == NULL) {
        return newBST(word, filename);
    } else if (strcmp(word, tree->word) < 0) {
        tree->left = insertIntoBST(tree->left, word, filename);
    } else if (strcmp(word, tree->word) > 0) {
        tree->right = insertIntoBST(tree->right, word, filename);
    } else if (strcmp(word, tree->word) == 0) {
        tree->fileList = insertFilename(tree->fileList, filename);
    }

    int dL = depth (tree->left);
    int dR = depth (tree->right);
    if ((dL - dR) > 1) tree = rotateR(tree);
    if ((dR - dL) > 1) tree = rotateL(tree);
    return tree;
}

FileList insertFilename (FileList head, char *filename) {
    if (head == NULL) {
        return newFileList(filename);
    } else {
        FileList curr = head;
        FileList prev = NULL;
        FileList new = newFileList(filename);
        while (curr != NULL) {
            if (strcmp(filename, curr->filename) == 0) { 
                curr->tf++;
                return head;
            }
            if (strcmp(filename, curr->filename) < 0) {
                if (prev == NULL) {
                    new->next = curr;
                    head = new;
                } else {
                    prev->next = new;
                    new->next = curr;
                }
                return head;
            } 
            prev = curr;
            curr = curr->next;
        }
        if (prev != NULL && strcmp(filename, prev->filename) > 0) {
            prev->next = new;
        }
        return head;
    }
}

void count_tf (InvertedIndexBST tree) {
    if (tree == NULL) return;
    count_tf(tree->left);
    help_count_tf(tree);
    count_tf(tree->right);
}

void help_count_tf (InvertedIndexBST tree) {
    char *word = malloc(100 * sizeof(char));
    double n_word;
    FILE *txt;
    FileList curr = tree->fileList;
    while (curr != NULL) {
        txt = fopen(curr->filename, "r");
        n_word = 0;
        while (fscanf(txt, "%s", word) != EOF) {
            n_word++;
        }
        curr->tf = curr->tf / n_word;
        curr = curr->next;   
        fclose(txt);
    }
}

TfIdfList help_calculateTfIdf (InvertedIndexBST tree, TfIdfList head, char *searchWord, int D) {
    if (tree == NULL) return head;
    // find out where is searchWord in the tree
    if (strcmp(searchWord, tree->word) == 0) {
        return calculating_TfIdf (tree, head, D);
    } 
    if (help_calculateTfIdf(tree->left, head, searchWord, D) != NULL) {
        return help_calculateTfIdf(tree->left, head, searchWord, D);
    }
    return help_calculateTfIdf(tree->right, head, searchWord, D);
}

TfIdfList calculating_TfIdf (InvertedIndexBST tree, TfIdfList head, int D) {
    double idf = 0;
    double total_file = 0;
    double tfidf = 0;
    FileList curr = tree->fileList;
    
    // counting the total number of file contain that word
    while (curr != NULL) {
        total_file++;
        curr = curr->next;
    }
    curr = tree->fileList;
    
    // calculate the idf
    idf = log10(D / total_file);
    
    while (curr != NULL) {
        // calculate tfidf
        tfidf = curr->tf * idf;
        // make a new TfIdf Node
        TfIdfList new = newTfIdfList (curr->filename, tfidf);
        // if cur_tfidf is empty, let it equal to the new node
        if (head == NULL) {
            head = new;
        } else {
            // call the function to insert the other node
            head = insertTfIdfList (new, head);
        }
        curr = curr->next;
    }
    return head;
}

TfIdfList insertTfIdfList (TfIdfList new, TfIdfList head) {
    TfIdfList curr = head;
    TfIdfList prev = NULL;
    while (curr != NULL) {
        // if the new tfidf is larger, put new in front of the curr 
        if (new->tfidf_sum > curr->tfidf_sum) {
            if (prev == NULL) {
                new->next = curr;
                head = new;
            } else {
                prev->next = new;
                new->next = curr;
            }
            return head;
        // if two tfidf are the same, and the filename are different
        } else if (new->tfidf_sum == curr->tfidf_sum && 
                    strcmp(new->filename, curr->filename) != 0) {
            // if new->filename is smaller than cur_tfidf->filename
            // put new in front of the curr 
            if (strcmp(new->filename, curr->filename) < 0) {
                if (prev == NULL) {
                    new->next = curr;
                    head = new;
                } else {
                    prev->next = new;
                    new->next = curr;
                }
                return head;
            }
        }
        prev = curr;
        curr = curr->next;
    }
    // other case
    if (prev != NULL && new->tfidf_sum < prev->tfidf_sum) {
        prev->next = new;
    } else if (prev != NULL && strcmp(new->filename, prev->filename) > 0) {
        prev->next = new;
    }
    return head;
}

TfIdfList order_list (TfIdfList head) {
    TfIdfList curr = head;
    TfIdfList new = newTfIdfList (curr->filename, curr->tfidf_sum);
    curr = curr->next;
    while (curr != NULL) {
        TfIdfList clone = newTfIdfList (curr->filename, curr->tfidf_sum);
        new = insertTfIdfList (clone, new);
        curr = curr->next;
    }
    return new;
}

// Helper: rotate tree left around root (from lab04/Tree.c)
InvertedIndexBST rotateL (InvertedIndexBST n2) {
	if (n2 == NULL) return NULL;
	InvertedIndexBST n1 = n2->right;
	if (n1 == NULL) return n2;
	n2->right = n1->left;
	n1->left = n2;
	return n1; 
}

// Helper: rotate tree right around root (from lab04/Tree.c)
InvertedIndexBST rotateR (InvertedIndexBST n1) {
    if (n1 == NULL) return NULL;
    InvertedIndexBST n2 = n1->left;
    if (n2 == NULL) return n1;
    n1->left = n2->right;
    n2->right = n1;
    return n2;
}

// Helper: calculate the depth (from lab04/Tree.c)
int depth (InvertedIndexBST t) {
    if (t == NULL) return 0;
    int ldepth = depth (t->left);
	int rdepth = depth (t->right);
	return 1 + ((ldepth > rdepth) ? ldepth : rdepth);
}