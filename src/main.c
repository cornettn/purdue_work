
#include <stdlib.h>
#include <stdio.h>

#include "sys_info.h"



int main() {

  int err = init_sys_info();

  if (err != 0) {
    printf("Its fucked");
    exit(0);
  }


  double version = get_disk_storage();

  printf("\n%f\n\n", version);
  //free(version);
}
