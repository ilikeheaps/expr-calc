struct Tree;
typedef struct Tree Tree;

Tree* new Tree(void* value);

Tree* joinTrees(void* value, Tree** trees);

(void*) TreeDFT(Tree*, void* (*fun)(void**, void*));