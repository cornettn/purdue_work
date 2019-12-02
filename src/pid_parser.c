#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/sysinfo.h>

#include "pid_parser.h"


/* Defines? */


/* Global Variables? */


/* Initialize? */


/*
 * Minimum requirements for the Process Information tab
 *   - Table with options to:
 *       - Display list of all running processes
 *       - Display processes only owned by current user
 *       - Display the list in tree format
 *   - This list should be updated at a fixed interval OR by using a refresh button
 *
 *   - For each individual process you should have the ability to:
 *       - Stop
 *       - Continue
 *       - Kill
 *       - List memory maps
 *       - List open files
 *
 *   - Each individual process should have a window that shows:
 *       - Process Name
 *       - User
 *       - State
 *       - Memory
 *       - Virtual memory
 *       - Resident memory
 *       - Shared memory
 *       - CPU time
 *       - Date/time started
 */


/* Functions */



process_t *create_pid_list() {

  int count = 0;
  process_t pid_list[1000];

  struct dirent* dir;
  DIR* start_dir = opendir("/proc");

  while ((dir = readdir(start_dir)) != NULL) {
    char *ptr;
    int pid = 0;
    if (strtol(dir->d_name, &ptr, pid) == 0) {
      break;
    }

    pid_list[count].pid = strtol(dir->d_name, &ptr, pid);
    pid_list[count].cpu_perc = 0;

    /* Getting PID Name */
    char * path = "/proc/";
    strcat(path, dir->d_name);

    char *path_stat = path;
    strcat(path_stat, "/stat");

    FILE *fp = fopen(path_stat, "r");
    fscanf(fp, "%*d (%s) %s", pid_list[count].proc_name, pid_list[count].state);
    fclose(fp);
    fp = NULL;

    /* ----- */


    char *path_mstat = path;
    strcat(path_mstat, "/mstat");

    fp = fopen(path_mstat, "r");

    double rss = 0;
    double swap = 0;
    fscanf(fp, "%*s %lf %*s %*s %*s %lf", &rss, &swap);
    fclose(fp);
    fp = NULL;

    pid_list[count].memory = rss + swap;

    count++;
  }
  //pid_list[count] = malloc(sizeof(char));
  //pid_list[count] = '\0';
  return 0;
}


int get_proc_num() {
  struct sysinfo info;
  sysinfo(&info);
  return info.procs;
}




















/* End pid_parser.c */
