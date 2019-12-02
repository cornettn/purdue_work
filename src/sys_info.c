
/* Includes vvv */

#include <stdlib.h>
#include <stdio.h>
#include <sys/utsname.h>
#include <string.h>
#include <sys/statvfs.h>
#include <mntent.h>

#include "sys_info.h"

/* Includes ^^^ */


/* Defines vvv */

#define SMALL_BUF (100)
#define BYTE_DIVISOR (1024.0)

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
} /* init_sys_info() */



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
} /* get_OS_version() */


/* Function for finding kernal version */

char *get_kernel_version() {
  return info.version;
} /* get_kernal_version() */


/* Function for getting OS name */

char *get_OS_name() {
  return info.sysname;
} /* get_OS_name() */


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
} /* get_mem_total() */


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
} /* get_CPU_info() */


/* Function for get the amount of space available in the file system, returns -1 on error */

double get_disk_storage() {
  struct mntent *mnt;
  FILE * fp = setmntent(_PATH_MOUNTED, "r");
  struct statvfs disk_amount;
  if (fp == NULL) {
    perror("get_disk_storage() file pointer found NULL");
    return -1;
  }
  while ((mnt = getmntent(fp)) != NULL) {
    if (strcmp(mnt->mnt_fsname,"udev") == 0) {
      if (statvfs(mnt->mnt_dir, &disk_amount) != 0) {
        perror("get_disk_storage() the statvfs() function returned an error");
        endmntent(fp);
        return -1;
      }
      else {
        double size = (disk_amount.f_bfree) * (disk_amount.f_bsize);
        size = size / BYTE_DIVISOR / BYTE_DIVISOR / BYTE_DIVISOR;
        endmntent(fp);
        return size;
      }
    }
  }
  perror("get_disk_storage could not find root");
  endmntent(fp);
  return -1;
} /* get_disk_storage() */


/* Function for filling the info struct */

void fill_info(sys_info *fill) {
  fill->OS_version = get_OS_name();
  fill->kernel_version = get_kernel_version();
  fill->OS_name = get_OS_name();
  fill->mem_total = get_mem_total();
  fill->CPU_info = get_CPU_info();
  fill->disk_storage = get_disk_storage();
}




