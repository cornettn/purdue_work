CC = gcc -g -Wall -Werror

all: gui

.PHONY: gui
gui:
	$(CC) `pkg-config --cflags gtk+-3.0` -o gui gui.c `pkg-config --libs gtk+-3.0`
