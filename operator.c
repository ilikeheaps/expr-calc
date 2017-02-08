#include "operator.h"
#include <stdlib.h>
#include <math.h>

double sum(double* args){
    return args[1] + args[0];
}

double diff(double* args){
    return args[1] - args[0];
}

double mult(double* args){
    return args[1] * args[0];
}

double my_div(double* args){
    return args[1] / args[0];
}

double my_sqrt(double* args){
    return sqrt(args[0]);
}

double sqr(double* args){
    return args[0] * args[0];
}

double my_pow(double* args){
    return pow(args[1], args[0]);
}

double sum2(double* args){
    return args[1] + args[0];
}

double fact_rec(double a)
{
    if(a <= 0)
        return 1;
    else 
        return a * fact_rec(a-1);
}

double fact(double* args){
    return fact_rec(args[0]);
}

Operator* newOperator(int priority, int arity, double (*function)(double*), notation_type notation, direction_type assoc)
{
    Operator* new = malloc(sizeof(*new));
    
    new -> priority = priority;
    new -> arity = arity;
    new -> function = function;
    new -> notation = notation;
    new -> assoc = assoc;
    
    return new;
}