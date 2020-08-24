/*  main.c  - main */

#include <xinu.h>

#define UNUSED(x) (void)(x)

#define PART_3_CASE_I (1)
#define PART_3_CASE_II (1)
#define PART_4 (0)

pid32 recvpid;
umsg32 ubuffer;

void sender_proc() {

  /* Allow recv proc to register cb func */
  sleepms(500);

  send(recvpid, 69);
  XDEBUG_KPRINTF("sender_proc: msg sent\n");
  send(recvpid, 70);
  sleepms(1000);
  send(recvpid, 71);
  sleep(4);
  send(recvpid, 72);
}

void recv_proc() {

  struct procent *prptr;
  prptr = &proctab[currpid];

  long *cbvalid = (long *) &prptr->prcbvalid;
  long *tmpvalid = (long *) &proctab[recvpid].prtmpvalid;
  long *tmpbuf = (long *) &proctab[recvpid].prtmpbuf;
  long *mbuf = (long *) &proctab[recvpid].prmbufptr;
  long *ent = (long *) &proctab[recvpid];
  long *cbf = (long *) &proctab[recvpid].prcbptr;
  long *ubuf = (long *) &ubuffer;
  long *state = (long *) &prptr->prstate;

  XDEBUG_KPRINTF("Address of proctab[currpid]: 0x%08x (%d)\n", ent, ent);
  XDEBUG_KPRINTF("Address of prcbvalid: 0x%08x (%d)\n", cbvalid, cbvalid);
  XDEBUG_KPRINTF("Address of prtmpvalid: 0x%08x (%d)\n", tmpvalid, tmpvalid);
  XDEBUG_KPRINTF("Address of prtmpbuf: 0x%08x (%d)\n", tmpbuf, tmpbuf);
  XDEBUG_KPRINTF("Address of prmbufptr: 0x%08x (%d)\n", mbuf, mbuf);
  XDEBUG_KPRINTF("Address of prcbptr: 0x%08x (%d)\n", cbf, cbf);
  XDEBUG_KPRINTF("Address of ubuffer: 0x%08x (%d)\n", ubuf, ubuf);
  XDEBUG_KPRINTF("Address of prstate: 0x%08x (%d)\n", state, state);
  XDEBUG_KPRINTF("Address of sleep: 0x%08x (%d)\n", &sleep, &sleep);

  XDEBUG_KPRINTF("Offset for prcbvalid: %d\n", cbvalid - ent);
  XDEBUG_KPRINTF("Offset for prtmpvalid: %d\n", cbvalid - tmpvalid);
  XDEBUG_KPRINTF("Offset for prtmpbuf (from prtmpvalid) %d\n", tmpvalid - tmpbuf);
  XDEBUG_KPRINTF("Offset for prmbufptr (from prtmpvalid) %d\n", tmpvalid - mbuf);
  XDEBUG_KPRINTF("Offset for prcbotr (from ent) %d\n", cbf - ent);

  XDEBUG_KPRINTF("recv_proc: &cbuser: 0x%08x (%d)\n", &cbuser, &cbuser);
  if (cbregister(&cbuser, &ubuffer) != OK) {
    XDEBUG_KPRINTF("recv_proc: cb func did not register\n");
    userret();
  }

  XDEBUG_KPRINTF("recv_proc: cb func registered\n");

  #if PART_3_CASE_I

  XDEBUG_KPRINTF("---------------\n"
                 "  CASE (I)     \n"
                 "---------------\n");

  sleep(1);
  XDEBUG_KPRINTF("recv_proc: out of sleep\n");
  XDEBUG_KPRINTF("weeeeeeee!\n");
  sleep(1);
  #endif

  #if PART_3_CASE_II

  /* ensures that the time slice will be depleted */

  XDEBUG_KPRINTF("---------------\n"
                 "  CASE (II)    \n"
                 "---------------\n"
                 "recv_proc: starting loop\n");
  while(TRUE)
    ;

  #endif

}

process	main(void)
{
#if PART_3_CASE_I || PART_3_CASE_II

  XDEBUG_KPRINTF("sizeof struct procent: %d\n", sizeof(struct procent));
  resume(create(sender_proc, 1024, 5, "sender", 0, NULL));
  recvpid = create(recv_proc, 1024, 5, "recv", 0, NULL);
  resume(recvpid);

  XDEBUG_KPRINTF("&proctab: 0x%08x (%d)\n",
      &proctab, &proctab);
  XDEBUG_KPRINTF("&proctab[currpid]: 0x%08x (%d)\n",
      &proctab[currpid], &proctab[currpid]);
  XDEBUG_KPRINTF("size of procent: %d\n", sizeof(struct procent));

  XDEBUG_KPRINTF("recv pid: %d\n", recvpid);
#endif


#if PART_4

  XDEBUG_KPRINTF("getting mem\n");
  char *test = (char *) getmem(400);
  XDEBUG_KPRINTF("got mem\n");

  XDEBUG_KPRINTF("getting mem\n");
  char *anothertest = (char *) getmem(300);
  XDEBUG_KPRINTF("got mem\n");

  XDEBUG_KPRINTF("freeing mem\n");
  freemem(test, 400);
  XDEBUG_KPRINTF("freed mem\n");


#endif




}
