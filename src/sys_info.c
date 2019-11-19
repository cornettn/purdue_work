
/* Includes vvv */

#include <stdio.h>
#include <sys/utsname.h>

/* Includes ^^^ */ 


/* Defines vvv */

/* Defines ^^^ */


/* Global Variables vvv */

struct utsname info;


/* Initializes global variables, returns 0 on succes, -1 on error. */

int init_sys_info () {
  int err = 0;
  err = uname(&info);
  if (err == -1) {
    printf("Fuck fuck fuck");
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

char *get_kernal_version() {
  return info.version;
}


/* Function for getting OS name */

char *get_OS_name() {
  return info.sysname;
}





