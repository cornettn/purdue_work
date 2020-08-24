/* bsend.c - bsend */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  bsend  -  Pass a message to a process and start recipient if waiting
 *------------------------------------------------------------------------
 */
syscall	bsend(
	  pid32		pid,		/* ID of recipient process	*/
	  umsg32	msg		/* Contents of message		*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/
  struct  procent *currprptr; /* Ptr to curr proc */

	mask = disable();
	if (isbadpid(pid)) {
		restore(mask);
		return SYSERR;
	}

	prptr = &proctab[pid];
  currprptr = &proctab[getpid()];


  if (prptr->prhasmsg == TRUE) {
    /* Receivers buffer is full */

    /* Change the state to PR_SND from current */
    currprptr->prstate = PR_SND;

    /* Enqueue sending proc into the receivers queue */
    prptr->prsenderflag = TRUE;
    enqueue(getpid(), prptr->prblockedsenders);

    /* Block until the message can be sent */
    resched();
  }

  /* Receivers buffer is empty */
	prptr->prmsg = msg;		/* Deliver message		*/
	prptr->prhasmsg = TRUE;		/* Indicate message is waiting	*/

	/* If recipient waiting or in timed-wait make it ready */

	if (prptr->prstate == PR_RECV) {
		ready(pid);
	} else if (prptr->prstate == PR_RECTIM) {
		unsleep(pid);
		ready(pid);
	}
	restore(mask);		/* Restore interrupts */
	return OK;
}
