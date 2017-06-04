CC = gcc
CCFLAGS = -O2 -Wall -Wnull-dereference -march=native -D_XOPEN_SOURCE -D_GNU_SOURCE -Wno-unused-variable

LDFLAGS = $(shell pkg-config --libs ncurses)

APP = todoApp


OBJECTS = todo.o color.o resize_term.o
.PHONY: clean run app

run: $(APP)
	./$(APP)

$(APP): $(OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $(APP)


.c.o:
	$(CC) $(CCFLAGS) -c $<

#dependencies
todo.o: todo.c todo.h color.h
color.o: color.c
resize_term.o: resize_term.c


clean:
	rm -rf $(OBJECTS) $(APP)
