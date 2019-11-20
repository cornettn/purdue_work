#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fctl.h>

int g_log_fd = -1;

int init_log() {
	int fd = open("log_messages.log", O_RDWR | O_APPEND | O_CREAT, 0600); 
	if (fd < 0) {
		perror("Could not open log file");
		exit(EXIT_FAILURE);
	}

	g_log_fd = fd;
	return 1;
}

void mylog(char *msg) {
	dprintf(g_log_fd, "%s\n", msg);
}