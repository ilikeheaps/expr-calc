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
    
    Tree** values = malloc( (op->arity + 1) * sizeof(*values) );
    
    int imax = op -> arity;
    for(int i=0; i < imax; i++)
        values[i] = popSt(st);
    values[imax] = NULL;
    
    pushSt(st, joinTrees(op->function, values));
    return 0;
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
        Token* opToken = popSt(operators);
        Operator* opTop = opToken -> value;
        //TODO: error handling
        applyOpToSt(opTop, values);
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
    Token** tokens = tokenizer_process(exp);
    
    //make operation tree and evaluate it
    Tree* opTree = makeOpTree(tokens);
    double val = calcNode(opTree);
    
    //free operation tree and contained values
    deleteOpTree(opTree);
    
    return val;
}

int main(int argc, char* args[])
{    
    //initialize tokenizer
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