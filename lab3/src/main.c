#include "include/type.h"
#include "include/util.h"
#include "include/queue.h"
#include "include/kernel.h"

int color;

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
      if (rflag)
      {
        printf("Process [%d]: Rescheduled.\n", running->pid);
        rflag = 0;
        tswitch();
      }
       printf("Running Process: %d [ppid:%d]\n", running->pid, running->ppid);

       printf("---------------------------------------------------------------\n");
       printList("Free List   :", freeList);
       printQueue("Ready Queue :", readyQueue);
       printSleep();
       printf("---------------------------------------------------------------\n\n");
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

int init()
{
    PROC *p;
    int i;

    printf("Initializing ... ");

    for(i = 0; i < NPROC; i++)
    {
       p = &proc[i];
       p->pid = i;
       p->ppid = i - 1;
       p->status = FREE;
       p->priority = 0;
       p->next = &proc[i + 1];
    }

    proc[NPROC-1].next = 0;
    freeList = &proc[0];
    readyQueue = sleepList = 0;

    /* Start P0 as the running process */
    p = get_proc(&freeList);
    p->ppid = 0;
    p->status = READY;
    running = p;
    nproc = 1;

    printf("complete.\n");
 }

int scheduler()
{
   if (running->status == READY)
   {
      enqueue(&readyQueue, running);   // Only enter if the running process is ready
   }

   running = dequeue(&readyQueue);    // New running process
   rflag = 0;
   //color = running->pid;
   printf("Switched running process to [pid:%d]\n", running->pid);

   if (running->pid == 0)
   {
     printf("< No runnable processes -- System halting >.\n");
   }
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
   tswitch();
}

int do_wait()
{
  if (kwait() < 0)
  {
    printf("Error: running process [%d] has no children.\n", running->pid);
  }
}

int do_exit()
{
  kexit(0);
}
