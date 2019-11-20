
#include <stdlib.h>
#include <stdio.h>
#include <mntent.h>

#include "sys_info.h"
#include "file_system.h"


int main() {

  /*
  int err = init_sys_info();

  if (err != 0) {
    printf("Its fucked");
    exit(0);
  }
  */

  FILE *fp = setmntent(_PATH_MOUNTED, "r");
  struct mntent *mnt = getmntent(fp);

  double version = get_dev_total_space(mnt);

  printf("\n%f\n\n", version);
  //free(version);
}
