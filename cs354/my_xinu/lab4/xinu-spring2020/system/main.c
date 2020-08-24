/*  main.c  - main */

#include <xinu.h>

#define UNUSED(x) (void)(x)
#define PART_3 (1)
#define PART_4_2 (1)
#define PART_4_3 (1)


pid32 recvpid;
void sendmsg();
void sendmoremsg();
void recvmsg();

process	main(void)
{

  #if PART_3
  rcreate(sendmsg, 1024, 5, "sendmsg", 0, NULL);
  rcreate(sendmoremsg, 1024, 5, "sendmoremsg", 0 , NULL);
  recvpid = rcreate(recvmsg, 1024, 5, "recvmsg", 0, NULL);
  bsend(recvpid, 50000);
  #endif

  #if PART_4_2
  resume(create(victimA, 1024, 5, "v1", 0, NULL));
  resume(create(victimA, 1024, 5, "v2", 0, NULL));
  resume(create(victimA, 1024, 5, "v3", 0, NULL));
  resume(create(attackerA, 1024, 5, "a1", 1, 2));
  #endif


  #if PART_4_3
  resume(create(victimA, 1024, 5, "v1", 0, NULL));
  resume(create(victimA, 1024, 5, "v2", 0, NULL));
  resume(create(victimA, 1024, 5, "v3", 0, NULL));
  resume(create(attackerB, 1024, 5, "a1", 1, 1));
  #endif

}

void haha() {
  kprintf("HAHAHA\n");
}

void sendmsg() {
  sleepms(50);

  bsend(recvpid, 1);
  bsend(recvpid, 2);
  bsend(recvpid, 3);
}

void sendmoremsg() {
  sleepms(50);

  bsend(recvpid, 101);
  bsend(recvpid, 102);
  bsend(recvpid, 103);
}

void recvmsg() {
  sleepms(1000);

  while(1) {
    int32 msg = receive();
    XDEBUG_KPRINTF("recvmsg: received %d\n", msg);
  }
}

