/* cbuser.c - cbuser */

#include <xinu.h>

/*----------------------------------------------------------
 * cbuser - the callback function used for testing
 *----------------------------------------------------------
 */
void cbuser(void) {
  XDEBUG_KPRINTF("cbuser: Inside the callback function\n");
  kprintf("--------------------------\n"
          "Message received! -- %d\n"
          "--------------------------\n", ubuffer);
}
