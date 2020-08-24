/* procgrosspumicro.c - procgrosspumincro */

#include <xinu.h>

#define NORMALIZATION (2.5)
#define MICRO_FACTOR (1000)


/*-----------------------------------------------------------------------------
 * procgrosspumicro - returns the value of the given processes gross cpu ticks
 *                      in a unit of microseconds.
 *-----------------------------------------------------------------------------
 */

uint32 procgrosscpumicro(
    pid32 pid /* process id to find the gross cpu ticks of */
    )
{

  intmask mask; /* iterrupt mask */
  mask = disable();

  uint64 cputicks = proctab[pid].prgrosscputick;

  //uint64 nanoseconds = (uint64) (cputicks * NORMALIZATION);
  //uint32 microseconds = nanoseconds / MICRO_FACTOR;

  restore(mask);
  return cputicks * NORMALIZATION / MICRO_FACTOR;

}
