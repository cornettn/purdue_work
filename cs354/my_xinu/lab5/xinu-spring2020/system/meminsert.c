/* meminsert.c - meminsert, memextract */

#include <xinu.h>


uint32 memblockflag;


void printblk(struct inusememblk *blk) {
  XDEBUG_KPRINTF("\nblk {\n"
                 "  memptr: 0x%08x (%d)\n"
                 "  memlen: %d\n"
                 "  mnext: 0x%08x (%d)\n"
                 "}\n\n", (long *) blk->memblockptr, (long *) blk->memblockptr,
                 blk->memlen, (long *) blk->mnext, (long *) blk->mnext);
}


/*--------------------------------------------------------------
 * meminsert - insert a block of memory into the processes mem list
 *--------------------------------------------------------------
 */

void meminsert(
    struct inusememblk *hdptr, /* the head of the list to insert into */
    char *memptr               /* memory to insert */
    )
{

  intmask mask = disable();

  struct inusememblk *currblk;
  struct inusememblk *newblk;


  memblockflag = 1;
  newblk = (struct inusememblk *) getmem(8);
  newblk->memblockptr = memptr;
  newblk->memlen = memsize;
  newblk->mnext = NULL;

  XDEBUG_KPRINTF("newblk:\n");
  printblk(newblk);

  currblk = hdptr;

  if (currblk == NULL) {
    XDEBUG_KPRINTF("\n*****\nFirst block\n*****\n");
    hdptr = newblk;
  }
  else {

    /* Insert into list */
    while (currblk->mnext != NULL) {
      if (currblk->mnext->memlen < memsize) {
        currblk = currblk->mnext;
      }
      else {
        /* len of new block is more than currblk->next */

        newblk->mnext = currblk->mnext;
        currblk->mnext = newblk;
      }
    }
  }

  XDEBUG_KPRINTF("LIST:\n\n");

  currblk = hdptr;
  while(currblk != NULL) {
    printblk(currblk);
    currblk = currblk->mnext;
  }
  XDEBUG_KPRINTF("\n\n END LIST:\n\n");

  restore(mask);
}


