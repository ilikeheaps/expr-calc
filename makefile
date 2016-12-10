CC = gcc
CFLAGS = -Wall
OBJ = main.o stack.o bintree.o


calc: $(OBJ)
	$(CC) $(CFLAGS) -o calc $<

*.o :
	$(CC) $(CFLAGS) -c -o $@ $<

clear:
	rm -f *.o

delete: clear
	rm calc

stack.o: stack.c stack.h
main.o: main.c
bintree.o: bintree.c bintree.h stack.o
sort.o: sort.c sort.h
