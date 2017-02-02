#include "tokenizer.h"
#include <stdlib.h>
#include <stdio.h>

/*
 * tokens can use ascii symbols ranging from 21 (!) to 126 (~); 20 (space) is reserved for token separation
 */

const int last_char = 126;
const int first_char = 21;
const int char_count = 126 - 21 + 1; //last - first + 1
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

#define opsCount 7
void tokenizer_initialize()
{
    dictionary = allocateNode();
    
    //infix operators
    add_to_dictionary("+", newToken(operator, newOperator(5, 2, sum, infix, left)));
    add_to_dictionary("-", newToken(operator, newOperator(5, 2, diff, infix, left)));
    add_to_dictionary("*", newToken(operator, newOperator(6, 2, mult, infix, left)));
    add_to_dictionary("/", newToken(operator, newOperator(6, 2, my_div, infix, left)));
    add_to_dictionary("^", newToken(operator, newOperator(7, 2, my_pow, infix, right)));
    
    //prefix functions
    add_to_dictionary("sqrt",
                      newToken(operator, newOperator(10, 1, my_sqrt, prefix, right)));
    add_to_dictionary("sqr",
                      newToken(operator, newOperator(10, 1, sqr, prefix, right)));
}

void deleteNode(Tree* node)
{
    if(node == NULL)
        return;
    else
    {
        //assumes node->children isn't null
        Tree** it = node -> children;
        while(*it != NULL)
            deleteNode(*it);
    }
}

void tokenizer_cleanup()
{
    deleteNode(dictionary);
}


char* skip_digits(char* text)
{
    char* current = text;
    while('0' <= *current && *current <= '9')
        current++;
    return current;
}

char* skip_dot(char* text)
{
    char* current = text;
    if(*current == '.')
        return current + 1;
    else
        return current;
}

Token** tokenizer_process(char* exp)
{
    double* val = malloc(sizeof(*val));
    
    /*char* current_word = malloc(initial_array_size * sizeof(*tokenized));
    int word_capacity = initial_array_size;
    int word_size = 0;*/
    
    Tree* current_word = dictionary;
    
    char* current_char = exp;
    Token** tokenized = malloc(initial_array_size * sizeof(*tokenized));
    int token_count = 0;
    int tokens_capacity = initial_array_size;
    
    while(*current_char != '\0')
    {
        if(*current_char == ' ')
            current_char++;
        else
            //special case for value tokens
            if(1 == sscanf(current_char, "%lf", val))
            {   
                current_char = skip_digits(skip_dot(skip_digits(current_char)));
                tokenized[token_count] = newToken(value, val);
                token_count++;
                if(token_count >= tokens_capacity)
                {
                    tokens_capacity = 2 * tokens_capacity;
                    tokenized = realloc(tokenized, tokens_capacity * sizeof(*tokenized));
                }
                val = malloc(sizeof(*val));
            }
            else
            {
                while(current_word != NULL && *current_char != '\0')
                {
                    current_word = current_word -> children[(int) *current_char - first_char];
                    current_char++;
                }
                if(current_word != NULL)
                {
                    Token* match = current_word->value;
                    tokenized[token_count] = match;
                    token_count++;
                    if(token_count >= tokens_capacity)
                    {
                        tokens_capacity = 2 * tokens_capacity;
                        tokenized = realloc(tokenized, tokens_capacity * sizeof(*tokenized));
                    }
                }
                else
                {
                    printf("Błąd na %ld znaku: nie znaleziono etykiety\n", current_char - exp);
                    
                    free(val);
                    free(current_word);
                    //TODO: free value tokens and tokenized array
                    return NULL;
                }
            }
    }
    
    //add terminating NULL to the end of tokenized
    tokenized = realloc(tokenized, (token_count + 1) * sizeof(*tokenized));
    tokenized[token_count] = NULL;
    
    free(val);
    free(current_word);
    
    return tokenized;
}