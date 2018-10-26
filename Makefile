CC ?= gcc

BIN := demo

SRC := $(wildcard *.c)

all:
	$(CC) -o $(BIN) $(SRC)

clean:
	rm -rf *.o $(BIN)

