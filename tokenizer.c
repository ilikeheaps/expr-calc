#include "tokenizer.h"

/*
 * tokens can use ascii symbols ranging from 21 (!) to 126 (~); 20 (space) is reserved for token separation
 */

const int last_char = 126;
const int first_char = 21;
const int char_count = last_char - first_char + 1;
//arrays of yet unknown size will be allocated with this size
const int initial_array_size = 10;

Tree* allocateNode()
{
    Tree* new = newTree(NULL);
    new->children = calloc(char_count, sizeof(*new->children));
    return new;
}

//nodes in dictionary must have their children array allocated
Tree* dictionary;

void initialize()
{
    dictionary = allocateNode();
}

Token* get_token_dict(char* label)
{
    char* current_char = label;
    Tree* current_word = dictionary;
    while(current_word != NULL && *current_char != '\0')
    {
        current_word = current_word -> children[(int) *current_char - first_char];
        current_char++;
    }
    if(current_word != NULL)
        return current_word -> value;
    else
        return NULL;
}

//will override the old definition
void add_to_dictionary(char* label, Token* token)
{
    char* current_char = label;
    //node representing a word created by going downward from root to the node
    Tree* current_word = dictionary; 
    while(*current_char != '\0' && 
          current_word -> children[*current_char - first_char] != NULL)
    {
        current_word = current_word -> children[(int) *current_char - first_char];
        current_char++;
    }
    while(*current_char != '\0')
    {
        Tree* nextNode = allocateNode();
        current_word -> children[*current_char - first_char] = nextNode;
        current_word = nextNode;
        current_char++;
    }
    current_word->value = token;
}


Token* process(char* exp)
{
    char* current_word = malloc(initial_array_size * sizeof(*tokenized));
    int word_capacity = initial_array_size;
    int word_size = 0;
    
    char* current_char = exp;
    Token** tokenized = malloc(initial_array_size * sizeof(*tokenized));
    //remeber about space for terminating null
    int tokens_capacity = initial_array_size;
    
    while(*current_char != '\0')
    {
        //TODO
    }
}