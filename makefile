CC = gcc
CFLAGS = -Wall
OBJ = stack.o tree.o tokenizer.o operator.o token.o


calc: main.c $(OBJ)
	$(CC) $(CFLAGS) -o calc $^

*.o :
	$(CC) $(CFLAGS) -c -o $@ $<

clear:
	rm -f *.o

delete: clear
	rm calc

stack.o: stack.c stack.h
tree.o: tree.c tree.h
sort.o: sort.c sort.h
operator.o: operator.c operator.h
tokenizer.o: tokenizer.c tokenizer.h tree.h operator.h
token.o: token.c token.h