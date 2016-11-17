# expr-calc
Simple program in C for evaluating arithmetic expressions.

#Functional requirements:
1. operates on floating point numbers
2. expressions are composed of infix operators, parantheses, one-argument functions and constants
3. each operation has a priority that defines operation precedence
4. can work either in interactive mode in which it evaluates each expression on the input (separated by line break) or evaluate expression given in program arguments

#Operators
1. multiplying: "*"
2. division: "/"
3. addition: "+"
4. subtraction: "-"
5. exponentation: "^"

#Functions:
1. square root: "sqrt( )"
2. square: "sqr( )"

#Implementation details:
1. may assume maximum length of expression to be 1000 characters
2. transforms the expression into operation tree (Polish notation equivalent)
3. linear complexity (respective to the number of operations and constants in expession)
4. transformation algorithm uses stack structure
5. tree is represented by appropriate structure and is allocated dynamically
6. calculations are done during depth first traversal in postorder
