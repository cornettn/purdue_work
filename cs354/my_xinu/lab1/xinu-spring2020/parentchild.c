/**
 * @author Noah Cornett
 * @date 1-30-2020
 */

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <string.h>
#include <malloc.h>


/*
 * Priority test when parent in written first.
 */

void trial_2() {
  int ret = fork();
  if (ret == -1) {
    char *str = "Uh Oh\n";
    write(1, str, strlen(str));
  }
  else if (ret != 0) {
    char *str = (char *) malloc(100);
    sprintf(str, "I am the parent with priority %d\n", getpriority(PRIO_PROCESS, 0));
    write(1, str, strlen(str));
  }
  else {
    char *str = (char *) malloc(100);
    sprintf(str, "I am the child with priority %d\n", getpriority(PRIO_PROCESS, 0));
    write(1, str, strlen(str));
  }
}

/*
 * Priority test when child is written first.
 */

void trial_1() {
  int ret = fork();
  if (ret == -1) {
    char *str = "Uh Oh\n";
    write(1, str, strlen(str));
  }
  else if (ret == 0) {
    char *str = (char *) malloc(100);
    sprintf(str, "I am the child with priority %d\n", getpriority(PRIO_PROCESS, 0));
    write(1, str, strlen(str));
  }
  else {
    char *str = (char *) malloc(100);
    sprintf(str, "I am the parent with priority %d\n", getpriority(PRIO_PROCESS, 0));
    write(1, str, strlen(str));
  }
}



/*
 * This is testing the priority system on linux.
 */

int main() {

  write(1, "Trial 1\n\0", 9);
  trial_1();
  write(1, "Trial 2\n\0", 9);
  trial_2();

}
