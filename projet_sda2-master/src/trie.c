#include "tree.h"


/**
 * @brief Initialize the dictionary
 * 
 * @return struct Trie* - the dictionary
 */
struct Trie * dictionary_init()
{
    struct Trie * element = (struct Trie*)malloc(sizeof(struct Trie));

    element->isWord = false;

    for (int i = 0; i < CHAR_MAX; i++)
    {
        element->children[i] = NULL;
    }

    return element;
}

/**
 * @brief Insert key/value pair into dictionary
 * 
 * @param root the trie
 * @param key the key to insert
 * @param value the value to insert
 */
void add_to_dictionary_trie(struct Trie * root, char * key, char * value)
{
    struct Trie * cursor = root;

    for (int i = 0; i < strlen(key); i++)
    {
        int index = key[i];
        if (cursor->children[index] == NULL)
        {
            cursor->children[index] = dictionary_init();
            cursor->children[index]->value = value;
        }

        cursor = cursor->children[index];
    }

    cursor->isWord = true;
}

/**
 * @brief Search for given key in trie
 * 
 * @param root the trie
 * @param key the key to find
 * @return true if key is found
 * @return false if not
 */
bool search(struct Trie * root, char * key)
{
    if (root == NULL) return false;

    struct Trie * cursor = root;

    printf("Given key: %s\n", key);


    while(*key)
    {
        cursor = cursor->children[*key];

        if (cursor == NULL) return false;

        key++;
    }
    
    return cursor->isWord;
}

/**
 * @brief Compress file
 * 
 * @param input_stream Input file
 * @param output_stream Output file
 */
void compression_trie(FILE * input_stream, FILE * output_stream)
{
    printf("begin\n");
    int code = 256;
    char string[CHAR_MAX];
    char nextChar;

    struct Trie * dict = dictionary_init();

    //first char
    fread(string, 1, 1, input_stream);
    printf("string %s\n", string);

    char * value;
    value = dec2hex(*string, NULL, sizeof(unsigned), true);
    add_to_dictionary_trie(dict, string, value);

    //read till EOF
    while ((fread(&nextChar, sizeof(nextChar), 1, input_stream)) > 0)
    {
        //check if P+C is in dictionary
        if(search(dict, concat(string, char2str(nextChar))) == 1)
        {
            //p = p+c
            strncpy(string, concat(string, char2str(nextChar)), strlen(string)+1);
        }
        //if if is not
        else 
        {
            //write to file
            wb_hex_as_short(output_stream, dec2hex(*string, NULL, sizeof(string), true));

            //add it to dictionary
            add_to_dictionary_trie(dict, string, dec2hex(code++, NULL, 16, true));
            
            //p = c
            strncpy(string, char2str(nextChar), 2);
        }
    }

    //write to file
    wb_hex_as_short(output_stream, dec2hex(*string, NULL, sizeof(string), true));
}