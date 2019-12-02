#ifndef PID_PARSER_H
#define PID_PARSER_H

typedef struct {
  int pid;            // The actual pid of the process X
  char *proc_name;    // The name of the process X
  char *mem_maps;     // The memory maps of the process
  char *open_files;   // The open files of the process
  char *user;         // The user associated with the process
  char *state;        // The state that the process is in X
  int cpu_perc;
  double memory;
  double virt_memory;
  double res_memory;       // The memory being used by the process
  double shared_mem;   // The shared memory of the process
  double cpu_time;     // The CPU time that the process gets
  char *time_started; // The time that the prcoess time started
} process_t;

/* Initialization */


process_t **create_pid_list();

int get_proc_num();
int kill_proc(int);
int stop_proc(int);
int cont_proc(int);


char *get_name(char *);
char *get_state(char *);
double get_mem(char *);
double get_virt_mem(char *);
double get_res_mem(char *);
double get_shared_mem(char *);
double get_cpu_time(char *);
char *get_time_started(char *);

/* Functions */



#endif
