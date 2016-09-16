#ifndef T_H
#define T_H

#define NPROC    9             // number of PROCs
#define SSIZE 1024             // per proc stack area
#define RED

/* PROC Status */
#define FREE     0
#define READY    1
#define STOP     2
#define DEAD     3
/***************/

typedef struct proc{
    struct proc *next;
    int    *ksp;               // saved ksp when not running
    int    pid;                // add pid for identify the proc
    int    ppid;               // parent process id
    int    status;             // FREE|READY|DEAD|STOP|etc.
    int    priority;           // Scheduling priority
    int    kstack[SSIZE];      // proc stack area
} PROC;

int  procSize = sizeof(PROC);

PROC proc[NPROC], *running, *freeList, *readyQueue;    // define NPROC procs
extern int color;

PROC *get_proc(PROC **list);

int *put_proc(PROC **list, PROC *q);

int enqueue(PROC **queue, PROC *p);

PROC *dequeue(PROC **queue);

int init();

int scheduler();

PROC *kfork();

int body();

int do_kfork();

int do_tswitch();

#endif /* T_H */
