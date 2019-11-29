
#ifndef GRAPH_INFO_H
#define GRAPH_INFO_H


int get_cpu_num();
double *get_cpu_use_list();

double get_mem_use();
double get_mem_total();
double get_swap_use();
double get_swap_total();

double get_recieving();
double get_total_recieved();
double get_sending();
double get_total_sent();


typedef struct cpu_hist {
  int cpu_num;
  double *cpu_use_list;
}cpu_hist;

typedef struct mem_and_swap_hist {
  double mem_use;
  double mem_total;
  double swap_use;
  double swap_total;
}ms_hist;

typedef struct net_hist {
  double recieving;
  double total_recieved;
  double sending;
  double total_sent;
}net_hist;  

cpu_hist *get_cur_cpu();
ms_hist *get_memswap();
net_hist *get_net();

#endif 
