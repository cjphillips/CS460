/*************************************************************************
  usp  1   2   3   4   5   6   7   8   9  10   11   12    13  14  15  16
----------------------------------------------------------------------------
 |uds|ues|udi|usi|ubp|udx|ucx|ubx|uax|upc|ucs|uflag|retPC| a | b | c | d |
----------------------------------------------------------------------------
***************************************************************************/

#include "include/type.h"
#include "include/util.h"
/*#include "include/wait.h"*/
/*#include "include/kernel.h"*/
#include "include/int.h"

int kgetpid()
{
  return running->pid;
}

int kps()
{
  int i;
  PROC *p;

  printf("PID  PPID  Status    Name  \n");
  printf("---------------------------\n");
  for(i = 0; i < NPROC; i++)
  {
    p = &proc[i];
    printf(" %d    %d    ", p->pid, p->ppid);
    switch(p->status)
    {
      case 0: printf("FREE      ");   break;
      case 1: printf("READY     ");   break;
      case 2: printf("RUNNING   ");   break;
      case 3: printf("STOPPED   ");   break;
      case 4: printf("SLEEP     ");   break;
      case 5: printf("ZOMBIE    ");   break;
      default: printf("UNKNOWN  ");   break;
    }
  }

  printf("%s\n", p->name);
}

int kchname(char *name)
{
  return -1;
}

int kkfork()
{
  return -1;
}

int ktswitch()
{
  return tswitch();
}

int kkwait(int *status)
{
  return -1;
}

int kkexit(int exitValue)
{
  return -1;
}

/* SYSTEM CALL HANDLER IN C CODE */
int kcinth()
{
  int a, b, c, d, r;
  u16 segment, offset;

  segment = running->uss;
  offset = running->usp;

  a = get_word(segment, offset + 26);
  b = get_word(segment, offset + 28);
  c = get_word(segment, offset + 30);
  d = get_word(segment, offset + 32);

  switch(a)
  {
    case 0: r = kgetpid();   break;
    case 1: r = kps();       break;
    case 2: r = kchname(b);  break;
    case 3: r = kkfork();     break;
    case 4: r = ktswitch();   break;
    case 5: r = kkwait(&b);    break;
    case 6: r = kkexit(b);    break;
    default: printf("[%d]: Invalid syscall.\n", a); break;
  }

  /* Place the return value into the ax register at offset (8 * 2) */
  put_word(r, segment, offset + 16);
}
