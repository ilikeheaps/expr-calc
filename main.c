#include "parser.h"
#include "tree.h"
#include "stack.h"

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

typedef enum
{
    bifix,
    prefix,
    infix
} notation;

typedef struct
{
    int priority;
    int arity;
    double (*function)(); //???: arity arguments
    notation type;
} Operator;
    
    

void eval(Token** expr, int tokenCount)
{
    Stack* operators = newStack();
    
    //values - stack of trees that represent a value
    //  (the value can be obtained by traversing the tree)
    Stack* values = newStack();
    
    
    if(operators == NULL || values = null)
    {
        printf("Stack memory allocation error\n");
        return;
    }
    
    for(int i=0; i<tokenCount; i++)
    {
        switch(expr[i]->type)
        {
            //note: applying operators means applying them to the top elements of the values stack
            case operator:
                /*TODO
                 * 1) apply operators to values until the priority of the top element of the stack is equal to or lower than the priority of the current element (or only lower depending on the associativity)
                 * 2) push the new operator onto the stack
                 */
                break;
            case value:
                //TODO: handle newTree and pushSt errors
                pushSt(values, newTree(expr[i] -> value));
                break;
            case openBracket:
                pushSt(operators, expr[i]);
                break;
            case endBracket:
                //TODO: apply operators up to openBracket (and remove openBracket)
                break;
                //TODO: default
        }
        
    }
    
    //TODO: apply operators to values until there is only one value left
    // if there are any operators left, return some error
}

int main(int argc, char* args[])
{    
    /*
     * TODO: initialize parser
     * TODO: read a string and parse it (to Token**)
     * TODO: eval(expr);
     */
    
    
    return 0;
}