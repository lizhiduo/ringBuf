CC ?= gcc -g

BIN := demo

SRC := $(wildcard *.c)

all:
	$(CC) -o $(BIN) $(SRC) -lpthread

clean:
	rm -rf *.o $(BIN)

