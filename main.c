#include "parser.h"
#include "tree.h"
#include "stack.h"
#include "tokenizer.h"
#include <stdlib.h>
#include <stdio.h>



//returns -1 if the stack doesn't have enough values to apply the function
int applyOpToSt(Operator* op, Stack* st)
{
    printf("  op: %p\n", op);
    printf("  Stack size: %d\n", sizeSt(st));
    printf("  Expecting %d arguments\n", op -> arity);
    if(sizeSt(st) < op -> arity)
        return -1;
    
    printf("  Seg1\n");
    
    Tree** values = malloc( (op->arity + 1) * sizeof(*values) );
    
    printf("  Seg2\n");
    
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
                        && ((Operator*) opTop -> value) -> priority < currOp -> priority)
                    {
                        break;
                    }
                    if( currOp -> assoc == right
                        && ((Operator*) opTop -> value) -> priority <= currOp -> priority)
                    {
                        break;
                    }
                    
                    Operator* opRem = (Operator*) popSt(operators);
                    //TODO: error handling
                    applyOpToSt(opRem, values);
                    //TODO: error handling
                }
                
                //push the new operator (still packed as a token) onto the stack
                printf("  Pushing %p onto operator stack\n", *currentToken);
                pushSt(operators, *currentToken);
                break;
            case value: ;
                printf("  Found value\n");
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
                printf("  Found opening bracket\n");
                pushSt(operators, *currentToken);
                break;
            case endBracket: ;
                printf("  Found ending bracket\n");
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
    printf("Parsed all tokens, reducing stacks\n");
    
    
    //apply operators to values until there is only one value left
    //TODO: if there are any operators left, return some error
    while(sizeSt(values) > 1)
    {
        printf(" Operators stack size: %d\n", sizeSt(operators));
        Token* opToken = popSt(operators);
        printf(" token: %p\n", opToken);
        Operator* opTop = opToken -> value;
        //TODO: error handling
        printf(" Applying operator to stack...\n");
        applyOpToSt(opTop, values);
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

#define max_line_len 100

char input[max_line_len + 1];

int main(int argc, char* args[])
{    
    
    
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
    
    //initialize tokenizer
    printf("Initializing tokenizer\n");
    tokenizer_initialize();
    
    //read a string and tokenize it
    printf("Input: ");
    scanf("%s", input);
    
    printf("Tokenizing expression\n");
    Token** tokens = tokenizer_process(input);
    
    Token** tok;
    for(tok = tokens; *tok != NULL; tok++);
    printf("# of tokens: %ld\n", tok - tokens);
    
    //make operation tree and evaluate it
    printf("Parsing tokens\n");
    Tree* opTree = makeOpTree(tokens);
    printf("Calculating value\n");
    double val = calcNode(opTree);
    
    printf("%lf\n", val);
    
    
    //TODO: free tokenizer, operation tree and operators
    
    return 0;
}