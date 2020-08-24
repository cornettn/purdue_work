/* procgrosscpu.c - procgrosscpu */

#include <xinu.h>

/*-------------------------------------------------------------
 * procgrosscpu - Returns a the given processes gross cpu time.
 *-------------------------------------------------------------
 */

uint32 procgrosscpu(
    pid32 pid /* The process whose gross cpu time will be returned */
    )
{

  intmask mask; /* Interrupt mask */
  mask = disable();

  if (pid < 0) {
    restore(mask);
    return SYSERR;
  }

  uint32 grosscputime = proctab[pid].prgrosscpu;

  restore(mask);
  return grosscputime;
}
