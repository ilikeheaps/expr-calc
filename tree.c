#include "tree.h"
#include <stdlib.h>

//returns NULL on memory allocation failure
Tree* newTree(void* value, int children_count)
{
    Tree* ret = malloc(sizeof(*ret));
    if(ret != NULL)
    {
        ret -> children = calloc(children_count, sizeof( *(ret -> children) ));
        ret -> children_count = children_count;
        ret -> value = value;
    }
    return ret;
}

//returns NULL on memory allocation failure
Tree* joinTrees(void* value, Tree** trees, int tree_count)
{
    Tree* ret = malloc(sizeof(*ret));
    if(ret != NULL)
    {
        ret -> children_count = tree_count;
        ret -> children = trees;
        ret -> value = value;
    }
    return ret;
}

//careful: fun must deallocate the second argument if it doesn't keep it
void* TreeDFT(Tree* start_node, void* (*fun)(void*, void**, int))
{
    int children_count = start_node -> children_count;
    
    void** children_values = malloc(children_count*sizeof(*children_values));
    Tree** children = start_node -> children;
    
    for(int i=0; i < children_count; i++)
        if(children[i] != NULL)
            children_values[i] = TreeDFT(children[i], fun);
        else
            children_values[i] = NULL;
    
    return fun(start_node->value, children_values, children_count);
}

void deleteTree(Tree* root, void (*delete_value)(void*, int))
{
    if(root == NULL)
        return;
    
    int children_count = root -> children_count;
    
    for(int i=0; i < children_count; i++)
        deleteTree(root->children[i], delete_value);
    free(root -> children);
    
    delete_value(root -> value, root -> children_count);
    free(root);
}
        