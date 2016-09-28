#ifndef QUEUE_H
#define QUEUE_H

PROC *get_proc(PROC **list);

int *put_proc(PROC **list, PROC *q);

int enqueue(PROC **queue, PROC *p);

PROC *dequeue(PROC **queue);

printList(char *name, PROC *list);

printQueue(char *name, PROC *queue);

printSleep();

#endif /* QUEUE_H */
