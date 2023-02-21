#ifndef TREE_H
#define TREE_H


#include "hex.h"
#include <string.h>
#include <stdio.h>
#include "utils.h"
#define CHAR_MAX 256

struct Trie {
    bool isWord;
    char * value;
    struct Trie * children[CHAR_MAX];
};

struct Trie * dictionary_init();

void add_to_dictionary_trie(struct Trie * root, char * key, char * value);

bool search(struct Trie * root, char * key);

void compression_trie(FILE * input_stream, FILE * output_stream);
#endif