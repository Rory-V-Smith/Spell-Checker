// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 2000;

// Hash table
node *table[N];

// word counter
int word_count = 0;

// Returns true if word is in dictionary, else false
// must be case sensitive // will only be passed words that contain (uppercase or lowercase) alphabetical characters and possibly apostrophes.
bool check(const char *word)
{
    // get hash value
    int index = hash(word);
    // access linked list at index
    node *cursor = table[index];
    // traverse through linked list
    while (cursor != NULL)
    {
        // check if word matches
        if (strcasecmp(cursor->word, word) == 0)
            return true;
        // move cursor to next node
        cursor = cursor->next;
    }
    return false;
}

// Hashes word to an index
// djb2 by Dan Bernstein.
unsigned int hash(const char *word)
{
    unsigned long hash = 5381;
    int c;
    while ((c = toupper(*word++)))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash % N;
}

// (loads words from dictionary into hash table);
// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // open dictionary file
    FILE *f = fopen(dictionary, "r");
    if (f == NULL)
    {
        fprintf(stderr, "could not open %s\n", dictionary);
        return false;
    }

    // read strings from file
    char buffer[LENGTH + 1];
    while(fscanf(f, "%s", buffer) != EOF)
    {
        // create new node
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            printf("Not enough memory\n");
            return false;
        }
        // copy word into node
        strcpy(n->word, buffer);

        // index word by calling hash function
        int index = hash(n->word);

        // set new pointer
        n->next = table[index];
        // set head to new pointrer
        table[index] = n;
        // increment word count
        word_count++;
    }
    //close file
    fclose(f);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return word_count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // iterate through buckets
    for (int i = 0; i < N; i++)
    {
        // set cursor to each bucket location
        node *cursor = table[i];
        // traverse through until the end of the linked list
        while (cursor != NULL)
        {
            // assign tmp node equal to cursor
            node *tmp = cursor;
            // move cursro to next node
            cursor = cursor->next;
            // free tmp
            free(tmp);
        }
    }
    return true;
}