/* wgetpid.c - wgetpid */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  wgetpid  -  Wrapper function for getpid()
 *------------------------------------------------------------------------
 */
pid32 wgetpid() {


  int func_num = 4; /* The syscall number for getpid() */
  int pid = -1; /* This will eventually contain the return value of getpid */


  asm("movl %1, %%eax\n\t"
      "int $33\n\t"
      "movl %%eax, %0"
      :"=r"(pid)      /* output registers */
      :"r"(func_num)  /* input registers */
      :"%eax"         /* Shared registers */
     );

  return pid;

}
