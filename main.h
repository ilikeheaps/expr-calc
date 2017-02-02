typedef enum
{
    prefix,
    infix
} notation_type;

typedef enum
{
    left,
    right
} direction_type;

typedef struct
{
    int priority;
    int arity;
    double (*function)(double*); //takes an array of arity values as argument
    notation_type notation;
    direction_type assoc; //associativity
} Operator;

