#include "include/type.h"

int tqueuecleared = 0;

void print_tqueue(TQE *t)
{
  int hit = 0;

  if (t)
  {
    hit = 1;
    printf("[TQUEUE]: [P%d- %d]", t->pproc->pid, t->time);
    t = t->next;
  }

  while(t)
  {
    printf(" -> [P%d- %d]", t->pproc->pid, t->time);
    t = t->next;
  }

  if(hit)
  {
    printf("\n");
  }

  if (tqueuecleared)
  {
    printf("[TQUEUE]: All timers cleared.\n");
    tqueuecleared = 0;
  }
}

TQE *get_timer(TQE **list)
{
  TQE *p = *list, *q = 0;

  while(p)
  {
    if (p->pproc == 0)
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

TQE *clear_timer(TQE **queue)
{
  TQE *t = *queue;
  if (t)
  {
    *queue = (*queue)->next;
    t->next = 0;
  }

  if (!*queue)
  {
    tqueuecleared = 1;
  }

  return t;
}

int set_timer(TQE **queue, int timec)
{
  TQE *tp = *queue, *tprev = 0, *t = 0;
  int accum = 0;

  t = get_timer(&tlist);
  if (!t)
  {
    return -1;
  }

  if (!tp)
  {
    /* Empty timer queue */
    *queue = t;
    (*queue)->time = timec;
    (*queue)->pproc = &proc[running->pid];
    (*queue)->action = 0;
    return 0;
  }

  printf("timer queue not empty.\n");
  while(tp)
  {
    if (accum + tp->time > timec)
    {
      printf("HERE\n");
      break;
    }

    accum += tp->time;
    tprev = tp;
    tp = tp->next;
  }

  t->time = timec - accum;
  t->pproc = &proc[running->pid];
  t->action = 0;

  if (tprev)
  {
    tprev->next = t;
    t->next = tp;
  }
  else
  {
    t->next = *queue;
    *queue = t;
  }

  if (t->next)
  {
    t->next->time -= t->time;
  }

  return 0;
}

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
    p->status = READY;
    return 0;
  }

  q = *queue;

  if (q->priority < p->priority)
  {
    p->next = q;
    *queue = p;
    p->status = READY;
    return 0;
  }

  while(q->next)
  {
    if (q->next->priority < p->priority)
    {
      // Middle of the list
      p->next = q->next;
      q->next = p;
      p->status = READY;
      return 0;
    }

    q = q->next;
  }

  // End of the list
  p->next = q->next;
  q->next = p;
  p->status = READY;
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
