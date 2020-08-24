#include <xinu.h>

void testio() {
  int x, y, i;
  x = 0;
  y = clktimemilli;
  while(clktimemilli - y < 8000) {
    x++;
    for (i = 0; i < 10000; i++) ;
    sleepms(50);
  }
  kprintf("io: %d\t%d\t%d\t%d\n", currpid, x,
      proctab[currpid].prgrosscpu,
      procgrosscpumicro(currpid));
}
