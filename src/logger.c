#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>

#define BUF_SIZE (1024)
#define SEPERATOR ("-----------------------------------------------")
#define UNUSED(x) (void)(x)

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

	/* Get the timestamp for the logs */

	time_t curr_time = time(NULL);
	struct tm *local_time = localtime(&curr_time);
	char *timestamp = (char *) malloc(BUF_SIZE);
	
	/* Copy the time into timestamp in format 'MM/DD/YY hh:mm:ss' */

	int size = strftime(timestamp, BUF_SIZE, "%x %X", local_time);
	UNUSED(size);

	/* Log the message */

	dprintf(g_log_fd, "%s %s\n", timestamp, msg);

	/* Free malloc()'d memory */

	free(timestamp);
	timestamp = NULL;
}

void stop_logging() {
	dprintf(g_log_fd, "%s\n", SEPERATOR);
	close(g_log_fd);
}
