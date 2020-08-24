/* memextract.c - memextract */

#include <xinu.h>

/*--------------------------------------------------------------
 * meminsert - insert a block of memory into the processes mem list
 *--------------------------------------------------------------
 */

void memextract(
    struct inusememblk *hdptr, /* the head of the list to insert into */
    char *memptr               /* memory to insert */
    )
{

  intmask mask = disable();

  struct inusememblk *currblk;

  currblk = hdptr;

  if (currblk->memblockptr == memptr) {
    hdptr = currblk->mnext;
    memblockflag = 1;
    freemem((char *)currblk, 8);
    return;
  }

  /* Iterate through list to find */

  while (currblk->mnext != NULL) {
    if (currblk->mnext->memblockptr == memptr) {
      struct inusememblk *nxt = currblk->mnext->mnext;
      memblockflag = 1;
      freemem((char *)currblk->mnext, 8);
      currblk->mnext = nxt;
      break;
    }
  }

  restore(mask);
}


