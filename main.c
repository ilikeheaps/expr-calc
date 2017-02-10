#include "tree.h"
#include "stack.h"
#include "tokenizer.h"
#include <stdlib.h>
#include <stdio.h>



//returns -1 if the stack doesn't have enough values to apply the function
int applyOpToSt(Operator* op, Stack* st)
{
    if(sizeSt(st) < op -> arity)
        return -1;
    
    Tree** values = malloc( (op->arity) * sizeof(*values) );
    
    for(int i=0; i < op -> arity; i++)
        values[i] = popSt(st);
    
    pushSt(st, joinTrees(op->function, values, op->arity));
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
                    if(matches->r > 0)
                        matches->r--;
                    else
                        matches->l++;
                    matches->r += currentOp -> arity;
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
    else if(matches->r > 0)
        ret = 6;
    else if(matches->l != 1)
    {
        printf("L: %d\n", matches->l);
        ret = 7;
    }
    
    while(!isEmptySt(bracketsMatch))
        free(popSt(bracketsMatch));
    
    deleteStack(bracketsMatch);
    
    free(matches);
    return ret;
}

//value type in inner nodes: double (*)(double*)
//value type in leaves: double* <-- must be freed later
Tree** makeOpTrees(Token** expr, int* result_count)
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
        switch((*currentToken)->type)
        {
            //note: applying operators means applying them to the top elements of the values stack
            case operator: ;
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
                pushSt(operators, *currentToken);
                break;
            case value: ;
                Tree* newVal = newTree((*currentToken) -> value, 0);
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
                pushSt(operators, *currentToken);
                break;
            case endBracket: ;
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
    
    
    //apply operators to values until there are no operators left
    while(!isEmptySt(operators))
    {
        
        Token* opToken = popSt(operators);
        
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
        if(applyOpToSt(opTop, values) != 0)
        {
            printf("Error applying operators: not enough arguments\n");
            return NULL;
        }
    }
    int values_count = sizeSt(values);
    (*result_count) = values_count;
    
    Tree** ans = malloc(values_count * sizeof(*ans));
    for(int i = 0; i < values_count; i++)
        ans[i] = popSt(values);
    
    //---deallocate stuff---
    //after popping all values it should be empty
    deleteStack(values);
    
    //if syntax check succedeed operators stack should be empty
    deleteStack(operators);
    
    return ans;
}

//TODO use tree.h
double calcNode(Tree* node)
{
    if(node -> children_count == 0)
        return *((double*)(node -> value));
    else
    {
        int count = node -> children_count;
        double* children_val = malloc(count * sizeof(*children_val));
        for(int i=0; i < count; i++)
            children_val[i] = calcNode(node->children[i]);
        double ret = ((double(*)(double*))node->value)(children_val);
        free(children_val);
        return ret;
    }
}

void deleteOpTreeValue(void* value, int children_count)
{
    if(children_count == 0)
        free(value);
}

void deleteOpTree(Tree* node)
{
    deleteTree(node, deleteOpTreeValue);
}

#define max_line_len 1000

char input[max_line_len + 1];

double* eval(char* exp, int* result_count)
{
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
    
    //make operation tree and evaluate it
    
    int values_count = 0;
    Tree** opTrees = makeOpTrees(tokens, &values_count);
    if(values_count == 0)
        return 0;
    *result_count = values_count;
    
    double* ans = malloc(values_count * sizeof(*ans));
    for(int i = 0; i < values_count; i++)
        ans[i] = calcNode(opTrees[i]);
        
   //free tokens array
    /*
    for(Token** curr = tokens; *curr != NULL; curr++)
    {
        if((*curr) -> type == value)
            free((*curr) -> value);
    }
    */
    free(tokens);
    
    //free operation trees and contained values
    //TODO: consider moving freeing values to makeOpTrees
    for(int i = 0; i < values_count; i++)
        deleteOpTree(opTrees[i]);
    free(opTrees);
    
    return ans;
}

int main(int argc, char* args[])
{    
    //initialize tokenizer
    tokenizer_initialize();
    
    //evaluate command line arguments
    if(argc > 1)
    {
        for(int i=1; i<argc; i++)
        {
            printf("Evaluating %s\n", args[i]);
            int values_count = 0;
            double* values = eval(args[i], &values_count);
            for(int i = 0; i < values_count; i++)
                printf("%lf\n", values[i]);
            
            free(values);
        }
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
            
            int values_count = 0;
            double* values = eval(input, &values_count);
            for(int i = 0; i < values_count; i++)
                printf("%lf\n", values[i]);
            
            free(values);
        }
    
    //free tokenizer
    tokenizer_cleanup();
    
    return 0;
}