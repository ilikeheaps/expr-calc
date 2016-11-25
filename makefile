CC = gcc
CFLAGS = -Wall
OBJ = main.o stack.o tree.o

calc: $(OBJ)
	$(CC) $(CFLAGS) -o calc $<

stack.o: stack.c stack.h
	$(CC) $(CFLAGS) -c -o $@ $<

main.o: main.c
	$(CC) $(CFLAGS) -c -o $@ $<

tree.o: tree.c tree.h stack.h
	$(CC) $(CFLAGS) -c -o $@ $<

clear:
	rm *.o