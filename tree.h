struct Tree;
typedef struct Tree Tree;

Tree* new Tree(void* value);

Tree* joinTrees(void* value, Tree** trees);

void* TreeDFT(Tree* startNode, void* (*fun)(void*, void**));

void* deleteTree(Tree* root, void* (*deleteValue)(void*));