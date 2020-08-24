/*  main.c  - main */

#include <xinu.h>

#define UNUSED(x) (void)(x)


void sndch(char);

process	main(void)
{

  int prio = (int) wgetprio(2);
  pid32 pid =  wgetpid();
  pid32 ppid = wgetppid(2);


  XDEBUG_KPRINTF("wgetprio(2) returned %d\n", (int) prio);
  XDEBUG_KPRINTF("getprio(2) returns %d\n", (int) getprio(2));
  XDEBUG_KPRINTF("wgetpid() returned %d\n", (int) pid);
  XDEBUG_KPRINTF("getpid returns %d\n", (int) getpid());
  XDEBUG_KPRINTF("wgetppid(2) returned %d\n", (int) ppid);
  XDEBUG_KPRINTF("getppid(2) returns %d\n", (int) getppid(2));

  //rcreate(sndch, 1024, 20, "send A", 1, 'a');
  //rcreate(sndch, 1024, 20, "send B", 1, 'b');
}

void sndch(char ch) {
  while(1) {
    kprintf("%c", ch);
  }
}
