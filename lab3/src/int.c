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

static char *ucode = "/bin/user1";

int kgetpid()
{
  return running->pid;
}

int kps()
{
  int i;
  PROC *p;

  printf("PID  PPID  Status    Name  \n");
  printf("------------------------------\n");
  for(i = 0; i < NPROC; i++)
  {
    p = &proc[i];
    if (p->status != FREE)
    {
      printf(" %d    %d  ", p->pid, p->ppid);
    }
    else
    {
      printf("           ");
    }
    switch(p->status)
    {
      case FREE:    printf("FREE      ");   break;
      case READY:   printf("READY     ");   break;
      case RUNNING: printf("RUNNING   ");   break;
      case STOPPED: printf("STOPPED   ");   break;
      case SLEEP:   printf("SLEEP     ");   break;
      case ZOMBIE:  printf("ZOMBIE    ");   break;
      default:      printf("UNKNOWN   ");   break;
    }
    color = 0x0A + p->pid;
    printf("%s\n", p->name);
    color = 0x0A;
  }

  return 0;
}

int kchname(char *name)
{
  u16 segment;
  int i = 0;
  char c;

  if (!name)
  {
    return -1;
  }

  segment = running->uss;

  while(i < NAMESIZE)
  {
    c = get_word(segment, name + i);
    running->name[i] = c;
    if (!c)
    {
      break;
    }
    i++;
  }

  return 0;
}

int kkfork()
{
  PROC *p = kfork(ucode);

  if (p)
  {
    return p->pid;
  }

  return -1;
}

int ktswitch()
{
  return tswitch();
}

int kkwait(int *status)
{
  int r = kwait(status);
  put_word(*status, running->uss, status);

  return r;
}

int kkexit(int exitValue)
{
  return kexit(exitValue);
}

char *getSyscallName(int value)
{
  switch(value)
  {
    case 0: return "getpid";
    case 1: return "ps";
    case 2: return "chname";
    case 3: return "kfork";
    case 4: return "switch";
    case 5: return "wait";
    case 6: return "exit";
    default: return "Invalid syscall.";
  }
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

  if (a != 0)
  {
    /* Just to avoid printing redundant getPid syscalls */
    printf("[KERNEL] Processing syscall: \'%s\' \n", getSyscallName(a));
  }

  switch(a)
  {
    case 0: r = kgetpid();   break;
    case 1: r = kps();       break;
    case 2: r = kchname(b);  break;
    case 3: r = kkfork();     break;
    case 4: r = ktswitch();   break;
    case 5: r = kkwait(&b);    break;
    case 6: r = kkexit(b);    break;
    default: printf("[KERNEL] Invalid syscall.\n", a); break;
  }

  printf("[KERNEL] Syscall returning with a value of %d.\n", r);
  /* Place the return value into the ax register at offset (8 * 2) */
  put_word(r, segment, offset + 16);
}
