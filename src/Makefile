CC = gcc
CCFLAGS = -c -O2 -Wall -march=native -D_XOPEN_SOURCE -D_GNU_SOURCE -Wno-unused-variable -ggdb3
LDFLAGS = $(shell pkg-config --libs ncurses)

BIN = ../bin/
TMP = $(BIN)build/
SRC = 

APP        = $(BIN)todoApp
TODO_O     = $(TMP)todo.o
GRAPHICS_O = $(TMP)graphics.o
GLOBALS_O = $(TMP)globals.o
.PHONY:      clean run app


app: $(TODO_O) $(GRAPHICS_O) $(GLOBALS_O)
	$(CC) $(TODO_O) $(GRAPHICS_O) $(GLOBALS_O) $(LDFLAGS) -o $(APP)

run: $(APP)
	./$(APP) -d $(BIN)data.txt

$(TODO_O): $(SRC)todo.c $(SRC)todo.h
	$(CC) $(CCFLAGS) $(SRC)todo.c -o $(TODO_O)
$(GRAPHICS_O): $(SRC)graphics.c
	$(CC) $(CCFLAGS) $(SRC)graphics.c -o $(GRAPHICS_O)
$(GLOBALS_O): $(SRC)globals.c
	$(CC) $(CCFLAGS) $(SRC)globals.c -o $(GLOBALS_O)
	

clean:
	rm -rf $(TMP)* $(APP)
