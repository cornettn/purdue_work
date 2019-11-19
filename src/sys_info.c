
/* Includes vvv */

#include <stdlib.h>
#include <stdio.h>
#include <sys/utsname.h>
#include <string.h>
#include <sys/statvfs.h>

/* Includes ^^^ */ 


/* Defines vvv */

#define SMALL_BUF (100)

/* Defines ^^^ */


/* Global Variables vvv */

struct utsname info;


/* Initializes global variables, returns 0 on succes, -1 on error. */

int init_sys_info () {
  int err = 0;
  err = uname(&info);
  if (err == -1) {
    perror("uname() function returned an error in init_sys_info()");
    return -1;
 }
  return 0;
}



/*
 * Min reqs for Tab 1:
 *   - OS Release Version
 *   - Kernal Version
 *   - Amount of memory
 *   - Processor Version
 *   - Disk Storage
 */


/* Function for finding OS Release Version */

char *get_OS_version() {
  return info.release;
}


/* Function for finding kernal version */

char *get_kernel_version() {
  return info.version;
}


/* Function for getting OS name */

char *get_OS_name() {
  return info.sysname;
}


/* Function for getting Total Memory, returns NULL on error, what is returned must be freed */

char *get_mem_total() {
  FILE *fp = fopen("/proc/meminfo", "r");
  if (fp) {
    char *memstr = malloc(sizeof(char) * SMALL_BUF);
    fscanf(fp, "MemTotal:\t%s", memstr);
    fclose(fp);
    strcat(memstr, " Kb");
    return memstr;
  }
  perror("Couldn't Open File in get_mem_total()");
  return NULL;
}


/* Function for getting CPU info, returns NULL on error, what is returned must be freed */

char *get_CPU_info() {
  FILE *fp = fopen("/proc/cpuinfo", "r");
  if (fp) {
    char *cpustr = malloc(sizeof(char) * SMALL_BUF);
    while (fscanf(fp, "%[^\n]\n", cpustr) != EOF) {
      if (strstr(cpustr, "model name") != NULL) {
        strcpy(cpustr, strchr(cpustr, ':') + 2);
        fclose(fp);
        return cpustr;
      }
    }
    fclose(fp);
    perror("Couldn't find information in get_CPU_info");
    return NULL;
  }
  perror("Couldn't Open file in get_CPU_info");
  return NULL;
}


/* Function for get the amount of space available in the file system, returns -1 on error */

long get_disk_storage() {
  struct statvfs *stat = NULL;
  int err = statvfs("/proc", stat);
  if (err != 0) {
    perror("get_disk_storage() the statvfs() function returned an error");
    return -1;
  }
  printf("Test");
  return (long)stat->f_bfree;
}




