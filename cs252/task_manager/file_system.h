
#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <mntent.h>

char *get_dev_name(struct mntent *);
char *get_dev_dir(struct mntent *);
char *get_dev_type(struct mntent *);
double get_dev_total_space(struct mntent *);
double get_dev_free_space(struct mntent *);
double get_dev_avail_space(struct mntent *);
double get_dev_used_space(struct mntent *);

typedef struct mount {
  char *dev_name;
  char *dev_dir;
  char *dev_type;
  double dev_total_space;
  double dev_free_space;  // Designates total free space
  double dev_avail_space; // Designates free space for unprivileged users
  double dev_used_space;
}mount;

mount **get_mount_list();
int get_mount_num();

#endif
