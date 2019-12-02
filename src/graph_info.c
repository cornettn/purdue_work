/* Includes vvv */

#include <stdlib.h>
#include <stdio.h>
#include <sys/utsname.h>
#include <string.h>
#include <sys/statvfs.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <mntent.h>

#include "graph_info.h"

/* Includes ^^^ */


/* Defines vvv */

#define SMALL_BUF (100)
#define BYTE_DIVISOR (1024.0)

/* Defines ^^^ */



/* Struct Fill Functions */

/* Function that fills a struct that gives system CPU usage info */

cpu_hist *get_cur_cpu() {
  cpu_hist *cur = NULL;

  cur->cpu_num = get_cpu_num();
  cur->cpu_use_perc = get_cpu_usage();

  return cur;
} /* get_cur_cpu() */


/* Function returns current memory and swap status in a struct */

ms_hist *get_memswap() {
  ms_hist *cur = malloc(sizeof(ms_hist));

  cur->mem_use = get_graph_info_mem_use();
  cur->mem_total = get_graph_info_mem_total();
  cur->swap_use = get_swap_use();
  cur->swap_total = get_swap_total();

  return cur;
} /* get_memswap() */


/* Function returns current network usage in a struct */

net_hist *get_net() {
  net_hist *cur = malloc(sizeof(net_hist));

  cur->recieving = get_recieving();
  cur->total_recieved = get_total_recieved();
  cur->sending = get_sending();
  cur->total_sent = get_total_sent();

  return cur;
} /* get_net() */


/*
 * Min reqs for Tab 3:
 *   - CPU Usage for each thread/core
 *   - Memory and Swap Usage
 *   - Network Usage (Send/Recieve)
 */


/* Function returns the total amount of cpus on the current system */

int get_cpu_num() {
  return get_nprocs_conf();
} /* get_cpu_num() */


/* Function returns the current percentage of the CPU being used */

double get_cpu_usage() {
  double perc = 0;

  FILE *fp = fopen("/proc/stat", "r");

  if (fp == NULL) {
    perror("get_cpu_usage() could not find proc file");
    return -1;
  }
  double used_start = 0;
  double temp_x = 0;
  double temp_y = 0;
  double idle_start = 0;

  fscanf(fp, "%*s %lf %lf %lf %lf", &used_start, &temp_x, &temp_y, &idle_start);

  used_start += (temp_x + temp_y);
  temp_x = 0;
  temp_y = 0;

  fclose(fp);
  fp = NULL;

  usleep(900);

  fp = fopen("/proc/stat", "r");

  if (fp == NULL) {
    perror("get_cpu_usage() could not find proc file");
    return -1;
  }

  double used_end = 0;
  double idle_end = 0;

  fscanf(fp, "%*s %lf %lf %lf %lf", &used_end, &temp_x, &temp_y, &idle_end);

  used_end += (temp_x + temp_y);

  fclose(fp);
  fp = NULL;


  double used_elapsed = used_end - used_start;
  double idle_elapsed = idle_end - idle_start;

  perc = (used_elapsed / idle_elapsed);

  return perc;
} /* get_cpu_usage() */


/* Function returns the current amount of memory being used on the system */

double get_graph_info_mem_use() {
  struct sysinfo mem;
  if (sysinfo(&mem) == -1) {
    perror("get_mem_use() sysinfo returned an error");
    return -1;
  }
  double total = mem.totalram;
  double free = mem.freeram;
  return total - free;
} /* get_graph_info_mem_use() */


/* Function returns the total amount of memory in the system */

double get_graph_info_mem_total() {
  struct sysinfo mem;
  if (sysinfo(&mem) == -1) {
    perror("get_graph_info_mem_total() sysinfo returned an error");
    return -1;
  }
  double total = mem.totalram;
  return total;
} /* get_graph_info_mem_total() */


/* Function returns the current amount of swap space being used */

double get_swap_use() {
  struct sysinfo mem;
  if (sysinfo(&mem) == -1) {
    perror("get_swap_use() sysinfo returned an error");
    return -1;
  }
  double total = mem.totalswap;
  double free = mem.freeswap;
  return total - free;
} /* get_swap_use() */


/* Function returns the total amount of swap space */

double get_swap_total() {
  struct sysinfo mem;
  if (sysinfo(&mem) == -1) {
    perror("get_swap_total() sysinfo returned an error");
    return -1;
  }
  double total = mem.totalswap;
  return total;
} /* get_swap_total() */



/* Driver function to get total recieved bytes at a certain moment */

double recieve_help() {

  FILE *fp = fopen("/proc/net/dev", "r");

  if (fp == NULL) {
    perror("proc file could  not be opened in get_recieving()");
    return -1;
  }

  double rec = 0;
  double temp = 0;
  char *test = malloc(sizeof(char) * SMALL_BUF);

  fscanf(fp, "%*[^\n]\n");
  fscanf(fp, "%*[^\n]\n");
  while (fscanf(fp, "%s:", test) != EOF) {
    if (strstr(test, "eth") != NULL) {
      fscanf(fp, "\t%lf", &temp);
      rec += temp;
    }
    fscanf(fp, "%*[^\n]\n");
  }

  free(test);
  test = NULL;
  fclose(fp);
  fp = NULL;

  return rec;
} /* recieve_help() */


/* Returns the current bytes being recieved per second */

double get_recieving() {
  double recieved_start = recieve_help();
  usleep(800);
  double recieved_end = recieve_help();
  return recieved_end - recieved_start;
} /* get_recieving() */


/* Returns the total recieved bytes on the network at the current moment */

double get_total_recieved() {
  return recieve_help();
} /* get_total_recieved() */


/* Driver function to get total sent bytes at a certain moment */

double send_help() {

  FILE *fp = fopen("/proc/net/dev", "r");

  if (fp == NULL) {
    perror("proc file could  not be opened in get_recieving()");
    return -1;
  }

  double rec = 0;
  double temp = 0;
  char *test = malloc(sizeof(char) * SMALL_BUF);

  fscanf(fp, "%*[^\n]\n");
  fscanf(fp, "%*[^\n]\n");
  while (fscanf(fp, "%s:", test) != EOF) {
    if (strstr(test, "eth") != NULL) {
      for (int x = 0; x < 9; x++) {
        fscanf(fp, "\t%lf", &temp);
      }
      rec += temp;
    }
    fscanf(fp, "%*[^\n]");
  }

  free(test);
  test = NULL;
  fclose(fp);
  fp = NULL;

  return rec;
} /* send_help() */


/* Function returns current amount of bytes being sent on a network */

double get_sending() {
  double send_start = send_help();
  usleep(800);
  double send_end = send_help();
  return send_end - send_start;
} /* get_sending() */


/* Function returns the total amount of bytes sent on a network */

double get_total_sent() {
  return send_help();
} /* get_total_sent() */


