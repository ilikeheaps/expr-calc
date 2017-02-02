#pragma once
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

double sum(double* args);
double diff(double* args);
double mult(double* args);
double my_div(double* args);
double my_sqrt(double* args);
double sqr(double* args);
double my_pow(double* args);
double sum2(double* args);
Operator* newOperator(int priority, int arity, double (*function)(double*), notation_type notation, direction_type assoc);