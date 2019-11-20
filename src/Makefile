CC = gcc -g -Wall -Werror

all: main

.PHONY: main
main:
	$(CC) `pkg-config --cflags gtk+-3.0` -o main main.c gui.c logger.c `pkg-config --libs gtk+-3.0`


.PHONY: clean
clean:
	rm -f main
