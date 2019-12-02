/* Includes vvv */

#include <stdlib.h>
#include <stdio.h>
#include <sys/utsname.h>
#include <string.h>
#include <sys/statvfs.h>
#include <mntent.h>

#include "file_system.h"

/* Includes ^^^ */


/* Defines vvv */

#define SMALL_BUF (100)
#define BYTE_DIVISOR (1024.0)

/* Defines ^^^ */



/* Creates and fills a mount list for the cur sys,
 * returns mount list on succes, NULL on error. */

mount **get_mount_list() {

  int mount_count = get_mount_num();
  mount **list = malloc(sizeof(mount*) * mount_count);

  FILE *fp;
  struct mntent * mnt;

  fp = setmntent(_PATH_MOUNTED, "r");

  if (fp == NULL) {
    perror("get_mount_list() could not find a fp");
    return NULL;
  }

  for (int x = 0; x < mount_count; x++) {
    mnt = getmntent(fp);
    if (mnt != NULL) {
      list[x] = malloc(sizeof(mount));
      list[x]->dev_name = get_dev_name(mnt);
      list[x]->dev_dir = get_dev_dir(mnt);
      list[x]->dev_type = get_dev_type(mnt);
      list[x]->dev_total_space = get_dev_total_space(mnt);
      list[x]->dev_free_space = get_dev_free_space(mnt);
      list[x]->dev_avail_space = get_dev_avail_space(mnt);
      list[x]->dev_used_space = get_dev_used_space(mnt);
    }
  }
  endmntent(fp);
  return list;
} /* get_mount_num() */


/* Finds the number of mount on the current system, returns -1 on err */

int get_mount_num() {
  FILE *fp;
  struct mntent * mnt;

  fp = setmntent(_PATH_MOUNTED, "r");
  if (fp == NULL) {
    perror("get_mount_num() could not read file");
    return -1;
  }
  int count = 0;
  while ((mnt = getmntent(fp)) != NULL) {
    count++;
  }
  endmntent(fp);
  return count;
} /* get_mount_num() */


/*
 * Min reqs for Tab 4:
 *   - Device Name
 *   - Device Dir
 *   - Device Type
 *   - Device Total Space
 *   - Device Avail Space
 *   - Device Used Space
 */


/* Function that returns the device name of the given mount */

char *get_dev_name(struct mntent * mnt) {
  return mnt->mnt_fsname;
} /* get_dev_name() */


/* Function that returns the device dir of the given mount */
char *get_dev_dir(struct mntent * mnt) {
  return mnt->mnt_dir;
} /* get_dev_dir() */


/* Function that returns the device type of the given mount */
char *get_dev_type(struct mntent *mnt) {
  return mnt->mnt_type;
} /* get_dev_type() */


/* Function that returns the total space of the given mount, and -1 on failure */
double get_dev_total_space(struct mntent * mnt) {
  struct statvfs buf;

  if (statvfs(mnt->mnt_dir, &buf) != 0) {
    perror("get_dev_total_space() had an error with statvfs()");
    return -1;
  }

  double size = (((double)buf.f_blocks) * ((double)(buf.f_frsize)));
  size = size / BYTE_DIVISOR / BYTE_DIVISOR / BYTE_DIVISOR;
  return size;
} /* get_dev_total_space() */


/* Function that returns the total free space in a mount given, and -1 on failure */

double get_dev_free_space(struct mntent *mnt) {
  struct statvfs buf;

  if (statvfs(mnt->mnt_dir, &buf) != 0) {
    perror("get_dev_free_space() had an error with statvfs()");
    return -1;
  }

  double size = (((double)buf.f_bfree) * ((double)(buf.f_bsize)));
  size = size / BYTE_DIVISOR / BYTE_DIVISOR / BYTE_DIVISOR;
  return size;
} /* get_dev_free_space() */


/* Function that returns the total avail space in the given mount, and -1 on failure */

double get_dev_avail_space(struct mntent *mnt) {
  struct statvfs buf;

  if (statvfs(mnt->mnt_dir, &buf) != 0) {
    perror("get_dev_avail_space() had an error with statvfs()");
    return -1;
  }

  double size = (((double)buf.f_bavail) * ((double)(buf.f_bsize)));
  size = size / BYTE_DIVISOR / BYTE_DIVISOR / BYTE_DIVISOR;
  return size;
} /* get_dev_avail_space() */


/* Function that returns the used space in a given mound */

double get_dev_used_space(struct mntent *mnt) {
  return get_dev_total_space(mnt) - get_dev_free_space(mnt);
} /* get_dev_used_space() */



