#include "bintree.h"
#include "stack.h"
#include <stdlib.h>

struct BinTree
{
    void* value;
    BinTree* left;
    BinTree* right;
};

struct BinTreeIterator
{
    Stack* parents;
    BinTree* current;
};

/*
 * returns NULL on failure (memory allocation error)
 */
BinTreeIterator* newTreeIt(BinTree* tree)
{
    BinTreeIterator* ret = malloc(sizeof(*ret));
    if(ret != NULL)
    {
        ret -> parents = newStack(INIT_STACK_SIZE); //defined in tree.h
        ret -> current = tree;
    }
    return ret;
}




void* BinTreeDFT(BinTree* tree, void*(*fun)(void* left_value, void* right_value, void* value))
{
    if(tree == NULL)
        return NULL;
    else
        return fun(BinTreeDFT(tree->left, fun), BinTreeDFT(tree->right, fun), tree->value);
}


/*
 * returns NULL pointer upon failure (memory allocation error)
 */
BinTree* newBinTree(void* value)
{
    BinTree* pntr = malloc(sizeof(*pntr));
    if(pntr != NULL)
    {
        pntr->value = NULL;
        pntr->left = NULL;
        pntr->right = NULL;
    }
    return pntr;
}

/*
 * returns NULL pointer upon failure
 */
BinTree* joinBinTrees(BinTree* left, BinTree* right, void* value)
{
    BinTree* pntr = malloc(sizeof(*pntr));
    if(pntr != NULL)
    {
        pntr->value = value;
        pntr->left = left;
        pntr->right = right;
    }
    return pntr;
}