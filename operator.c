#include "stack.h"
#include "operator.h"
#include "tree.h"

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
    
    pushSt(joinTrees(op, values));
    return 0;
}