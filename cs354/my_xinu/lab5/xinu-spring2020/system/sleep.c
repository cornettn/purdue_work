/* sleep.c - sleep sleepms */

#include <xinu.h>

#define	MAXSECONDS	2147483		/* Max seconds per 32-bit msec	*/

/*------------------------------------------------------------------------
 *  sleep  -  Delay the calling process n seconds
 *------------------------------------------------------------------------
 */
syscall	sleep(
	  int32	delay		/* Time to delay in seconds	*/
	)
{
	if ( (delay < 0) || (delay > MAXSECONDS) ) {
		return SYSERR;
	}
	sleepms(1000*delay);
	return OK;
}

/*------------------------------------------------------------------------
 *  sleepms  -  Delay the calling process n milliseconds
 *------------------------------------------------------------------------
 */
syscall	sleepms(
	  int32	delay			/* Time to delay in msec.	*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/

	if (delay < 0) {
		return SYSERR;
	}

	if (delay == 0) {
		yield();
		return OK;
	}

	/* Delay calling process */

	mask = disable();
	if (insertd(currpid, sleepq, delay) == SYSERR) {
		restore(mask);
		return SYSERR;
	}

	proctab[currpid].prstate = PR_SLEEP;

  /* Sleep the process */

  resched();

  /* Process is awake now */

  void (*cbfunction)();
  struct procent *prptr = &proctab[currpid];
  if (prptr->prcbvalid) {
    /* Process has a callback function */

    if (prptr->prtmpvalid) {
      /* Process has a valid message */

      prptr->prtmpvalid = FALSE;
      *prptr->prmbufptr = prptr->prtmpbuf;

      cbfunction = prptr->prcbptr;
      (*cbfunction)();

    }
  }

  restore(mask);

  // Call the function in user context
	return OK;
}
