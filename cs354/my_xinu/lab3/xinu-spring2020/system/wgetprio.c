/* wgetprio.c - wgetprio */

#include <xinu.h>

/* -------------------------------------------------------------
 * wgetprio - A warpper function for getprio.
 * -------------------------------------------------------------
 */
syscall wgetprio(
      pid32 pid /* The pid whose priority will be returned */
    )
{


  int prio = -1; /* Will eventually contain the return value of getprio */

  asm("movl $5, %%eax\n\t" /* Move the value of getprio sys call num into $eax */
      "movl %1, %%ebx\n\t"
      "int $33\n\t"       /* Make the interrupt call to 33 */
      "movl %%eax, %0" /* Return the value after the interrupt into pid */
      :"=r"(prio)      /* output registers */
      :"r"(pid)  /* input registers */
      :"%eax", "%ebx"         /* Shared registers */
     );


  return prio;

}
