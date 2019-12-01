
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


/* Global Variables vvv */



/* Struct Fill Functions */


cpu_hist *get_cur_cpu() {
  cpu_hist *cur = NULL;

  // ... Fill
  // cur->cpu_num = get_cpu_num();


  return cur;
}


/* Function returns current memory and swap status in a struct */

ms_hist *get_memswap() {
  ms_hist *cur = malloc(sizeof(ms_hist));

  cur->mem_use = get_graph_info_mem_use();
  cur->mem_total = get_graph_info_mem_total();
  cur->swap_use = get_swap_use();
  cur->swap_total = get_swap_total();

  return cur;
}


net_hist *get_net() {
  net_hist *cur = malloc(sizeof(net_hist));

  cur->recieving = get_recieving();
  cur->total_recieved = get_total_recieved();
  cur->sending = get_sending();
  cur->total_sent = get_total_sent();

  return cur;
}


/*
 * Min reqs for Tab 3:
 *   - CPU Usage for each thread/core
 *   - Memory and Swap Usage
 *   - Network Usage (Send/Recieve)
 */


int get_cpu_num() {
  return get_nprocs_conf();
}

double *get_cpu_use_list() {
  double * temp = NULL;

  return temp;
}



double get_graph_info_mem_use() {
  struct sysinfo mem;
  if (sysinfo(&mem) == -1) {
    perror("get_mem_use() sysinfo returned an error");
    return -1;
  }
  double total = mem.totalram;
  double free = mem.freeram;
  return total - free;
}

double get_graph_info_mem_total() {
  struct sysinfo mem;
  sysinfo(&mem);
  double total = mem.totalram;
  return total;
}

double get_swap_use() {
  struct sysinfo mem;
  sysinfo(&mem);
  double total = mem.totalswap;
  double free = mem.freeswap;
  return total - free;
}

double get_swap_total() {
  struct sysinfo mem;
  sysinfo(&mem);
  double total = mem.totalswap;
  return total;
}



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
  fclose(fp);
  fp = NULL;

  return rec;
}


/* Returns the current bytes being recieved per second */

double get_recieving() {
  double recieved_start = recieve_help();
  usleep(800);
  double recieved_end = recieve_help();
  return recieved_end - recieved_start;
}


/* Returns the total recieved bytes on the network at the current moment */

double get_total_recieved() {
  return recieve_help();
}


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
  fclose(fp);
  fp = NULL;

  return rec;
}


double get_sending() {
  double send_start = send_help();
  usleep(800);
  double send_end = send_help();
  return send_end - send_start;
}

double get_total_sent() {
  return send_help();
}


