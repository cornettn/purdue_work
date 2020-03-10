CC = gcc -g -Wall -Werror
SRC_C = main.c gui.c logger.c sys_info.c pid_parser.c graph_info.c file_system.c
SRC_O = main.o gui.o logger.o sys_info.o pip_parser.o graph_info.o file_system.o
EXECUTABLE = main
GTK_PACKAGE = `pkg-config --cflags gtk+-3.0 --libs`

all: main

.PHONY: main
main:
	$(CC) $(GTK_PACKAGE) $(SRC_C) -lm -o $(EXECUTABLE) $(GTK_PACKAGE)


.PHONY: clean
clean:
	rm -f $(EXECUTABLE) $(SRC_O)
