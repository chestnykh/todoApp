CC = gcc
CCFLAGS = -c -ansi -O2 -Wall -march=native -D_XOPEN_SOURCE -D_GNU_SOURCE -Wpointer-compare
LDFLAGS = $(shell pkg-config --libs ncurses)

BIN = ../bin/
TMP = $(BIN)build/
SRC = 


OBJECTS = $(TMP)graphics.o \
	  $(TMP)globals.o \
	  $(TMP)todo.o

APP        = $(BIN)todoApp
.PHONY:      clean run app


app: $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $(APP)

run: $(APP)
	./$(APP) -d $(BIN)data.txt

$(TMP)todo.o: $(SRC)todo.c $(SRC)todo.h
	$(CC) $(CCFLAGS) $(SRC)todo.c -o $@
$(TMP)graphics.o: $(SRC)graphics.c
	$(CC) $(CCFLAGS) $(SRC)graphics.c -o $@
$(TMP)globals.o: $(SRC)globals.c
	$(CC) $(CCFLAGS) $(SRC)globals.c -o $@
	

clean:
	rm -rf $(TMP)* $(APP)
