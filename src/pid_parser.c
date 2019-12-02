#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/sysinfo.h>

#include "pid_parser.h"


/* Defines */

#define SMALL_BUF (1024)


/* Global Variables */



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


/* Function that creates and fills a list of structs representing processes */

process_t **create_pid_list() {

  int count = 0;
  int num = get_proc_num();
  process_t **pid_list = malloc(sizeof(process_t *) * (num + 1));

  struct dirent* dir;
  DIR* start_dir = opendir("/proc");


  while ((dir = readdir(start_dir))) {
    if (atoi(dir->d_name) != 0) {
      pid_list[count] = malloc(sizeof(process_t));

      pid_list[count]->pid = atoi(dir->d_name);
      pid_list[count]->cpu_perc = 0;
      pid_list[count]->proc_name = get_name(dir->d_name);
      pid_list[count]->state = get_state(dir->d_name);
      pid_list[count]->memory = get_mem(dir->d_name);

      pid_list[count]->mem_maps = NULL;
      pid_list[count]->open_files = NULL;
      pid_list[count]->virt_memory = get_virt_mem(dir->d_name);
      pid_list[count]->res_memory = get_res_mem(dir->d_name);
      pid_list[count]->shared_mem = get_shared_mem(dir->d_name);
      pid_list[count]->cpu_time = 0;
      pid_list[count]->time_started = NULL;
      count++;
    }
  }

  pid_list[count] = NULL;

  return pid_list;
} /* create_pid_list() */


/* Returns the name of the given pid */

char *get_name(char* pid) {
  char * path = malloc(sizeof(char) * SMALL_BUF);
  strcpy(path, "/proc/");
  strcat(path, pid);
  strcat(path, "/stat");

  char * temp = malloc(sizeof(char) * SMALL_BUF);
  FILE *fp = fopen(path, "r");
  fscanf(fp, "%*d (%s) ", temp);
  fclose(fp);
  fp = NULL;

  free(path);
  path = NULL;

  temp[strlen(temp) - 1] = '\0';

  return temp;
} /* get_name() */


/* Returns the state of a given pid */

char *get_state(char* pid) {
  char * path = malloc(sizeof(char) * SMALL_BUF);
  strcpy(path, "/proc/");
  strcat(path, pid);
  strcat(path, "/stat");

  char * temp = malloc(sizeof(char) * SMALL_BUF);
  FILE *fp = fopen(path, "r");
  fscanf(fp, "%*d %*s %s ", temp);
  fclose(fp);
  fp = NULL;

  free(path);
  path = NULL;

  return temp;
} /* get_state() */


/* Returns the memory used by a given pid */

double get_mem(char* pid) {
  char * path = malloc(sizeof(char) * SMALL_BUF);
  strcpy(path, "/proc/");
  strcat(path, pid);
  strcat(path, "/statm");

  double rss = 0;
  double swap = 0;
  FILE *fp = fopen(path, "r");
  fscanf(fp, "%*s %lf %*s %*s %*s %lf", &rss, &swap);
  fclose(fp);
  fp = NULL;

  free(path);
  path = NULL;

  return (rss + swap);
} /* get_mem() */


/* Function finds and returns the virtual memory utilized by a given process */

double get_virt_mem(char* pid) {
  char * path = malloc(sizeof(char) * SMALL_BUF);
  strcpy(path, "/proc/");
  strcat(path, pid);
  strcat(path, "/statm");

  double virt = 0;
  FILE *fp = fopen(path, "r");
  fscanf(fp, "%lf ", &virt);
  fclose(fp);
  fp = NULL;

  free(path);
  path = NULL;

  return virt;
} /* get_virt_mem() */


/* Function finds and returns the resident memory utilized by a given process */

double get_res_mem(char* pid) {
  char * path = malloc(sizeof(char) * SMALL_BUF);
  strcpy(path, "/proc/");
  strcat(path, pid);
  strcat(path, "/statm");

  double res = 0;
  FILE *fp = fopen(path, "r");
  fscanf(fp, "%*s %lf ", &res);
  fclose(fp);
  fp = NULL;

  free(path);
  path = NULL;

  return res;
} /* get_res_mem() */


/* Function finds and returns the shared memory utilized by a given process */

double get_shared_mem(char* pid) {
  char * path = malloc(sizeof(char) * SMALL_BUF);
  strcpy(path, "/proc/");
  strcat(path, pid);
  strcat(path, "/statm");

  double shared = 0;
  FILE *fp = fopen(path, "r");
  fscanf(fp, "%*s %*s %lf ", &shared);
  fclose(fp);
  fp = NULL;

  free(path);
  path = NULL;

  return shared;
} /* get_shared_mem() */


/* Function returns the current amount of running processes on the system */

int get_proc_num() {
  struct sysinfo info;
  sysinfo(&info);
  return info.procs;
} /* get_proc_num() */


/* Kills a process specified by the given id */

int kill_proc(int pid) {
  if (kill(pid, SIGKILL) == 0) {
    return 0;
  }
  return -1;
} /* kill_proc() */


/* Stops a process specified by the given id */

int stop_proc(int pid) {
  if (kill(pid, SIGSTOP) == 0) {
    return 0;
  }
  return -1;
} /* kill_proc() */


/* Continues a process specified by the given id */

int cont_proc(int pid) {
  if (kill(pid, SIGCONT) == 0) {
    return 0;
  }
  return -1;
} /* kill_proc() */






/* End pid_parser.c */
