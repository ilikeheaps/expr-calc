#include "parser.h"
#include "tree.h"
#include "stack.h"
#include "main.h"
#include "tokenizer.h"
#include <stdlib.h>
#include <stdio.h>



//returns -1 if the stack doesn't have enough values to apply the function
int applyOpToSt(Operator* op, Stack* st)
{
    if(sizeSt(st) < op -> arity)
        return -1;
    
    Tree** values = malloc( (op->arity + 1) * sizeof(*values) );
    
    int imax = op -> arity;
    for(int i=0; i < imax; i++)
        values[i] = popSt(st);
    values[imax] = NULL;
    
    pushSt(st, joinTrees(op, values));
    return 0;
}

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
    
    for(Token** currentToken = expr; currentToken != NULL; currentToken++)
    {
        switch((*currentToken)->type)
        {
            //note: applying operators means applying them to the top elements of the values stack
            case operator: ;
                Operator* currOp = (*currentToken) -> value;
                if(currOp -> assoc == left)
                {
                    //apply operators to values until the priority of the top element of the stack is lower than the priority of the current element
                    while(1)
                    {
                        if(isEmptySt(operators))
                            break;
                        
                        Token* opTop = topSt(operators);
                        //this should only be operator or opening bracket
                        if(opTop -> type != operator)
                            break;
                        if(((Operator*) opTop -> value) -> priority < currOp -> priority)
                            break;
                        
                        
                        Operator* opRem = (Operator*) popSt(operators);
                        //TODO: error handling
                        applyOpToSt(opRem, values);
                        //TODO: error handling
                    }
                }
                else //right-associative
                {
                    //apply operators to values until the priority of the top element of the stack is lower than the priority of the current element
                    while(1)
                    {
                        if(isEmptySt(operators))
                            break;
                        
                        Token* opTop = topSt(operators);
                        //this should only be operator or opening bracket
                        if(opTop -> type != operator)
                            break;
                        if(((Operator*) opTop -> value) -> priority <= currOp -> priority)
                            break;
                        
                        Operator* opRem = (Operator*) popSt(operators);
                        //TODO: error handling
                        applyOpToSt(opRem, values);
                        //TODO: error handling
                    }
                }
                //push the new operator (still packed as a token) onto the stack
                pushSt(operators, *currentToken);
                break;
            case value: ;
                Tree* valueTree;
                valueTree = newTree((*currentToken) -> value);
                if(valueTree == NULL)
                {
                    printf("eval line __LINE__: couldn't allocate tree node\n");
                    //TODO: deallocate stacks
                }
                Tree* newVal = newTree((*currentToken) -> value);
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
                opTop = (Token*) popSt(operators);
                while(opTop -> type != openBracket)
                {
                    //opTop should be of operator type
                    applyOpToSt((Operator*) opTop -> value, values);
                    opTop = (Token*) popSt(operators);
                }
                //removes opening bracket
                //chyba nie (void) popSt(operators);
                break;
            //TODO: default: error
        }
        
    }
    
    
    //apply operators to values until there is only one value left
    //TODO: if there are any operators left, return some error
    while(sizeSt(values) > 1)
    {
        Operator* opTop = topSt(operators);
        //TODO: error handling
        applyOpToSt(opTop, values);
    }
    Tree* ans = popSt(values);
        
    //TODO: deallocate stuff (stacks and perhaps something more)
    
    return ans;
}

double sum(double* args){
    return args[0] + args[1];
}

double diff(double* args){
    return args[0] - args[1];
}

double mult(double* args){
    return args[0] * args[1];
}

double my_div(double* args){
    return args[0] / args[1];
}

double my_sqrt(double* args){
    //TODO
    return 0;
}

double sqr(double* args){
    //TODO
    return 0;
}

double my_pow(double* args){
    //TODO
    return 0;
}

Operator* newOperator(int priority, int arity, double (*function)(double*), notation_type notation, direction_type assoc)
{
    //TODO
    return NULL;
}

#define opsCount 7

int main(int argc, char* args[])
{    
    //TODO: CHECK: allocate/initialize operators
    Operator ops[opsCount] = {
                //infix operators:
                     {.priority = 5,
                      .arity = 2,
                      .function = sum,
                      .notation = infix,
                      .assoc = left},
                     {.priority = 5,
                      .arity = 2,
                      .function = diff,
                      .notation = infix,
                      .assoc = left},
                     {.priority = 6,
                      .arity = 2,
                      .function = mult,
                      .notation = infix,
                      .assoc = left},
                     {.priority = 6,
                      .arity = 2,
                      .function = my_div,
                      .notation = infix,
                      .assoc = left},
                //functions:
                     {.priority = 1,
                      .arity = 1,
                      .function = my_sqrt,
                      .notation = prefix,
                      .assoc = right},
                     {.priority = 1,
                      .arity = 1,
                      .function = sqr,
                      .notation = prefix,
                      .assoc = right},
                     {.priority = 1,
                      .arity = 2,
                      .function = my_pow,
                      .notation = prefix,
                      .assoc = right}
                    };
    
    //TODO: allocate tokens
    
    /*
    //size: opsCount operators, brackets (2), value and NULL pointer to indicate end
    Token** tokens = malloc((opsCount + 4) * sizeof(*tokens));
    for(int i=0; i < opsCount; i++)
    {
        tokens[i] = malloc(sizeof(*tokens));
        *tokens[i] = {.type = operator, .value = &ops[i]};
    }
    */
    
    /*
     * TODO: initialize tokenizer
     * TODO: read a string and tokenize it
     * TODO: eval(opTree);
     * TODO: free tokenizer and operators
     */
    
    
    return 0;
}