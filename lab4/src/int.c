/*************************************************************************
  usp  1   2   3   4   5   6   7   8   9  10   11   12    13  14  15  16
----------------------------------------------------------------------------
 |uds|ues|udi|usi|ubp|udx|ucx|ubx|uax|upc|ucs|uflag|retPC| a | b | c | d |
----------------------------------------------------------------------------
***************************************************************************/

#ifndef NPROC
#define NPROC 9
#endif


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
    case 3: r = kfork();     break;
    case 4: r = kswitch();   break;
    case 5: r = kwait(b);    break;
    case 6: r = kexit(b);    break;
    default: printf("[%d]: Invalid syscall.\n", a); break;
  }

  /* Place the return value into the ax register at offset (8 * 2) */
  put_word(r, segment, offset + 16);
}

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
