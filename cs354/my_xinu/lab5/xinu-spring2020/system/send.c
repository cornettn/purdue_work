/* send.c - send */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  send  -  Pass a message to a process and start recipient if waiting
 *------------------------------------------------------------------------
 */
syscall	send(
	  pid32		pid,		/* ID of recipient process	*/
	  umsg32	msg		/* Contents of message		*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/

	mask = disable();
	if (isbadpid(pid)) {
		restore(mask);
		return SYSERR;
	}

	prptr = &proctab[pid];
	if (prptr->prhasmsg) {
		restore(mask);
		return SYSERR;
	}


  if (prptr->prstate == PR_SLEEP ||
      prptr->prstate == PR_READY) {
    if (prptr->prcbvalid) {
      /* The receiving process has a registered cb function */

      XDEBUG_KPRINTF("send.c: recevier has valid cb function\n");

      if (prptr->prtmpvalid) {
        /* receiving process has a message already and the cb func has not been
        * called yet. Discard the new message. */

        XDEBUG_KPRINTF("send.c: receiver already has a message\n");
        return OK;
        //msg = prptr->prtmpbuf;
      }

      prptr->prtmpvalid = TRUE;
      prptr->prtmpbuf = msg;

      XDEBUG_KPRINTF("send.c: Setting prtmpbuf to %d\n", msg);
    }
  }
  else {

    /* send mesasge without a callback  */

    XDEBUG_KPRINTF("send.c: Recv proc has no cb func\n");
    prptr->prmsg = msg;		/* Deliver message		*/
	  prptr->prhasmsg = TRUE;		/* Indicate message is waiting	*/

  }


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
