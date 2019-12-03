#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "invertedIndex.h"
#include "Tree.h"

// Functions for Part-1

/** Follow the instructions provided earlier in the specs to normalise 
    a given string. You need to modify a given string, do NOT create another copy.
*/
char * normaliseWord (char *str) {
    for (char *cur = str; *cur != '\0'; cur++) {
        if (*cur != ' ') {
            int i = 0;
            for (; *cur != '\0'; cur++) {
                str[i] = *cur;
                i++;
            }
            str[i] = '\0';
            break;
        }
    }

    for (int i = (strlen(str) - 1); (i > 0 && str[i] == ' '); i--) {
        if (str[i - 1] != ' ') {
            str[i] = '\0';
            break;
        }
    }

    for (char *cur = str; *cur != '\0'; cur++) {
        *cur = tolower(*cur);
        if (*(cur + 1) == '\0' && (*cur == '.' || *cur == ',' 
                || *cur == ';' || *cur == '?')) {
            *cur = '\0';
        }
    }
    return str;
}

/** The function needs to read a given file with collection of file names, 
    read each of these files, generate inverted index as discussed in 
    the specs and return the inverted index. Do not modify invertedIndex.h file.
*/
InvertedIndexBST generateInvertedIndex (char *collectionFilename) {
    InvertedIndexBST    new = NULL;
    char *file_name = malloc (100 * sizeof(char));
    char *word = malloc (100 * sizeof(char));
    FILE *fp;
    FILE *txt;
    fp = fopen(collectionFilename, "r");
    while (fscanf(fp, "%s", file_name) != EOF) {
        txt = fopen (file_name, "r");
        if (txt == NULL) continue;
        while (fscanf(txt, "%s", word) != EOF) {
            word = normaliseWord (word);
            new = insertIntoBST (new, word, file_name);
        }
        fclose (txt);
    }
    fclose (fp);
    count_tf (new);
    return new;
}


/** The function should output a give inverted index tree to a file named 
    invertedIndex.txt. One line per word, words should be alphabetically ordered, 
    using ascending order. Each list of filenames (for a single word) should be 
    alphabetically ordered, using ascending order.
*/
void printInvertedIndex (InvertedIndexBST tree) {
    if (tree == NULL) return;
    
    printInvertedIndex (tree->left);
    
    // open the file, and write text at the end
    FILE *fp = fopen("invertedIndex.txt" , "a");
	if( fp == NULL ) return;
    
    // printf detail
    fprintf(fp, "%s ",tree->word);
	FileList cur = tree->fileList;
	while(cur != NULL) {
        fprintf(fp, "%s ", cur->filename);
		cur = cur->next;
	}
    fprintf(fp, "\n");
	fclose(fp);
	
    printInvertedIndex (tree->right);
}

// Functions for Part-2


/** The function returns an ordered list where each node contains filename and the 
    corresponding tf-idf value for a given searchWord. You only need to return 
    documents (files) that contain a given searchWord. The list must be in descending 
    order of tf-idf values. If you have multple files with same tf-idf value, order 
    such files (documents) on their filenames using ascending order.
*/
TfIdfList calculateTfIdf (InvertedIndexBST tree, char *searchWord, int D) {
    TfIdfList new = NULL;
    if (searchWord == NULL) return new;
    new = help_calculateTfIdf (tree, new, searchWord, D);
    return new;
}


/** The function returns an ordered list where each node contains filename and summation 
    of tf-idf values of all matching searchWords. You only need to return documents (files) 
    that contain one or more searchWords. The list must be in descending order of summation 
    of tf-idf values (tfidf_sum). If you have multple files with same tfidf_sum value, order 
    such files (documents) on their filenames using ascending order.
*/
TfIdfList retrieve (InvertedIndexBST tree, char* searchWords[] , int D) {
    TfIdfList head = NULL;
    if (searchWords[0] == NULL) return head;
    for (int i = 0; searchWords[i] != NULL; i++) {
        // add the first item in the list
        if (head == NULL) {
            head = calculateTfIdf (tree, searchWords[i], D);
        } else {
            TfIdfList add = calculateTfIdf (tree, searchWords[i], D);
            TfIdfList curr = add;
            while (curr != NULL) {
                TfIdfList new_curr = head;
                TfIdfList clone = newTfIdfList (curr->filename, curr->tfidf_sum);
                // check the list if already have the same filename
                // sum up their tfidf
                while (new_curr != NULL) {
                    if (strcmp(new_curr->filename, clone->filename) == 0) {
                        new_curr->tfidf_sum += clone->tfidf_sum;
                        break;
                    }
                    new_curr = new_curr->next;
                }
                // new_curr = NULL means there is no same filename, then insert clone into the list
                if (new_curr == NULL) {
                    head = insertTfIdfList (clone, head);
                }
                curr = curr->next;
            }     
        }
    }
    // make the list into right order
    head = order_list (head);
    return head;
}


