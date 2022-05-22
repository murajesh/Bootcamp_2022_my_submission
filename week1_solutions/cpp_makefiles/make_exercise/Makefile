CC=g++
DEPS=functions.h
CFLAGS=-I.
OBJ= main.o hello.o factorial.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAG)
