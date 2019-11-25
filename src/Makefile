CC = gcc -g -Wall -Werror
SRC_C = main.c gui.c logger.c sys_info.c pid_parser.c
SRC_O = main.o gui.o logger.o sys_info.o pip_parser.o
EXECUTABLE = main
GTK_PACKAGE = `pkg-config --cflags gtk+-3.0`

all: main

.PHONY: main
main:
	$(CC) $(GTK_PACKAGE) -o $(EXECUTABLE) $(SRC_C) $(GTK_PACKAGE)


.PHONY: clean
clean:
	rm -f $(EXECUTABLE) $(SRC_O)
