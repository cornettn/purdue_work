/* cbregister.c - cbregister */

#include <xinu.h>

/*---------------------------------------------------
 * cbregister - callback function
 *---------------------------------------------------
 */

syscall cbregister(
    void (* fp)(void), /* a function pointer to a user callback */
    umsg32 *mbufptr    /* a pointer to a 1 word msg buffer */
    )
{

  struct procent *prptr;

  /* Check to ensure that fp lies within the bounds of text segment, and
   * mbufptr does not point inside the the text segment */
  if (!( (uint32)fp >= (uint32)&text &&
         (uint32)fp <= (uint32)&etext
       )) {

    XDEBUG_KPRINTF("cbregister: BAD -- fp not in text segment: %08X\n", (uint32) &fp);

    /* fp is not within text segment bounds */
    return SYSERR;
  }

  XDEBUG_KPRINTF("cbregister: fp is in text segment\n");

  /* fp is within bounds, so check membufptr */
  if (!((uint32)mbufptr > (uint32)&etext)) {

    XDEBUG_KPRINTF("cbregister: BAD -- mbufptr is in the text segment\n");

    /* mbufptr is inside text */
    return SYSERR;
   }

  XDEBUG_KPRINTF("cbregister: mbufptr is not in text segment\n");

  /* Both arguments are now good to go */

  prptr = &proctab[currpid];


  /* Initialize the callback function fields */
  prptr->prcbptr = fp;
  prptr->prcbvalid = TRUE;

  XDEBUG_KPRINTF("cbregister: Set prcbvalid to TRUE\n");

  /* Initialize the processes bufptr field */
  prptr->prmbufptr = mbufptr;

  return OK;
}
