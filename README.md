# expr-calc
Simple program in C for evaluating arithmetic expressions.

## Overview:
1. operates on floating point numbers
2. expressions are composed of infix operators, parantheses, multi-argument functions and constants
3. each operation has a priority that defines operation precedence
4. can work either in interactive mode in which it evaluates each expression on the input (separated by line break) or evaluate expression given in program arguments

## Usage
1. basic operations
  
  All operations in expression have to be written explicitly (`2(1+2)` is wrong) and each label can mean only one operation (so there is no 'negation -', there is only 'subtraction -')
2. sequences
  
  One can input a sequence of expressions separated by commas. Each of them will be evaluated and the result will be displayed.
3. functions
  
  Functions can be called in two ways:
  - `f(expr1, expr2)`
  - `f expr1 expr2`
  
  In the second case if the expressions contain any operators, they need to be wrapped in brackets as functions have higher priority than any operator.


## Operators
1. multiplying: "*"
2. division: "/"
3. addition: "+"
4. subtraction: "-"
5. exponentiation: "^"

## Functions:
1. square root: `sqrt(expr)`
2. square: `sqr(expr)`
3. factorial: `fact(expr)`
4. exponentiation: `pow(expr1, expr2)`
