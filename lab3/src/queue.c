#include "include/type.h"

PROC *get_proc(PROC **list)
{
  PROC *p = *list, *q = 0;

  while(p)
  {
    if (p->status == FREE)
    {
      if (!q)
      {
        *list = (*list)->next;
        p->next = 0;
        return p;
      }

      q->next = p->next;
      p->next = 0;
      return p;
    }

    q = p;
    p = p->next;
  }

  return 0;
}

int *put_proc(PROC **list, PROC *q)
{
  PROC *p = *list;

  if (!p)
  {
    *list = q;
    return 0;
  }

  while(p->next)
  {
    p = p->next;
  }

  p->next = q;
  return 0;
}

int enqueue(PROC **queue, PROC *p)
{
  PROC *q;

  if (!(*queue))
  {
    // Empty queue
    *queue = p;
    return 0;
  }

  q = *queue;

  if (q->priority < p->priority)
  {
    p->next = q;
    *queue = p;
    return 0;
  }

  while(q->next)
  {
    if (q->next->priority < p->priority)
    {
      // Middle of the list
      p->next = q->next;
      q->next = p;
      return 0;
    }

    q = q->next;
  }

  // End of the list
  p->next = q->next;
  q->next = p;
  return 0;
}

PROC *dequeue(PROC **queue)
{
  PROC *p = *queue;

  if (p)
  {
    // Simply remove the first item in the queue
    *queue = (*queue)->next;
    p->next = 0;
  }

  // If nothing was present in the queue, this will just return null
  return p;
}

int insert(PROC **list, PROC *p)
{
  if (!(*list))
  {
    *list = p;
    return 0;
  }

  p->next = *list;
  *list = p;
}

printSleep()
{
  int i;

  printf("SleepList:  ");
  for(i = 1; i < NPROC; i++)
  {
    if (proc[i].status == SLEEP)
    {
      printf("%d[e=%d]->", proc[i].pid, proc[i].event);
    }
  }
  printf("null\n");
}

printList(char *name, PROC *list)
{
  printf("%s", name);
  while(list)
  {
    printf("%d->", list->pid, list->priority);
    list = list->next;
  }
  printf("null\n");
}

/* Same as print list, but prints the proc's parent id */
printQueue(char *name, PROC *queue)
{
  printf("%s", name);
  while(queue)
  {
    printf("%d[%d]->", queue->pid, queue->priority);
    queue = queue->next;
  }
  printf("null\n");
}
