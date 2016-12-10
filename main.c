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

Tree* eval(Token** expr, int tokenCount)
{
    Stack* operators = newStack();
    if(operators == NULL)
    {
        printf("Couldn't allocate memory for operators stack\n");
        return;
    }
    
    //values - stack of trees that represent a value
    //  (the value can be obtained by traversing the tree)
    Stack* values = newStack();
    if(values = null)
    {
        printf("Couldn't allocate memory for values stack\n");
        deleteStack(operators);
        return;
    }
    
    for(int i=0; i<tokenCount; i++)
    {
        switch(expr[i]->type)
        {
            //note: applying operators means applying them to the top elements of the values stack
            case operator:
                Operator* currOp = expr[i] -> value;
                if(currOp -> type == bifix
                /*TODO
                 * 1) apply operators to values until the priority of the top element of the stack is equal to or lower than the priority of the current element (or only lower depending on the associativity)
                 * 2) push the new operator onto the stack
                 */
                break;
            case value:
                Tree* valueTree = newTree(expr[i] -> value);
                if(valueTree == NULL)
                {
                    printf("eval line __LINE__: couldn't allocate tree node\n");
                    //TODO: deallocate stacks
                }
                Tree* newVal = newTree(expr[i]) -> value);
                if(newVal == NULL)
                {
                    //TODO: error
                    return;
                }
                if(pushSt(values, newVal))
                {
                    //TODO: error
                    return;
                }
                break;
            case openBracket:
                pushSt(operators, expr[i]);
                break;
            case endBracket:
                //TODO: handle errors: popSt, applyOp
                Operator* opTop = popSt(operators)
                while(opTop -> type != openBracket)
                {
                    applyOp(values, opTop)
                    opTop = popSt(operators);
                }
                (void) popSt;
                break;
            //TODO: default: error
        }
        
    }
    
    //TODO: apply operators to values until there is only one value left
    // if there are any operators left, return some error
    
    return topSt(values);
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