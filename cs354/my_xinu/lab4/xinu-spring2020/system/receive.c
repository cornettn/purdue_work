/* receive.c - receive */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  receive  -  Wait for a message and return the message to the caller
 *------------------------------------------------------------------------
 */
umsg32	receive(void)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/
	umsg32	msg;			/* Message to return		*/
  struct procent *sndprptr;    /* Ptr to the next sender proc */
  pid32 sndproc;    /* pid of the next sender proc */

	mask = disable();
	prptr = &proctab[currpid];
	if (prptr->prhasmsg == FALSE) {
    prptr->prstate = PR_RECV;
		resched();		/* Block until message arrives	*/
	}

	msg = prptr->prmsg;		/* Retrieve message		*/

  if (prptr->prsenderflag == TRUE) {
    /* Another process has a message to be sent to the curr proc */

    /* Get the process who is waiting to send msg */
    sndproc = dequeue(prptr->prblockedsenders);
    sndprptr = &proctab[sndproc];

    /* Send the message */
    prptr->prmsg = sndprptr->prsndmsg;

    /* Insert sndproc into the ready list */

    /* TODO: Figure out if I should use ready, or manually change
     * the state and insert into the list */
//    sndprptr->prstate = PR_READY;
//    insert(sndproc, readylist, sndprptr->prprio);

    ready(sndproc);

    /* If there are no more blocked senders, adjust the flag accordingly */
    if (isempty(prptr->prblockedsenders)) {
      prptr->prsenderflag = FALSE;
    }
  }
  else {
    /* No other proc are waiting to send msg */
    prptr->prhasmsg = FALSE;	/* Reset message flag		*/
  }

	restore(mask);
	return msg;
}
