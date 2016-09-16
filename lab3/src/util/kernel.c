#include "../include/type.h"
#include "../include/kernel.h"

extern int body();
extern int nproc;

PROC *kfork()
{
   int i;
   PROC *p;

   printf("Proccess [pid:%d] attempting fork ... ", running->pid);
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

   /* initialize new proc's stack */
   for(i = 1; i < 12; i++)
   {
     p->kstack[SSIZE - i] = 0;
   }

   printf("success: Process [pid:%d] created.\n", p->pid);

   p->kstack[SSIZE - 1] = (int)body;
   p->ksp = &p->kstack[SSIZE-11];
   p->kstack[SSIZE-10] = p->kstack[SSIZE-11] = 0x1000;
   enqueue(&readyQueue, p);
   return p;
}

int ksleep(int event)
{
  PROC *p;

  running->event = event;  // Record the event value in the running process's event field
  running->status = SLEEP; // Set the running process to SLEEP
  tswitch();               // Give up CPU to another process.
}

int kwakeup(int event)
{
  int i;
  PROC *p;

  for(i = 1; i < NPROC; i++) // Skip P0, not applicable to the origin process
  {
    p = &proc[i];
    if (p->status == SLEEP && p->event == event)
    {
      p->event = 0;            // Cancel this event
      p->status = READY;       // Ensure that this process is ready to run again
      enqueue(&readyQueue, p);
    }
  }
}

/* Wait for a zombie child */
int kwait(int *status)
{
  PROC *p;
  int i, hasChild = 0;

  while(1)
  {
    for(i = 1; i < NPROC; i++)
    {
      // Search all processes for a child
      p = &proc[i];
      if (p->status != FREE && p->ppid == running->pid) // Is not free and is a child
      {
        hasChild = 1;
        if (p->status == ZOMBIE)
        {
          // Found a dead child process
          printf("Found zombie process [%d].\n", p->pid);
          *status = p->exitCode;
          p->status = FREE;
          put_proc(&freeList, p);
          nproc--;
          return (p->pid);
        }
      }
    }

    if (!hasChild)
    {
      // Error, no child found
      return -1;
    }

    ksleep(running); // Has children, but none are zombies. Sleep.
  }
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
