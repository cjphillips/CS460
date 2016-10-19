#include "include/user.h"
#include "include/util.h"
#include "include/io.h"

int (*fptr[ ])() = {ps, chname, fork, exec, kswitch, wait, exit, showMenu };

int TimeSlice = 5;

main(int argc, char *argv[])
{
  char name[NAMESIZE];
  int pid, i, j;

  while(1)
  {
    pid = getpid();
    color = pid + 0x04;
    printf("------------------------------------------------------\n");
    printf("[PROC %d - USER ONE CODE] in segment %x.\n", pid, getcs());
    printf("[PROC %d] Processing for %d seconds\n", pid, TimeSlice);
    printf("------------------------------------------------------\n");

    while(i < 100000)
    {
      while(j < 200)
      {
        j++;
      }
      i++;
    }

    printf("HERE!\n");
  }
}

crt0(char *str)
{
  int argc = 0;
  printf("\n");
  argc = tokenize(str);

  main(argc, args);
}
