CC = gcc
CFLAGS = -Wall
OBJ = main.o stack.o tree.o


calc: main.o stack.o tree.o
	$(CC) $(CFLAGS) -o calc $^

*.o :
	$(CC) $(CFLAGS) -c -o $@ $<

clear:
	rm -f *.o

delete: clear
	rm calc

stack.o: stack.c stack.h
main.o: main.c main.h
tree.o: tree.c tree.h
sort.o: sort.c sort.h
operator.o: operator.c operator.h
