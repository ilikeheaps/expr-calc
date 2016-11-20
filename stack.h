/*
   Basic stack structure for FIFO queue. When removing elements, it won't deallocate the data of elements it contained so you have to  handle it when removing each element. You can't destroy a non-empty stack. The stack can't contain null element (some functions return null as an error value).
 */
struct Stack;

/*
 * 1 - couldn't allocate memory for the new element
 */
int push(struct Stack*, void* el);

/*
 * null - stack is empty
 */
void* top(struct Stack*);

/* Doesn't reduce the stack capacity.
 * 
 * Return values on error:
 * null - no elements on the stack
 */
void* pop(struct Stack*);

int size(struct Stack*);

int isEmpty(struct Stack*);

/*
 * 1 - failed to reallocate memory, stack unchanged
 * 2 - attempting to set the capacity smaller than size (would lead to memory corruption)
 */
int setSize(struct Stack*, int);