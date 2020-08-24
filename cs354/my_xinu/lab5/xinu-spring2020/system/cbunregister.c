/* cbunregister.c - cbunregister  */


#include <xinu.h>

/* ------------------------------------------------------------------
 * sbunregister - used to unregister the currprocs callback function
 * ------------------------------------------------------------------
 */

#define UNREGISTERED (0)

syscall cbunregister(void) {

  struct procent *prptr = &proctab[getpid()];


  if (prptr->prcbvalid == UNREGISTERED) {
    /* There is no cb function registered */
    return SYSERR;
  }

  prptr->prcbvalid == UNREGISTERED;
  return OK;
}
