#include "tree.h"
#include <stdlib.h>

struct Tree{
    Tree** children; //null-terminated array
    void* value;
};

//returns NULL on memory allocation failure
Tree* newTree(void* value)
{
    Tree* ret = malloc(sizeof(*ret));
    if(ret != NULL)
    {
        ret -> children = NULL;
        ret -> value = value;
    }
    return ret;
}

//returns NULL on memory allocation failure
Tree* joinTrees(void* value, Tree** trees)
{
    Tree* ret = malloc(sizeof(*ret));
    if(ret != NULL)
    {
        ret -> children = trees;
        ret -> value = value;
    }
    return ret;
}

//careful: fun must deallocate the second argument if it doesn't keep it
void* TreeDFT(Tree* startNode, void* (*fun)(void*, void**))
{
    Tree** current = startNode -> children;
    int childrenCount = 0;
    
    while(*current != NULL)
    {
        childrenCount++;
        current++;
    }
    
    void** childrenValues = malloc((childrenCount + 1)*sizeof(*childrenValues));
    Tree** children = startNode -> children;
    
    for(int i=0; i < childrenCount; i++)
        childrenValues[i] = TreeDFT(children[i], fun);
    childrenValues[childrenCount] = NULL;
    
    return fun(startNode->value, childrenValues);
}

void deleteTree(Tree* root, void (*deleteValue)(void*))
{
    Tree** current = root -> children;
    while(*current != NULL)
    {
        deleteTree(*current, deleteValue);
        current++;
    }
    free(root -> children);
    deleteValue(root -> value);
    free(root);
}
        