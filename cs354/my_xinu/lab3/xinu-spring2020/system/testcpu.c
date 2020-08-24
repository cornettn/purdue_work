#include <xinu.h>

void testcpu() {
  int x, y;
  x = 0;
  y = clktimemilli;
  while(clktimemilli - y < 8000) {
    x++;
  }

  kprintf("cpu: %d\t%d\t%d\t%d\n", currpid, x,
      proctab[currpid].prgrosscpu,
      procgrosscpumicro(currpid));
}
