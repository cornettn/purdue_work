CC = gcc -g -Wall -Werror

all: main

.PHONY: main
gui:
	$(CC) `pkg-config --cflags gtk+-3.0` -o main main.c `pkg-config --libs gtk+-3.0`
