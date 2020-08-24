#include <xinu.h>

void mymotd(void) {

  /* Only print this message is XINUTEST is defined */

  #if XINUTEST == 1
  char *name = "Noah Cornett";
  char *username = "cornettn";
  kprintf("\nHello! My name is %s and I am the developer of this version of"
     " xinu! My username is %s.\n", name, username);
  #endif
}
