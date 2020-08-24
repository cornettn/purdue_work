/* procbirth.c - procbirth */

#include <xinu.h>

/*--------------------------------------------------------
 * procbirth - Returned the birthday of the given process
 *--------------------------------------------------------
 */

uint32 procbirth(
    pid32 pid /* The process whose birthday will be returned */
    )
{

  intmask mask; /* interrupt mask */
  mask = disable();

  if (pid <= 0) {
    restore(mask);
    return SYSERR;
  }

  uint32 birthday = proctab[pid].prbirth;

  restore(mask);
  return birthday;
}
