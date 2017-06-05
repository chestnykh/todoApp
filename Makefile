CC = gcc
CCFLAGS = -c -O2 -Wall -march=native -D_XOPEN_SOURCE -D_GNU_SOURCE -Wno-unused-variable
LDFLAGS = $(shell pkg-config --libs ncurses)

BIN = bin/
TMP = $(BIN)build/
SRC = src/

APP        = $(BIN)todoApp
TODO_O     = $(TMP)todo.o
GRAPHICS_O = $(TMP)graphics.o
.PHONY:      clean run app


$(APP): $(TODO_O) $(GRAPHICS_O)
	$(CC) $(TODO_O) $(GRAPHICS_O) $(LDFLAGS) -o $(APP)

run: $(APP)
	./$(APP)

$(TODO_O): $(SRC)todo.c $(SRC)todo.h
	$(CC) $(CCFLAGS) $(SRC)todo.c -o $(TODO_O)
$(GRAPHICS_O): $(SRC)graphics.c
	$(CC) $(CCFLAGS) $(SRC)graphics.c -o $(GRAPHICS_O)

clean:
	rm -rf $(TMP)* $(APP)
