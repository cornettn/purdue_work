CC = gcc -g -Wall -Werror
SRC_C = main.c gui.c logger.c sys_info.c pid_parser.c graph_info.c file_system.c
SRC_O = main.o gui.o logger.o sys_info.o pip_parser.o graph_info.o file_system.o
EXECUTABLE = main

all: main

.PHONY: main
main:
	$(CC) `pkg-config --cflags gtk+-3.0` -o SRC_C `pkg-config --libs gtk+-3.0`


.PHONY: clean
clean:
	rm -f main SRC_O
