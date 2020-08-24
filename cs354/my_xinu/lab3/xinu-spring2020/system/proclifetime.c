/* proclifetime.c - proclifetime */

#include <xinu.h>

/*----------------------------------------------------------
 * proclifetime - Returns the lifetime of the given process.
 *----------------------------------------------------------
 */

uint32 proclifetime(
    pid32 pid  /* The pid whose lifetime will be returned */
    )
{

  intmask mask; /* interrupts mask */
  mask = disable();

  uint32 lifetime = clktimemilli - proctab[pid].prbirth;

  restore(mask);
  return lifetime;
}
