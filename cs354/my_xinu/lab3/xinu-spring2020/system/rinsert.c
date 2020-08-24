/* rinsert.c - rinsert */

#include <xinu.h>

/*------------------------------------
 * rinsert - Inserts processes in ascending
 *           order of priorty into the ready
 *           list.
 *------------------------------------
 */

status rinsert(
    pid32 pid, /* ID of process to insert */
    qid16 q, /* ID of the queue to use */
    int32 key /* Key for the inserted process */
    )
{
	qid16	curr;			/* Runs through items in a queue*/
	qid16	prev;			/* Holds previous node index	*/

  if (isbadqid(q) || isbadpid(pid)) {
		return SYSERR;
	}

	curr = firstid(q);

  /* sort in non-decreasing order, so <= */

	while (queuetab[curr].qkey <= key) {
		curr = queuetab[curr].qnext;
	}

	/* Insert process between curr node and previous node */

	prev = queuetab[curr].qprev;	/* Get index of previous node	*/
	queuetab[pid].qnext = curr;
	queuetab[pid].qprev = prev;
	queuetab[pid].qkey = key;
	queuetab[prev].qnext = pid;
	queuetab[curr].qprev = pid;
  return OK;

}
