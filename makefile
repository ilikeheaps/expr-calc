CC = gcc
CFLAGS = -Wall
OBJ = main.o stack.o tree.o


calc: $(OBJ)
	$(CC) $(CFLAGS) -o calc $<

*.o :
	$(CC) $(CFLAGS) -c -o $@ $<

clear:
	rm *.o

stack.o: stack.c stack.h
main.o: main.c
tree.o: tree.c tree.h stack.h
sort.o: sort.c sort.h
