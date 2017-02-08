#include "tree.h"
#include "stack.h"
#include "tokenizer.h"
#include <stdlib.h>
#include <stdio.h>



//returns -1 if the stack doesn't have enough values to apply the function
int applyOpToSt(Operator* op, Stack* st)
{
    printf("   op: %p\n", op);
    printf("   Stack size: %d\n", sizeSt(st));
    printf("   Expecting %d arguments\n", op -> arity);
    if(sizeSt(st) < op -> arity)
        return -1;
    
    Tree** values = malloc( (op->arity + 1) * sizeof(*values) );
    
    int imax = op -> arity;
    for(int i=0; i < imax; i++)
        values[i] = popSt(st);
    values[imax] = NULL;
    
    pushSt(st, joinTrees(op->function, values));
    return 0;
}

typedef struct
{
    int l; //found values - to  the left
    int r; //required values - to the right
} Pair;
    
Pair* newPair(int l, int r)
{
    Pair* new = malloc(sizeof(*new));
    new -> l = l;
    new -> r = r;
    return new;
}

/*
 * 1 - no value before infix operator
 * 2 - applying operator to operator
 * 3 - not enough values to complete expression in brackets
 * 4 - mismatched ending bracket
 * 5 - mismatched opening bracket
 * 6 - not enough values applied to operators
 * 7 - expression doesn't represent one value (as in it might be a pair or no value at all)
 */
int checkSyntax(Token** expr)
{
    Pair* matches = newPair(0, 0);
    Stack* bracketsMatch = newStack();
    for(Token** currentToken = expr; *currentToken != NULL; currentToken++)
    {
        switch((*currentToken) -> type)
        {
            case operator: ;
                Operator* currentOp = (*currentToken) -> value;
                if(currentOp -> notation == infix)
                {
                    if(1 <= matches->l && matches->r == 0)
                    {
                        matches->r++;
                    }
                    else
                        if(matches->l <= 1)
                            return 1; //no value before operator
                        else
                            return 2; //applying operator to operator
                }
                else //notation == prefix
                {
                    if(matches->r == 0)
                    {
                        matches->l++;
                        matches->r += currentOp -> arity;
                    }
                    else
                        return 2; //applying operator to operator
                }
                break;
                
            case value:
                if(matches->r == 0)
                    matches->l++;
                else
                    matches->r--;
                break;
                
            case openBracket:
                pushSt(bracketsMatch, matches);
                matches = newPair(0, 0);
                break;
                
            case endBracket:
                if(matches->r > 0)
                    return 3;
                if(isEmptySt(bracketsMatch))
                    return 4;
                    
                Pair* outer = popSt(bracketsMatch);
                outer->r -= matches->l;
                if(outer->r < 0)
                {
                    outer->l -= outer->r;
                    outer->r = 0;
                }
                free(matches);
                matches = outer;
                break;
        }
    }
    int ret = 0;
    if(!isEmptySt(bracketsMatch))
        ret = 5;
    if(matches->r > 0)
        ret = 6;
    if(matches->l != 1)
    {
        printf("L: %d\n", matches->l);
        ret = 7;
    }
    
    free(matches);
    return ret;
}

//value type in inner nodes: double (*)(double*)
//value type in leaves: double* <-- must be freed later
Tree* makeOpTree(Token** expr)
{
    //cotains operators or brackets packed into token structure
    Stack* operators = newStack();
    if(operators == NULL)
    {
        printf("Couldn't allocate memory for operators stack\n");
        return NULL;
    }
    
    //values - stack of trees that represent a value to be calculated
    //  (the value can be obtained by traversing the tree)
    Stack* values = newStack();
    if(values == NULL)
    {
        printf("Couldn't allocate memory for values stack\n");
        deleteStack(operators);
        return NULL;
    }
    
    for(Token** currentToken = expr; *currentToken != NULL; currentToken++)
    {
        printf(" Parsing next token...\n");
        switch((*currentToken)->type)
        {
            //note: applying operators means applying them to the top elements of the values stack
            case operator: ;
                printf("  Found operator\n");
                Operator* currOp = (*currentToken) -> value;
                while(1)
                {
                    if(isEmptySt(operators))
                        break;
                    
                    Token* opTop = topSt(operators);
                    //this should only be operator or opening bracket
                    if(opTop -> type != operator)
                        break;
                    if( currOp -> assoc == left
                        && ((Operator*)(opTop -> value)) -> priority < currOp -> priority)
                    {
                        break;
                    }
                    if( currOp -> assoc == right
                        && ((Operator*)(opTop -> value)) -> priority <= currOp -> priority)
                    {
                        break;
                    }
                    
                    Operator* opRem = (Operator*) ((Token*)popSt(operators)) -> value;
                    //TODO: error handling
                    applyOpToSt(opRem, values);
                    //TODO: error handling
                }
                
                //push the new operator (still packed as a token) onto the stack
                printf("   Pushing %p onto operator stack\n", *currentToken);
                pushSt(operators, *currentToken);
                break;
            case value: ;
                printf("   Found value\n");
                Tree* newVal = newTree((*currentToken) -> value);
                free(*currentToken);
                if(newVal == NULL)
                {
                    //TODO: error handling
                    return NULL;
                }
                if(pushSt(values, newVal))
                {
                    //TODO: error handling
                    return NULL;
                }
                break;
            case openBracket:
                printf("  Found opening bracket\n");
                pushSt(operators, *currentToken);
                break;
            case endBracket: ;
                printf("  Found ending bracket\n");
                //TODO: handle errors: popSt, applyOp
                Token* opTop;
                if(!isEmptySt(operators))
                    opTop = (Token*) popSt(operators);
                else
                    opTop = NULL;
                while(!isEmptySt(operators) && opTop -> type != openBracket)
                {
                    //opTop should be of operator type
                    applyOpToSt((Operator*) opTop -> value, values);
                    opTop = (Token*) popSt(operators);
                }
                if(opTop -> type != openBracket)
                {
                    printf("Error: unmatched ending bracket\n");
                    //TODO deallocate stuff
                    return NULL;
                }
                //removes opening bracket
                //chyba nie (void) popSt(operators);
                break;
            //TODO: default: error
        }
        
    }
    printf("Parsed all tokens, reducing stacks\n");
    
    
    //apply operators to values until there are no operators left
    while(!isEmptySt(operators))
    {
        printf(" Operators stack size: %d\n", sizeSt(operators));
        
        Token* opToken = popSt(operators);
        printf(" token: %p\n", opToken);
        
        //error handling
        if(opToken -> type != operator)
        {
            if(opToken -> type == openBracket)
                printf("Error: unmatched bracket\n");
            if(opToken -> type == endBracket)
                printf("Implementation error (main.c %d): ending bracket remained\n", __LINE__);
            if(opToken -> type == value)
                printf("Implementation error: value on operator stack\n");
            //TODO: deallocate stuff
            
            return NULL;
        }
        Operator* opTop = opToken -> value;
        printf(" Applying operator to stack...\n");
        if(applyOpToSt(opTop, values) != 0)
        {
            printf("Error applying operators: not enough arguments\n");
            return NULL;
        }
        printf(" Applied\n");
    }
    Tree* ans = popSt(values);
    
    //TODO: deallocate stuff (stacks and perhaps something more)
    
    return ans;
}


