/* wgetppid.c - wgetppid */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  wgetppid  -  Wrapper function for getppid()
 *------------------------------------------------------------------------
 */
syscall wgetppid(
      pid32 pid /* The pid whose parent pid will be returned */
    )
{

  int ppid = -1; /* This will eventually contain the return value of getppid */

  asm("movl $6, %%eax\n\t" /* Move the value of the getppid sys call into $eax */
      "movl %1, %%ebx\n\t"
      "int $33\n\t"       /* Make the interrupt call to 33 */
      "movl %%eax, %0" /* Return the value after the interrupt into pid */
      :"=r"(ppid)      /* output registers */
      :"r"(pid)  /* input registers */
      :"%eax", "%ebx"         /* Shared registers */
     );


  return ppid;

}
