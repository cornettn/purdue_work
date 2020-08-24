/*  main.c  - main */

#include <xinu.h>

#define UNUSED(x) (void)(x)

#define PART_THREE (0)
#define PART_FIVE (1)


void check(pid32);
void p1();

process	main(void)
{

  #if PART_FIVE == 1
/*
  resume(create((void *) testcpu, 1024, 20, "1", 0));
  resume(create((void *) testcpu, 1024, 20, "2", 0));
  resume(create((void *) testcpu, 1024, 20, "3", 0));
  resume(create((void *) testcpu, 1024, 20, "4", 0));
*/
  /*
  resume(create((void *) testio, 1024, 20, "5", 0));
  resume(create((void *) testio, 1024, 20, "6", 0));
  resume(create((void *) testio, 1024, 20, "7", 0));
  resume(create((void *) testio, 1024, 20, "8", 0));
 */

/*
  resume(create((void *) testcpu, 1024, 20, "1", 0));
  resume(create((void *) testcpu, 1024, 20, "2", 0));
  resume(create((void *) testcpu, 1024, 20, "3", 0));
  resume(create((void *) testcpu, 1024, 20, "4", 0));
  resume(create((void *) testio, 1024, 20, "5", 0));
  resume(create((void *) testio, 1024, 20, "6", 0));
  resume(create((void *) testio, 1024, 20, "7", 0));
  resume(create((void *) testio, 1024, 20, "8", 0));
*/

  ///*
  resume(create((void *) testcpu, 1024, 20, "1", 0));
  sleepms(500);
  resume(create((void *) testcpu, 1024, 20, "2", 0));
  sleepms(500);
  resume(create((void *) testcpu, 1024, 20, "3", 0));
  sleepms(500);
  resume(create((void *) testcpu, 1024, 20, "4", 0));
  sleepms(500);
//*/
  #endif


  #if PART_THREE == 1

  pid32 pid = rcreate(p1, 1024, 20, "p1", 0);

  while(1){
    XDEBUG_KPRINTF(
        "------------------------------\n"
        "P1 Process\n"
        "------------------------------\n");
    check(pid);
    sleep(1);
  }

  #endif


}

/* infinite loop */

void p1() {
  while (1){
    ;
  }
}


void check(pid32 pid) {

  uint32 grosscpumilli = procgrosscpu(pid);
  XDEBUG_KPRINTF("Process %d has gross cpu %d\n", pid, grosscpumilli);

  uint64 grosscpumicro = procgrosscpumicro(pid);
  XDEBUG_KPRINTF("Process %d has gross cpu ticks %d\n", pid, grosscpumicro);

  XDEBUG_KPRINTF("Difference in micro seconds: %d\n", grosscpumilli * 1000 - grosscpumicro);

}
