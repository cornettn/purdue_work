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
  char *virt_memory;
  char *res_memory;       // The memory being used by the process
  char *shared_mem;   // The shared memory of the process
  char *cpu_time;     // The CPU time that the process gets
  char *time_started; // The time that the prcoess time started
} process_t;

/* Initialization */



/* Functions */



#endif
