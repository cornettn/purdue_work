#include <stdio.h>
#include <unistd.h>

#include "my_malloc.h"

int main() { 

  int* ptr = (int *) my_malloc(0);
  printf("Pointer: %p\n", ptr);

  return 0;
}
