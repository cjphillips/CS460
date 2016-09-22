#include "include/type.h"
#include "include/queue.h"
#include "include/wait.h"

#define NUMREG 13
#define FLAG   0x0020

extern int body();

void set_registers(u16 segment, u16 offset)
{
  int i;

  printf("  -> usp=%x\n", offset);

  for(i = 0; i < NUMREG - 1; i++)
  {
    /* DI, SI, BP, DX, CX, BX, AX, PC=VA(0)*/
    put_word(0, segment, offset + i * 2);
  }

  put_word(segment, segment, offset); // DS
  put_word(segment, segment, offset + 2); // ES
  put_word(segment, segment, offset + 20);  // CS
  put_word(FLAG, segment, offset + 22);   // flag

  printf("  [DS(&..%d)=%x]\n  [ES(&..%d)=%x]\n  [FL(&..%d)=%x]\n  [CS(&..%d)=%x]\n",
    offset,      segment,
    offset + 2,  segment,
    offset + 20, segment,
    offset + 22, FLAG);
}

PROC *kfork(char *filename)
{
  int i;
  u16 segment;
  PROC *p;

  printf("[KERNEL] Process [%d] attemping fork ... ", running->pid);
  p = get_proc(&freeList);
  if (!p)
  {
    printf("failed: No more available processes.\n");
    return 0;
  }
  printf("\n");

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

  p->usp = running->usp;
  p->uss = segment;

  printf("[KERNEL] ");
  load(filename, segment);

  printf("[KERNEL] Setting registers ...\n");
  set_registers(segment, p->usp);

  enqueue(&readyQueue, p);

  printf("[KERNEL] Success: Process [%d] forked at segment %x.\n", p->pid, segment);

  nproc++;
  return p;
}

int kexit(int exitValue)
{
  PROC *p;
  int i, wakeupP1 = 0;

  if (running->pid == 1 && nproc > 2)
  {
    printf("[KERNEL] Cannot exit, child processes are still running (P1 cannot die yet).\n");
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

  printf("[KERNEL] Process [%d] exited with code %d.\n", running->pid, exitValue);

  // Wakeup parent (and also P1 if necessary)
  kwakeup(running->parent);
  if (wakeupP1)
  {
    printf("[KERNEL] Waking up process [1].\n");
    kwakeup(&proc[1]);
  }
  getc();
  tswitch(); // Give up the CPU
}
