/* gettmslice.c - gettmslice */

#include <xinu.h>

/* --------------------------------------------------------------------------
 * gettmslice - This function will return how much time is left its time slice
 * --------------------------------------------------------------------------
 */
uint32 gettmslice(void) {

  intmask mask;
  mask = disable();

  return preempt;

  restore(mask);
}