double calcNode(Tree* node)
{
    if(node -> children == NULL)
        return *((double*)(node -> value));
    else
    {
        int count;
        for(count=0; node->children[count] != NULL; count++);
        double* children_val = malloc(count * sizeof(*children_val));
        for(int i=0; i < count; i++)
            children_val[i] = calcNode(node->children[i]);
        double ret = ((double(*)(double*))node->value)(children_val);
        free(children_val);
        return ret;
    }
}

void deleteOpTree(Tree* node)
{
    if(node != NULL)
    {
        //leaf node containing pointer to double allocted on the heap
        if(node -> children == NULL)
            free(node -> value);
        //inner node containing pointer to a function
        else
            for(Tree** current = node->children; *current != NULL; current++)
                deleteOpTree(*current);
       
        free(node);
    }
}

#define max_line_len 1000

char input[max_line_len + 1];

double eval(char* exp)
{
    printf("Tokenizing expression\n");
    Token** tokens = tokenizer_process(exp);
    
    if(tokens == NULL)
        return 0;
    
    switch(checkSyntax(tokens))
    {
        /*
         * 1 - no value before infix operator
         * 2 - applying operator to operator
         * 3 - not enough values to complete expression in brackets
         * 4 - mismatched ending bracket
         * 5 - mismatched opening bracket
         * 6 - not enough values applied to operators
         * 7 - expression doesn't represent one value (as in it might be a pair or no value at all)
         */
        case 0:
            printf("Syntax okay\n");
            break;
        case 1:
            printf("Syntax error: no value before infix operator\n");
            return 0;
        case 2:
            printf("Syntax error: applying operator to operator\n");
            return 0;
        case 3:
            printf("Syntax error: not enough values to complete expression in brackets\n");
            return 0;
        case 4:
            printf("Syntax error: mismatched ending bracket\n");
            return 0;
        case 5:
            printf("Syntax error: mismatched opening bracket\n");
            return 0;
        case 6:
            printf("Syntax error: not enough values applied to operators\n");
            return 0;
        case 7:
            printf("Syntax error: expression doesn't represent one value\n");
            return 0;
            break;
        
    }
    
    Token** tok;
    for(tok = tokens; *tok != NULL; tok++);
    printf("# of tokens: %ld\n", tok - tokens);
    
    //make operation tree and evaluate it
    printf("Parsing tokens\n");
    Tree* opTree = makeOpTree(tokens);
    if(opTree == NULL)
        return 0;
    
    printf("Calculating value\n");
    double val = calcNode(opTree);
    
    //free operation tree and contained values
    deleteOpTree(opTree);
    
    return val;
}

int main(int argc, char* args[])
{    
    printf("%d %s\n", argc, args[1]);
    //initialize tokenizer
    printf("Initializing tokenizer\n");
    tokenizer_initialize();
    
    //evaluate command line arguments
    if(argc > 1)
    {
        for(int i=1; i<argc; i++)
            printf("%lf\n", eval(args[i]));
    }
    else
        //interactive mode
        while(1)
        {
            //read a string and tokenize it
            printf("Input: ");
            fgets(input, max_line_len, stdin);
            
            if(input[0] == 'e' &&
               input[1] == 'x' &&
               input[2] == 'i' &&
               input[3] == 't' &&
              (input[4] == '\n' || input[4] == '\0'))
                break;
            
            
            printf("%lf\n", eval(input));
        }
    
    //free tokenizer
    tokenizer_cleanup();
    
    return 0;
}