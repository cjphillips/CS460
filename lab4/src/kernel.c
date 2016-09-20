#include "include/type.h"
#include "include/queue.h"

extern int body();

void set_registers(u16 segment)
{
  int i;

  /*put_word(segment, segment, running->ksp + -2);*/
  /*put_word(segment, segment, running->ksp + -4);*/
  put_word(segment, segment, -2); // DS
  put_word(segment, segment, -2); // ES
  for(i = 0; i < 8; i++)
  {
    /* DI, SI, BP, DX, CX, BX, AX, PC=VA(0) */
    put_word(0, segment, (i + 3) * -2);
  }
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
  for (i = 0; i < 12; i++)
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
