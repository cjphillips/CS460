#include "t.h"

main()
{
    printf("\n\nMTX starts in main()\n");
    init();
    kfork();
    while(1)
    {
      if(readyQueue)
      {
        tswitch();
      }
    }
}

int body()
{
    char c;
    do
    {
       printf("Running Process: %d\n   Parent Pid: %d\n", running->pid, running->ppid);
       printf("---------------------------------------------------------------\n");
       printList("Free List   :", freeList);
       printList("Ready Queue :", readyQueue);
       printf("---------------------------------------------------------------\n\n");
       printf("[s|f]: ");
       c = getc();
       printf("%c\n", c);

       switch (c)
       {
         case 'f': do_kfork(); break;
         case 's': do_tswitch(); break;
         default: printf("Unrecognized input.\n\n"); break;
      }
    } while(1);
}

int init()
{
    PROC *p;
    int i;

    printf("Initializing ... ");

    for(i = 0; i < NPROC; i++)
    {
       p = &proc[i];
       p->pid = i;
       p->status = FREE;
       p->priority = 0;
       p->next = &proc[i + 1];
    }

    proc[NPROC-1].next = 0;
    freeList = &proc[0];
    readyQueue = 0;

    /* Start P0 as the running process */
    p = get_proc(&freeList);
    p->ppid = 0;
    p->status = READY;
    running = p;

    printf("complete.\n");

    printf("Running: [%d]\n", running->pid);
 }

int scheduler()
{
   if (running->status == READY)
   {
      enqueue(&readyQueue, running);   // Only enter if the running process is ready
   }

   running = dequeue(&readyQueue);    // New running process
   printf("Switched running process to [pid:%d]\n", running->pid);
}

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

   /* initialize new proc's stack */
   for(i = 1; i < 10; i++)
   {
     p->kstack[SSIZE - i] = 0;
   }

   printf("success: Process [pid:%d] created.\n", p->pid);

   p->kstack[SSIZE - 1] = (int)body;
   p->ksp = &p->kstack[SSIZE-9];
   enqueue(&readyQueue, p);
   return p;
}

int do_kfork()
{
   PROC *p;
   printf("Forking...\n");
   p = kfork();
   if (!p)
   {
      return -1;
   }

   return p->pid;
}

int do_tswitch()
{
   printf("Switching...\n");
   tswitch();
}
