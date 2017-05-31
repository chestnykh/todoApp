CC = gcc

all: todo.c todo.h
	$(CC) todo.c -o todo -lncurses -D_XOPEN_SOURCE

clean:
	rm -rf todo
