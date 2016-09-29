#include "include/type.h"
#include "include/queue.h"

int body()
{
    char c;
    color = 0x02;
    printf("[KERNEL] P%d running.\n", running->pid);

    printf("---------------------------------------------------------------\n");
    printList("Free List   :", freeList);
    printQueue("Ready Queue :", readyQueue);
    printSleep();
    printf("---------------------------------------------------------------\n\n");
    do
    {
       printf("[s|f|w|q|u]: ");
       c = getc();
       printf("%c\n", c);

       switch (c)
       {
         case 'f': do_kfork();   break;
         case 's': do_tswitch(); break;
         case 'w': do_wait();    break;
         case 'q': do_exit();    break;
         case 'u': goUmode();    break;
         default: printf("Unrecognized input.\n\n"); break;
      }
    } while(1);
}

int init()
{
  PROC *p;
  int i;

  printf("[KERNEL] Initializing processes ... ");

  for(i = 0; i < NPROC; i++)
  {
    p = &proc[i];
    p->pid = i;
    p->ppid = 0;
    p->status = FREE;
    p->priority = 0;
    strcpy(proc[i].name, pname[i]);
    p->next = &proc[i + 1];
  }

  freeList = &proc[0];
  proc[NPROC - 1].next = 0;
  readyQueue = sleepList = 0;

  /* Set Proc 0 as the running process: */
  p = get_proc(&freeList);
  p->status = RUNNING;
  p->ppid   = 0;
  p->parent = p;
  running   = p;
  nproc     = 1;
  p->usp    = 0xFFE8;

  printf("Complete.\n");
}

int scheduler()
{
  if (running->status == RUNNING)
  {
    running->status = READY;
    enqueue(&readyQueue, running);
  }

  running = dequeue(&readyQueue);
  running->status = RUNNING;
}

int int80h();

int set_vector(u16 vector, u16 handler)
{
  // Usage: put_word(word, segment, offset)

  put_word(handler, 0, vector << 2);
  put_word(0x1000, 0, (vector << 2) + 2);
}

main()
{
  color = 0x04;
  printf("\n\nWelcome! Starting in MTX kernel.\n\n");
  init();
  set_vector(80, int80h);

  kfork("/bin/user1"); // Fork to process 1

  printf("[KERNEL] P%d running.\n", running->pid);
  while(readyQueue)
  {
    printf("[KERNEL] Switching process away from P0.\n\n");
    tswitch();
  }

  printf("< No runnable processes -- System halting >.\n");
}
