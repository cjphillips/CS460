#include "include/user.h"
#include "include/util.h"
#include "include/io.h"

int (*fptr[ ])() = {ps, chname, fork, exec, kswitch, wait, exit, showMenu };

int TimeSlice = 5;

main(int argc, char *argv[])
{
  char name[NAMESIZE];
  int pid;
  long i, j;

  pid = getpid();
  color = pid + 0x04;
  printf("------------------------------------------------------\n");
  printf("[PROC %d - USER ONE CODE] in segment %x.\n", pid, getcs());
  printf("------------------------------------------------------\n");

  while(1)
  {
    i = j = 0;
    
    while(i < 9000000)
    {
      while(j < 500000)
      {
        j++;
      }
      i++;
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
