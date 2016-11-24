/*
   Basic stack structure for FIFO queue. When removing elements, it won't deallocate the data of elements it contained so you have to  handle it when removing each element. You can't delete a non-empty stack. The stack can't contain NULL element (some functions return NULL as an error value).
 */
#include "stack.h"
#include <stdlib.h>

struct Stack
{
    int size;
    int capacity;
    void** data;
};

/*
 * NULL - memory allocation error
 */
Stack* newStack(int size)
{
    Stack* s = malloc(sizeof(*s));
    if(s == NULL)
        return NULL;
    
    s->data = malloc(size*sizeof(*s->data));
    if(s->data == NULL)
    {
        free(s);
        return NULL;
    }
    
    s->capacity = size;
    s->size = 0;
    
    return s;
}

/*
 * 1 - stack contains elements and can't be removed
 */
int deleteStack(Stack* s)
{
    if(s -> size != 0)
        return 1;
    else
    {
        free(s->data);
        free(s);
        return 0;
    }
}

/*
 * 1 - couldn't allocate memory for the new element
 */
int pushSt(Stack* s, void* el)
{
    if(s->size >= s->capacity)
    {
        int new_capacity = 2*s->size;
        void* tmp = realloc(s->data, new_capacity*sizeof(*s->data));
        while(tmp == NULL && new_capacity > s->size)
        {
            new_capacity = (new_capacity + s->size)/2;
            tmp = realloc(s->data, new_capacity*sizeof(*s->data));
        }
        
        if(tmp != NULL)
        {
            s->data = tmp;
            s->capacity = new_capacity;
        }
        else
            return 1;
    }
    
    s->data[s->size++] = el;
    return 0;
}

/*
 * NULL - stack is empty
 */
void* topSt(Stack* s)
{
    if(s->size > 1)
        return s->data[s->size - 1];
    else
        return NULL;
}

/* Doesn't reduce the stack capacity.
 * 
 * Return values on error:
 * NULL - no elements on the stack
 */
void* popSt(Stack* s)
{
    if(s->size < 1)
        return NULL;
    
    return s->data[--s->size];
}

int sizeSt(Stack* s)
{
    return s->size;
}

int isEmptySt(Stack* s)
{
    if(s->size == 0)
        return 1;
    else
        return 0;
}

/*
 * 1 - failed to reallocate memory, stack unchanged
 * 2 - attempting to set the capacity smaller than size (would lead to memory corruption)
 */
int setSizeSt(Stack* s, int new_capacity)
{
    if(new_capacity < s->size)
        return 2;
    
    //TODO check if *s->data is *(s->data)
    void* tmp = realloc(s->data, new_capacity*sizeof(*s->data));
    if(tmp != NULL)
    {
        s->data = tmp;
        return 0;
    }
    else
        return 1;
}