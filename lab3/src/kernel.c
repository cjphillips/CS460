#include "include/type.h"
#include "include/queue.h"
#include "include/wait.h"

#define NUMREG 13

extern int body();

void set_registers(u16 segment, u16 offset)
{
  int i;

  /*put_word(segment, segment, running->ksp + -2);
  put_word(segment, segment, running->ksp + -4);
  put_word(segment, segment, segment-2); // DS
  put_word(segment, segment, segment-2); // ES*/
  for(i = 1; i < NUMREG; i++)
  {
    put_word(0, segment, i*-2);
    /* DI, SI, BP, DX, CX, BX, AX, PC=VA(0)
    put_word(0, segment, (i + 3) * segment-2);*/
  }
  put_word(segment, segment, offset); // DS
  put_word(segment, segment, offset + 2); // ES
  put_word(segment, segment, offset + 20);  // CS
  put_word(0x0020, segment, offset + 18);   // flag
}

PROC *kfork(char *filename)
{
  int i;
  u16 segment;
  PROC *p;

  printf("Process [%d] attemping fork ... ", running->pid);
  p = get_proc(&freeList);
  if (!p)
  {
    printf("failed: No more available processes.\n");
    return 0;
  }
  printf("Done.\n");

  p->status = READY;
  p->priority = 1;
  p->ppid = running->pid;
  p->parent = running;
  nproc++;

  /* Initialize the new proc's stack: */
  for (i = 1; i < 10; i++)
  {
    p->kstack[SSIZE - i] = 0;
  }

  segment = (p->pid + 1) * 0x1000;
  p->kstack[SSIZE - 1] = (int)body;
  p->ksp = &(p->kstack[SSIZE - 9]);

  p->usp = proc[p->pid].usp;
  p->uss = segment;

  load(filename, segment);

  printf("Setting registers ... ");
  set_registers(segment, p->usp);
  printf("Done.\n");

  enqueue(&readyQueue, p);

  printf("Success: Process [%d] forked at segment %x.\n", p->pid, segment);

  return p;
}

int kexit(int exitValue)
{
  PROC *p;
  int i, wakeupP1 = 0;

  if (running->pid == 1 && nproc > 2)
  {
    printf("Cannot exit, child processes are still running (P1 cannot die yet).\n");
    return -1;
  }

  // Send children, dead or alive, to P1's orphanage
  for(i = 1; i < NPROC; i++)
  {
    p = &proc[i];
    if(p->status != FREE && p->ppid == running->pid)
    {
      p->ppid = 1;
      p->parent = &proc[1];
      wakeupP1++;
    }
  }

  // Record exit value and become a zombie
  running->exitCode = exitValue;
  running->status = ZOMBIE;

  printf("Process [%d] exited with code %d.\n", running->pid, exitValue);

  // Wakeup parent (and also P1 if necessary)
  kwakeup(running->parent);
  if (wakeupP1)
  {
    printf("Waking up process [1].\n");
    kwakeup(&proc[1]);
  }
  tswitch(); // Give up the CPU
}
