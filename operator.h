#include "stack.h"

typedef enum
{
    prefix,
    infix
} notation;

typedef struct
{
    int priority;
    int arity;
    double (*function)(double**); //takes a n NULL-terminated array of arity values as argument
    notation type;
} Operator;

int applyOpToSt(Operator*, Stack*);