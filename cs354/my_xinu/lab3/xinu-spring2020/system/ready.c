/* ready.c - ready */

#include <xinu.h>

qid16	readylist;			/* Index of ready list		*/

/*------------------------------------------------------------------------
 *  ready  -  Make a process eligible for CPU service
 *------------------------------------------------------------------------
 */
status	ready(
	  pid32		pid		/* ID of process to make ready	*/
	)
{
	register struct procent *prptr;

	if (isbadpid(pid)) {
		return SYSERR;
	}

	/* Set process state to indicate ready and add to ready list */


	prptr = &proctab[pid];


  /* Lab 3 Part 4
   * - If the newly ready process was just asleep, assign the priority to
   *   be the highest (lowest prvgrosscpu value in the ready list)
   */

  if (prptr->prstate == PR_SLEEP) {
    if (currpid == NULLPROC) {

      /* This implies that the ready list is empty because the
       * NULLPROC is running */

      prptr->prvgrosscpu = proctab[NULLPROC].prvgrosscpu++;
    }
    else {

      /* Assign the prvgrossvalue to be the lowest value in the ready list */

      prptr->prvgrosscpu = firstkey(readylist);
      if (firstid(readylist) == NULLPROC) {

        /* Ensure that NULLPROC remains the lowest prio */

        firstkey(readylist)++;
        proctab[NULLPROC].prvgrosscpu++;
      }
    }

    /* Really ensure that NULLPROC is last */

    if ((currpid != NULLPROC) &&
        (prptr->prvgrosscpu >= queuetab[NULLPROC].qkey)) {
      queuetab[NULLPROC].qkey = prptr->prvgrosscpu + 1;
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

  }

  prptr->prstate = PR_READY;

  /* Lab 3 - Use rinsert instead of insert to accomodate for
   *         non-decreasing shed. */

  rinsert(pid, readylist, prptr->prvgrosscpu);
	resched();

	return OK;
}
