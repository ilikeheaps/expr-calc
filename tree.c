#include "tree.h"
#include "stack.h"
#include <stdlib.h>

struct Tree
{
    void* value;
    Tree* left;
    Tree* right;
};

struct TreeIterator
{
    Stack* parents;
    Tree* current;
};

/*
 * returns NULL on failure (memory allocation error)
 */
TreeIterator* newTreeIt(Tree* tree)
{
    TreeIterator* ret = malloc(sizeof(*ret));
    if(ret != NULL)
    {
        ret -> parents = newStack(INIT_STACK_SIZE); //defined in tree.h
        ret -> current = tree;
    }
    return ret;
}




void* DFT(Tree* tree, void*(*fun)(void* left_value, void* right_value, void* value))
{
    if(tree == NULL)
        return NULL;
    else
        return fun(DFT(tree->left, fun), DFT(tree->right, fun), tree->value);
}


/*
 * returns NULL pointer upon failure (memory allocation error)
 */
Tree* newTree(void* value)
{
    Tree* pntr = malloc(sizeof(*pntr));
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
Tree* joinTrees(Tree* left, Tree* right, void* value)
{
    Tree* pntr = malloc(sizeof(*pntr));
    if(pntr != NULL)
    {
        pntr->value = value;
        pntr->left = left;
        pntr->right = right;
    }
    return pntr;
}