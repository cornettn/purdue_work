
#include <stdlib.h>
#include <stdio.h>
#include <mntent.h>

#include "sys_info.h"
#include "file_system.h"




void print_mounts(mount **list) {
  if (list == NULL) {
    return;
  }

  for (int x = 0; x < get_mount_num(); x++) {
    printf("Dev Name: %s\nDev Dir: %s\nDev Type: %s\nDev Total Space: %f\nDev Free Space: %f\nDev Avail Space: %f\nDev Used Space: %f\n",
            list[x]->dev_name, list[x]->dev_type, list[x]->dev_type,
            list[x]->dev_total_space, list[x]->dev_free_space, 
            list[x]->dev_avail_space, list[x]->dev_used_space);
  }


}



int main() {

  /*
  int err = init_sys_info();

  if (err != 0) {
    printf("Its fucked");
    exit(0);
  }
  */

 // FILE *fp = setmntent(_PATH_MOUNTED, "r");
  //struct mntent *mnt = getmntent(fp);

  //double version = get_dev_total_space(mnt);

  //mount **list = get_mount_list();

  //print_mounts(list);

  //printf("\n%f\n\n", version);
  //free(version);
}





