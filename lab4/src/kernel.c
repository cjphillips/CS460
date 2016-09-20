#include "include/type.h"
#include "include/queue.h"

#define NUMREG 13

int body()
{
    char c;
    printf("Running Process: %d [ppid:%d]\n", running->pid, running->ppid);

    printf("---------------------------------------------------------------\n");
    printList("Free List   :", freeList);
    printQueue("Ready Queue :", readyQueue);
    printSleep();
    printf("---------------------------------------------------------------\n\n");

    do
    {
       printf("[s|f|w|q]: ");
       c = getc();
       printf("%c\n", c);

       switch (c)
       {
         case 'f': do_kfork();   break;
         case 's': do_tswitch(); break;
         case 'w': do_wait();    break;
         case 'q': do_exit();    break;
         default: printf("Unrecognized input.\n\n"); break;
       }
    } while(1);
}

void set_registers(u16 segment, u16 offset)
{
  int i;

  /*put_word(segment, segment, running->ksp + -2);*/
  /*put_word(segment, segment, running->ksp + -4);*/
  //put_word(segment, segment, segment-2); // DS
  //put_word(segment, segment, segment-2); // ES
  for(i = 0; i < NUMREG; i++)
  {
    put_word(0, segment, i*-22);
    /* DI, SI, BP, DX, CX, BX, AX, PC=VA(0) */
    //put_word(0, segment, (i + 3) * segment-2);
  }
  put_word(segment, segment, -2); // DS
  put_word(segment, segment, -4); // ES
  put_word(segment, segment, -22); // CS
  put_word(0x0020, segment, -24);  // flag
}

PROC *kfork(char *filename)
{
  int i;
  u16 segment;
  u16 offset;
  PROC *p;

  printf("Process [%d] attemping fork ... ", running->pid);
  p = get_proc(&freeList);
  if (!p)
  {
    printf("failed: No more available processes.\n");
    return 0;
  }

  p->status = READY;
  p->priority = 1;
  p->ppid = running->pid;
  p->parent = running;
  nproc++;

  /* Initialize the new proc's stack: */
  for (i = 0; i < 10; i++)
  {
    p->kstack[SSIZE - i] = 0;
  }

  segment = (p->pid + 1) * 0x1000;
  p->kstack[SSIZE - 1] = (int)body;
  p->ksp = &p->kstack[SSIZE - 9];

  load(filename, segment);
  set_registers(segment);

  enqueue(&readyQueue, p);

  printf("success: Process [%d] forked at segment 0x%x.\n", p->pid, segment);

  return p;
}
