#pragma once
struct Tree;
typedef struct Tree Tree;

struct Tree
{
    Tree** children; //null-terminated array
    int children_count;
    void* value;
};


Tree* newTree(void* value, int children_count);

Tree* joinTrees(void* value, Tree** trees, int tree_count);

//careful: fun must deallocate the second argument if it doesn't keep it
void* TreeDFT(Tree* startNode, void* (*fun)(void*, void**, int));

void deleteTree(Tree* root, void (*delete_value)(void*, int));