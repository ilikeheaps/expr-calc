#include "operator.h"
#include <stdlib.h>

double sum(double* args){
    return args[0] + args[1];
}

double diff(double* args){
    return args[0] - args[1];
}

double mult(double* args){
    return args[0] * args[1];
}

double my_div(double* args){
    return args[0] / args[1];
}

double my_sqrt(double* args){
    //TODO
    return 0;
}

double sqr(double* args){
    //TODO
    return 0;
}

double my_pow(double* args){
    //TODO
    return 0;
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