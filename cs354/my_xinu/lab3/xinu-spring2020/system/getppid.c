/* getppid.c - getppid */

#include <xinu.h>

/* --------------------------------------------------------------------
 * getppid - This will return the pid of the given processes parent.
 * --------------------------------------------------------------------
 */
pid32 getppid(
    pid32 pid /* The pid whose parent's id will be returned */
    )
{

  intmask mask;
  mask = disable();

  if (pid <= 0) {
    return SYSERR;
  }

  return proctab[pid].prparent;

  restore(mask);

}
