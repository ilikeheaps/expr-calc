#include "stack.h"

typedef enum
{
    bifix,
    prefix,
    infix
} notation;

typedef struct
{
    int priority;
    int arity;
    double (*function)(double**); //takes an NULL-terminated array of arity values as argument
    notation type;
} Operator;

void applyOpToSt(Operator*, Stack*);