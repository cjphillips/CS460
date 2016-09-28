#include "include/type.h"
#include "include/queue.h"
#include "include/wait.h"

#define NUMREG 13
#define FLAG   0x0020

extern int body();

int kexec(char *ufile)
{
  int i, length = 0;
  char filename[FILE_LENGTH], *cp = filename;

  u16 segment = running->uss;

  /* Get the filename from the user's segment */
  while( (*cp++ = get_byte(segment, ufile++)) && length++ < 64);

  if (load(filename, segment) < 0)
  {
    return -1;
  }

  /* Re-initialize the user's proc stack so that it returns to VA (0) */
  for(i = 1; i < 12; i++)
  {
    put_word(0, segment, -2*i);
  }
  running->usp = 0xFFE8; // New USP at -24

  /* -1  -2  -3  -4  -5  -6  -7  -8  -9  -10  -11  -12    (Ustack layout) */
  /* FL  CS  PC  AX  BX  CX  DX  BP  SI  DI   ES   DS     (Registers)     */
  put_word(segment, segment, -2*12);
  put_word(segment, segment, -2*11);
  put_word(segment, segment, -2*-2);
  put_word(0x0020,  segment, -2);
}

void set_registers(u16 segment, u16 offset)
{
  int i;

  for(i = 0; i < NUMREG - 1; i++)
  {
    /* DI, SI, BP, DX, CX, BX, AX, PC=VA(0)*/
    put_word(0, segment, offset + i * 2);
  }

  put_word(segment, segment, offset); // DS
  put_word(segment, segment, offset + 2); // ES
  put_word(segment, segment, offset + 20);  // CS
  put_word(FLAG, segment, offset + 22);   // flag
}

void copyImage(u16 parentSeg, u16 childSeg, u16 size)
{
  u16 i;

  for(i = 0; i < size; i++)
  {
    put_word(get_word(parentSeg, 2*i), childSeg, 2*i);
  }
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

  if (filename)
  {
    printf("[KERNEL] ");
    load(filename, segment);

    printf("[KERNEL] Setting registers ... ");
    set_registers(segment, p->usp);
    printf("done.\n");
  }

  enqueue(&readyQueue, p);

  printf("[KERNEL] Success: Process [%d] forked at segment %x.\n", p->pid, segment);

  nproc++;
  return p;
}

int fork()
{
  int pid;
  u16 segment;

  PROC *p = kfork(0);
  if (!p)
  {
    return -1;
  }

  segment = (p->pid + 1) * 0x1000;
  copyImage(running->uss, segment, 32 * 1024); // Copy 32K words

  p->uss = segment;
  p->usp = running->usp;

  put_word(segment, segment, p->usp);       // DS
  put_word(segment, segment, p->usp+2);     // ES
  put_word(0,       segment, p->usp+2*8);   // AX
  put_word(segment, segment, p->usp+2*10);  // CS

  return p->pid;
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

  tswitch(); // Give up the CPU
}
