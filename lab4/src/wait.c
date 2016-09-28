#include "include/type.h"

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
      /* Search all processes for a child */
      p = &proc[i];
      if (p->status != FREE && p->ppid == running->pid) // Is not free and is a child
      {
        hasChild = 1;
        if (p->status == ZOMBIE)
        {
          /* Found a dead child process */
          printf("[KERNEL] Found zombie process [%d].\n", p->pid);
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
      /* Error, no child found */
      return -1;
    }

    ksleep(running); // Has children, but none are zombies. Sleep.
  }
}
