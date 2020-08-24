/* resched.c - resched, resched_cntl */

#include <xinu.h>

struct	defer	Defer;

/*------------------------------------------------------------------------
 *  resched  -  Reschedule processor to highest priority eligible process
 *------------------------------------------------------------------------
 */
void	resched(void)		/* Assumes interrupts are disabled	*/
{
	struct procent *ptold;	/* Ptr to table entry for old process	*/
	struct procent *ptnew;	/* Ptr to table entry for new process	*/


    /* If rescheduling is deferred, record attempt and return */

	if (Defer.ndefers > 0) {
		Defer.attempt = TRUE;
		return;
	}

	/* Point to process table entry for the current (old) process */

	ptold = &proctab[currpid];

	if (ptold->prstate == PR_CURR) {  /* Process remains eligible */

    /* Lab 3 - 3/4/20
     * Change the comparator to accomodate for non-decreasing sched.
     * Comment out this comparison because priority is now dependent on
     * the prvgrosscpu value, and this value is not updated until after
     * ctxsw() returns, so this if statement would prevent the prvgrosscpu
     * from ever updating */

    //  if (ptold->prvgrosscpu < firstkey(readylist)) {
    //    return;
	  //	}

		/* Old process will no longer remain current */

		ptold->prstate = PR_READY;

    /* Lab 3, Part 4
     * - use rinsert instead of insert
     * - Additionally, use the prvgrosscpu value as the priority in order to implement
     * a form of far scheduling. */

    rinsert(currpid, readylist, ptold->prvgrosscpu);


	}

	/* Force context switch to highest priority ready process */


	currpid = dequeue(readylist);
	ptnew = &proctab[currpid];
	ptnew->prstate = PR_CURR;
	preempt = QUANTUM;		/* Reset time slice for process	*/



  /* Millisecond counter */

  uint32 beforecputime = clktimemilli;

  /* CPU clock ticks counter */

  uint64 beforecputick;
  asm volatile ( "rdtsc" : "=A"(beforecputick) );

  ctxsw(&ptold->prstkptr, &ptnew->prstkptr);


	/* Old process returns here when resumed */

  /* Lab 3 - 3/3/20 */
  /* - Update the prgrosscpu accordingly
   * - Update the prvgrosscpu value as well to implement fair sched.
   * */

  if (clktimemilli == beforecputime) {
    ptnew->prgrosscpu++;
    ptnew->prvgrosscpu++;
  }
  else {
    ptnew->prgrosscpu += clktimemilli - beforecputime;
//    ptnew->prvgrosscpu += clktimemilli - beforecputime;
  }

  /* Updated the prgrosscputick accordingly */

  uint64 aftercputick;
  asm volatile ( "rdtsc" : "=A"(aftercputick) );

  ptnew->prgrosscputick += aftercputick - beforecputick;


  /* Lab 3, Part 4
   * - Ensure that the NULLPROC always has the lowest priority
   *   (highest prvgrosscpu value). Update the ready list to reflect
   *   change as well. */

  if (ptnew->prvgrosscpu >= proctab[NULLPROC].prvgrosscpu) {
    proctab[NULLPROC].prvgrosscpu = ptnew->prvgrosscpu + 1;
    if (lastid(readylist) == NULLPROC) {
      lastkey(readylist) = proctab[NULLPROC].prvgrosscpu;
    }
  }
    /* Really ensure that NULLPROC is last */

    if ((currpid != NULLPROC) &&
        (ptnew->prvgrosscpu >= queuetab[NULLPROC].qkey)) {
      queuetab[NULLPROC].qkey = ptnew->prvgrosscpu + 1;
      proctab[NULLPROC].prvgrosscpu = queuetab[NULLPROC].qkey;

    if (queuetab[NULLPROC].qnext != queuetail(readylist)) {

      /* If Null proc is not already last,
      * Ensure that the NULLPROC is the last entry in the ready list */

      XDEBUG_KPRINTF("Rearranging ready list\n");

      qid16 nprev = queuetab[NULLPROC].qprev;
      qid16 nnext = queuetab[NULLPROC].qnext;

      queuetab[NULLPROC].qprev = queuetab[queuetail(readylist)].qprev;
      queuetab[queuetab[queuetail(readylist)].qprev].qnext = NULLPROC;

      queuetab[NULLPROC].qnext = queuetail(readylist);
      queuetab[queuetail(readylist)].qprev = NULLPROC;

      queuetab[nprev].qnext = nnext;
      queuetab[nnext].qprev = nprev;
    }
  }

	return;
}

/*------------------------------------------------------------------------
 *  resched_cntl  -  Control whether rescheduling is deferred or allowed
 *------------------------------------------------------------------------
 */
status	resched_cntl(		/* Assumes interrupts are disabled	*/
	  int32	defer		/* Either DEFER_START or DEFER_STOP	*/
	)
{
	switch (defer) {

	    case DEFER_START:	/* Handle a deferral request */

		if (Defer.ndefers++ == 0) {
			Defer.attempt = FALSE;
		}
		return OK;

	    case DEFER_STOP:	/* Handle end of deferral */
		if (Defer.ndefers <= 0) {
			return SYSERR;
		}
		if ( (--Defer.ndefers == 0) && Defer.attempt ) {
			resched();
		}
		return OK;

	    default:
		return SYSERR;
	}
}
