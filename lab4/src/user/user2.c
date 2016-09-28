#include "include/user.h"
#include "include/util.h"
#include "include/io.h"

int (*fptr[ ])() = {ps, chname, fork, exec, kswitch, wait, exit, showMenu };

main(int argc, char *argv[])
{
  char name[NAMESIZE];
  int pid, cmd;
  pid = getpid();
  color = pid + 0x04;
  while(1)
  {
    printf("------------------------------------------------------\n");
    printf("[PROC %d - USER TWO CODE] in segment %x.\n", pid, getcs());
    printf("------------------------------------------------------\n");

    showMenu();
    do {
      printf("> ");
      gets(name);

      if(!name[0])
      {
        continue;
      }

      cmd = findCmd(name);
      if (cmd >= 0)
      {
        fptr[cmd]();
      }
      else
      {
        printf("\n\'%s\': command not recognized.\n", name);
      }
    } while(1);
  }
}

main0(char *str)
{
  main(0, 0);
}
