#ifndef TYPE_H
#define TYPE_H

typedef unsigned char   u8;
typedef unsigned short u16;
typedef unsigned long  u32;

#define NPROC    9
#define SSIZE 1024

/******* PROC status ********/
#define FREE     0
#define READY    1
#define RUNNING  2
#define STOPPED  3
#define SLEEP    4
#define ZOMBIE   5

#define NAMESIZE 32

#define FILE_LENGTH 64

typedef struct proc{
    struct proc *next;
    int    *ksp;               // at offset 2

    int    uss, usp;           // at offsets 4,6
    int    inkmode;            // at offset 8

    int    pid;                // add pid for identify the proc
    int    status;             // status = FREE|READY|RUNNING|SLEEP|ZOMBIE
    int    ppid;               // parent pid
    struct proc *parent;
    int    priority;
    int    event;
    int    exitCode;
    char   name[NAMESIZE];           // name string of PROC

    int    kstack[SSIZE];      // per proc stack area
}PROC;

typedef struct tqe {
  struct tqe   *next;  // Next timer pointer
  int           time;  // Requested time
  struct proc  *pproc;  // Pointer to the calling process
  int (*action) ();    // Function pntr handlers (0: wakeup, 1: notify)
}TQE;

PROC proc[NPROC], *running, *freeList, *readyQueue, *sleepList;
TQE tqe[NPROC], *tq, *tlist;

int procSize = sizeof(PROC);
int nproc = 0;

char *pname[] = {
  "Sun", "Mercury", "Venus", "Earth",  "Mars", "Jupiter",
  "Saturn", "Uranus", "Neptune"
};

int color;

#endif /* TYPE_H */
