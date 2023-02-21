#ifndef LISTE_H
#define LISTE_H

#include "hex.h"
#include <string.h>
#include <stdio.h>
#include "utils.h"
#define CHAR_MAX 256
#define DICTIONARY_SIZE 1024

typedef struct List
{
    int c; // key
    int p; // prefix
    int value;  // index
    struct List * next;
} List;


List * initialize_dictionary();
void free_dict(List * dictionary);
void add_to_dictionary(List * dictionary, unsigned int prefix, unsigned int character, unsigned int value);
unsigned int get_character(List * dictionary, int value);
unsigned int get_prefix(List * dictionary, int value);
int search_p_and_c(List * dictionary, unsigned int prefix, unsigned int character);
List * add_element(List * dictionary, List * node_to_add);
List * find_last_node(List * dictionary);
unsigned int decode_string(List * dictionary, unsigned int code, FILE* output_stream);
void compression_liste(FILE *input_stream, FILE *output_stream);
void decompression_liste(FILE * input_stream, FILE *output_stream);

#endif