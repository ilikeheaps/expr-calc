#define INIT_STACK_SIZE 10 //stack for queuing parents in iterator

struct BinTree;
typedef struct BinTree BinTree;

struct BinTreeIterator;
typedef struct BinTreeIterator BinTreeIterator;

//functions for trees
void* BinTreeDFT(BinTree* tree, void*(*fun)(void* left_value, void* right_value, void* value));

BinTree* newBinTree(void* value);

BinTree* joinBinTrees(BinTree* left, BinTree* right, void* value);

//functions for iterators
BinTreeIterator* newBinTreeIt(BinTree* tree);

