#include "liste.h"

/**
 * @brief Find last node in list
 * 
 * @param dictionary Where to search
 * @return List* The last node in list
 */
List * find_last_node(List * dictionary)
{
    List * last_node = dictionary;
    while(last_node->next != NULL)
    {
        last_node = last_node->next;
    }
    return last_node;
}

/**
 * @brief Add element to a list
 * 
 * @param dictionary 
 * @param node_to_add 
 * @return List* Updated list
 */
List * add_element(List * dictionary, List * node_to_add)
{

    List * last_node = find_last_node(dictionary);

    //if not null, add after last node
    if (dictionary != NULL) 
        last_node->next = node_to_add;
    //else if it is null 
    //it will be the first element to be added
    else dictionary = node_to_add;

    last_node = node_to_add;
    node_to_add->next = NULL;

    return node_to_add;
}

/**
 * @brief Search for given prefix and character in the dictionary
 * 
 * @param dictionary The dictionary in which we want to search
 * @param prefix The prefix to search for
 * @param character The character to search for
 * @return int Returns the value of p+c in the dictionary or -1 if no match
 */
int search_p_and_c(List * dictionary, unsigned int prefix, unsigned int character)
{
    List * element = dictionary;

    while (element != NULL) {
        
        if (element->p == prefix && element->c == character) return element->value;

        element = element->next;
    }

    return -1;
}

/**
 * @brief Initialize the dictionary
 * 
 * @return List* 
 */
List * initialize_dictionary()
{
    List * dictionary = (List*)malloc(sizeof(List));
    for (int i = 0; i < CHAR_MAX; i++)
    {
        List * node_to_add = (List*)malloc(sizeof(List));
        node_to_add->c = i;
        node_to_add->p = -1;

        add_element(dictionary, node_to_add);
    }
    return dictionary;
}

/**
 * @brief Free dictionary
 * 
 * @param dictionary 
 */
void free_dict(List * dictionary)
{
    while(dictionary != NULL)
    {
        free(dictionary);
        dictionary = dictionary->next;
    }
}

/**
 * @brief Add element to dictionary
 * 
 * @param dictionary Where to add
 * @param prefix The prefix to add
 * @param character The character to add
 * @param value The value to add
 */
void add_to_dictionary(List * dictionary, unsigned int prefix, unsigned int character, unsigned int value)
{
    List * new_element = malloc(sizeof(List));
    new_element->value = value;
    new_element->c = character;
    new_element->p = prefix;

    add_element(dictionary, new_element);
}


/**
 * @brief Get the character
 * 
 * @param dictionary Where to search
 * @param value Searching the char based on its value 
 * @return unsigned int Returns the character, or 1 if it is not found
 */
unsigned int get_character(List * dictionary, int value)
{
    List * element = dictionary; 
    
    for(element; element != NULL; element = element->next)
    {
        if (element->value == value) return element->c;
    }
    
    return 1;
}

/**
 * @brief Get the prefix 
 * 
 * @param dictionary Where to search
 * @param value Searching for prefix based on its value
 * @return unsigned int Returns the prefix, or 1 if it is not found
 */
unsigned int get_prefix(List * dictionary, int value)
{
    List * element = dictionary; 
    
    for(element; element != NULL; element = element->next)
    {
        if (element->value == value) return element->p;
    }
    
    return 1;
}

unsigned int decode_string(List * dictionary, unsigned int code, FILE* output_stream)
{
    unsigned int character, temp;

    if (code > 255)
    {
        character = get_character(dictionary, code);
        temp = decode_string(dictionary, get_prefix(dictionary, code), output_stream);
    }
    else
    {
        character = code;
        temp = code;
    }

    fputc(character, output_stream);
    return temp;
}


void decompression_liste(FILE * input_stream, FILE * output_stream)
{
    int previous_code;
    int current_code;

    int code = 256;
    unsigned int firstChar;

    previous_code = hex2dec(rb_next_short_as_hex(input_stream));
    printf("FIRST CHAR IN FILE = %d\n", previous_code);
    
    putc(previous_code, output_stream);

    List * dictionary = initialize_dictionary();

    printf("Before while\n");

    while(rb_next_short_as_hex(input_stream) != NULL)
    {
        printf("Enter while\n");
        current_code = hex2dec(rb_next_short_as_hex(input_stream));
        printf("CURRENT CODE = %d\n", current_code);
        if (current_code >= code){
            printf("writing to file\n");
            putc(firstChar = decode_string(dictionary, previous_code, output_stream), output_stream);
        }
        else 
        {
            firstChar = decode_string(dictionary, current_code, output_stream);
            printf("FIRST CHAR = %d\n", firstChar);
        }
        
        if (code < DICTIONARY_SIZE){
            printf("CODE < DICTIONARY_SIZE\n");
            add_to_dictionary(dictionary, previous_code, firstChar, code++);
        } 

        previous_code = current_code;
        printf("PREVIOUS CODE %d\n", previous_code);
    }
}

void compression_liste(FILE *input_stream, FILE *output_stream)
{
    //P = first input character
    unsigned int p = getc(input_stream);
    unsigned int c;
    int code = 256;
    char * output_code;
    unsigned int ioutput_code;
    int i;

    //Initialize dictionary of 256 charcaters
    List * dictionary = initialize_dictionary();

    //while (C = next input character) not end of input stream
    while ((c = getc(input_stream)) != EOF) {
        //if P+C is in dictionary
        if ((i = search_p_and_c(dictionary, p, c)) != -1)
        {
            p = i; // p = p+c            
        }
        else
        {
            output_code = dec2hex(p, output_code, 8, false);
            ioutput_code = hex2dec(output_code);

            printf("Output code : %d\n", ioutput_code);

            //output the code for p
            wb_hex_as_short(output_stream, output_code);

            //add P+C to dictionary
            if (code < DICTIONARY_SIZE) add_to_dictionary(dictionary, p, c, code++);

            p = c;
        }
    }

    //got to end of file, output code for last character
    output_code = dec2hex(p, output_code, 8, false);
    wb_hex_as_short(output_stream, output_code);

    ioutput_code = hex2dec(output_code);

    printf("Output code : %d\n", ioutput_code);

    free_dict(dictionary);
}