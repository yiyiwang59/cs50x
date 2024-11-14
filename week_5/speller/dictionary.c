// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <strings.h>
#include <string.h>
#include "dictionary.h"
#include <stdlib.h>
#include <stdio.h>

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26*26;

// Hash table
node *table[N];

int word_count = 0;
// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    int index = hash(word);
    node *pointer = table[index];
    while (pointer != NULL) {
        if (strcasecmp(pointer->word, word) == 0) {
            return true;
            break;
        }
        pointer = pointer->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    int index = ((tolower(word[0]) - 'a') * 26);
    if (word[1] != '\0') {
        index += (tolower(word[1]) - 'a');
    }
    return index;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    //Open dict file
    FILE *source = fopen(dictionary, "r");
    if (source == NULL)
    {
        return false;
    }
    //Iterate through each word
    char word[LENGTH + 1];
    while (fscanf(source, "%s", word) != EOF) {
        //create space for node
        node *n = malloc(sizeof(node));
        if (n == NULL) {
            return false;
        }
        strcpy(n->word, word);

        //determine index of where to insert into hash table
        int index = hash(word);
        //Insert node into hash table
        n->next = table[index];
        table[index] = n;
        word_count++;
    }
    //close the file
    fclose(source);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return word_count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    for (int i=0; i < N; i++) {

        node *cursor = table[i];
        while (cursor != NULL) {
            node *tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }
    }
    return true;
}
