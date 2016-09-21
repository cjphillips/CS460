#include "t.h"

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
  }

  // If nothing was present in the queue, this will just return null
  return p;
}

printList(char *name, PROC *list)
{
  printf("%s", name);
  while(list)
  {
    printf("%d->", list->pid);
    list = list->next;
  }
  printf("null\n");
}
