#define INIT_STACK_SIZE 10 //stack for queuing parents in iterator

struct Tree;
typedef struct Tree Tree;

struct TreeIterator;
typedef struct TreeIterator TreeIterator;

//functions for trees
void* DFT(Tree* tree, void*(*fun)(void* left_value, void* right_value, void* value));

Tree* newTree(void* value);

Tree* joinTrees(Tree* left, Tree* right, void* value);

//functions for iterators
TreeIterator* newTreeIt(Tree* tree);

