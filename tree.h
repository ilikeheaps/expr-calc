struct Tree;
typedef struct Tree Tree;

void* DFT(Tree* tree, void*(*fun)(void* left_value, void* right_value, void* value));

Tree* newTree(void* value);

Tree* joinTrees(Tree* left, Tree* right, void* value);