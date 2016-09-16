#ifndef TYPE_H
#define TYPE_H

#define NPROC      9                // number of PROCs
#define SSIZE      1024             // per proc stack area
#define RED

/* PROC Status */
#define FREE        0
#define READY       1
#define RUNNING     2
#define STOPPED     3
#define SLEEP       4
#define ZOMBIE      5
/***************/

typedef struct proc{
    struct proc *next;
    int    *ksp;               // saved ksp when not running
    int    pid;                // add pid for identify the proc
    int    ppid;               // parent process id
    int    status;             // FREE|READY|DEAD|STOP|etc.
    int    priority;           // Scheduling priority
    struct proc *parent;       // Parent process pointer
    int    event;              // Sleep event
    int    exitCode;           // Process's exit code
    int    kstack[SSIZE];      // proc stack area
} PROC;

int nproc, rflag;

int  procSize = sizeof(PROC);
PROC proc[NPROC], *running, *freeList, *readyQueue, *sleepList;    // define NPROC procs

#endif /* TYPE_H */
