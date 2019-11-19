

#ifndef SYS_INFO_H
#define SYS_INFO_H

int init_sys_info();

char *get_OS_version();
char *get_kernel_version();
char *get_OS_name();
char *get_mem_total();
char *get_CPU_info();
double get_disk_storage();

typedef struct sys_info {
  char *OS_version;
  char *kernel_version;
  char *OS_name;
  char *mem_total;
  char *CPU_info;
  double disk_storage;

}sys_info;

void fill_info(sys_info *);

#endif 
