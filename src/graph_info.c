
/* Includes vvv */

#include <stdlib.h>
#include <stdio.h>
#include <sys/utsname.h>
#include <string.h>
#include <sys/statvfs.h>
#include <sys/sysinfo.h>
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
  net_hist *cur = NULL;

  // ... Fill

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
  struct sysinfo *mem = NULL;
  if (sysinfo(mem) == -1) {
    perror("get_mem_use() sysinfo returned an error");
    return -1;
  }
  double total = mem->totalram;
  double free = mem->freeram;
  return total - free;
}

double get_graph_info_mem_total() {
//#include <slope.h>
  struct sysinfo *mem = NULL;
  sysinfo(mem);
  double total = mem->totalram;
  return total;
}

double get_swap_use() {
  struct sysinfo *mem = NULL;
  sysinfo(mem);
  double total = mem->totalswap;
  double free = mem->freeswap;
  return total - free;
}

double get_swap_total() {
  struct sysinfo *mem = NULL;
  sysinfo(mem);
  double total = mem->totalswap;
  return total;
}



double get_recieving() {

  FILE *fp = fopen("/proc/net/dev", "r");

  if (fp == NULL) {
    return -1;
  }


  return 0;
}

double get_total_recieved() {
  return 0;
}

double get_sending() {
  return 0;
}

double get_total_sent() {
  return 0;
}


