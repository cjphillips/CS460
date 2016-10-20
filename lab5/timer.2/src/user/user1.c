#include "include/user.h"
#include "include/util.h"
#include "include/io.h"

int (*fptr[ ])() = {ps, chname, fork, exec, kswitch, wait, exit, showMenu };

int TimeSlice = 5;

main(int argc, char *argv[])
{
  char name[NAMESIZE];
  int pid, ppid;
  long i, j;

  while(1)
  {
    pid = getpid();
    ppid = getppid();
    color = pid + 0x04;
    i = j = 0;
    printf("------------------------------------------------------\n");
    printf("[PROC %d (PPID: %d)]\n", pid, ppid);
    printf("------------------------------------------------------\n\n\n");
    while(1)
    {
      while(i++ < 999999)
      {
        while(j++ < 100000);
      }
    }
  }
}

crt0(char *str)
{
  int argc = 0;
  printf("\n");
  argc = tokenize(str);

  main(argc, args);
}
