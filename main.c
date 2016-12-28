#include "parser.h"
#include "tree.h"
#include "stack.h"
#include "operator.h"

typedef enum
{
    operator,
    value,
    openBracket,
    endBracket
} atom;

typedef struct
{
    atom type;
    void* value;
} Token;

Tree* makeOpTree(Token** expr)
{
    Stack* operators = newStack();
    if(operators == NULL)
    {
        printf("Couldn't allocate memory for operators stack\n");
        return;
    }
    
    //values - stack of trees that represent a value to be calculated
    //  (the value can be obtained by traversing the tree)
    Stack* values = newStack();
    if(values = null)
    {
        printf("Couldn't allocate memory for values stack\n");
        deleteStack(operators);
        return;
    }
    
    for(Token** currentToken = expr; currentToken != NULL; currentToken++)
    {
        switch((*currentToken)->type)
        {
            //note: applying operators means applying them to the top elements of the values stack
            case operator:
                Operator* currOp = (*currentToken) -> value;
                if(currOp -> assoc == left)
                {
                    //apply operators to values until the priority of the top element of the stack is lower than the priority of the current element
                    while(emptySt(operators) || topSt(operators) -> priority < currOp -> priority)
                    {
                        Operator* opTop = (Operator*) popSt(operators);
                        //TODO: error handling
                        applyOpToSt(opTop, values);
                        //TODO: error handling
                    }
                }
                else //right-associative
                {
                    //apply operators to values until the priority of the top element of the stack is lower than the priority of the current element
                    while(emptySt(operators) || topSt(operators) -> priority <= currOp -> priority)
                    {
                        Operator* opTop = (Operator*) popSt(operators);
                        //TODO: error handling
                        applyOpToSt(opTop, values);
                        //TODO: error handling
                    }
                }
                //push the new operator onto the stack
                pushSt(operators, currOp);
                break;
            case value:
                Tree* valueTree = newTree((*currentToken) -> value);
                if(valueTree == NULL)
                {
                    printf("eval line __LINE__: couldn't allocate tree node\n");
                    //TODO: deallocate stacks
                }
                Tree* newVal = newTree((*currentToken)) -> value);
                if(newVal == NULL)
                {
                    //TODO: error handling
                    return;
                }
                if(pushSt(values, newVal))
                {
                    //TODO: error handling
                    return;
                }
                break;
            case openBracket:
                pushSt(operators, expr[i]);
                break;
            case endBracket:
                //TODO: handle errors: popSt, applyOp
                Operator* opTop = (Operator*) popSt(operators)
                while(opTop -> type != openBracket)
                {
                    applyOpToSt(opTop, values)
                    opTop = (Operator*) popSt(operators);
                }
                //removes opening bracket
                (void) popSt(operators);
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

int main(int argc, char* args[])
{    
    /*
     * TODO: allocate/initialize operators
     * TODO: initialize parser
     * TODO: read a string and parse it (to Token**)
     * TODO: eval(expr);
     * TODO: free parser and operators
     */
    
    return 0;
}