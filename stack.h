/*
   Basic stack structure for FIFO queue. When removing elements, it won't deallocate the data of elements it contained so you have to  handle it when removing each element. You can't destroy a non-empty stack. The stack can't contain null element (some functions return null as an error value).
 */
struct Stack;
typedef struct Stack Stack;

/*
 * 1 - couldn't allocate memory for the new element
 */
int pushSt(struct Stack*, void* el);

/*
 * null - stack is empty
 */
void* topSt(struct Stack*);

/* Doesn't reduce the stack capacity.
 * 
 * Return values on error:
 * null - no elements on the stack
 */
void* popSt(struct Stack*);

int sizeSt(struct Stack*);

int isEmptySt(struct Stack*);

/*
 * 1 - failed to reallocate memory, stack unchanged
 * 2 - attempting to set the capacity smaller than size (would lead to memory corruption)
 */
int setSizeSt(struct Stack*, int);

Stack* newStack(int);

int deleteStack(Stack*);
