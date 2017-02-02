struct Tree
{
    Tree** children; //null-terminated array
    void* value;
};

typedef struct Tree Tree;

Tree* newTree(void* value);

Tree* joinTrees(void* value, Tree** trees);

//careful: fun must deallocate the second argument if it doesn't keep it
void* TreeDFT(Tree* startNode, void* (*fun)(void*, void**));

void deleteTree(Tree* root, void (*deleteValue)(void*));