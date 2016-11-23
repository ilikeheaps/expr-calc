#include "tree.h"
#include <stdlib.h>

struct Tree
{
    void* value;
    Tree* left;
    Tree* right;
};



void* DFT(Tree* tree, void*(*fun)(void* left_value, void* right_value, void* value))
{
    if(tree == NULL)
        return NULL;
    else
        return fun(DFT(tree->left, fun), DFT(tree->right, fun), tree->value);
}


/*
 * returns NULL pointer upon failure
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