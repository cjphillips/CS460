#include "include/user.h"
#include "include/util.h"
#include "include/io.h"

int (*fptr[ ])() = {ps, chname, fork, exec, kswitch, wait, exit, showMenu };

main(int argc, char *argv[])
{
  char name[NAMESIZE];
  int pid, cmd, i = 0, *temp;

  while(1)
  {
    pid = getpid();
    color = pid + 0x04;
    printf("------------------------------------------------------\n");
    printf("[PROC %d - USER TWO CODE] in segment %x.\n", pid, getcs());
    printf(" --> argc = %d\n", argc);
    while(i < argc)
    {
      printf(" --> [%d] %s\n", i, argv[i]);
      i++;
    }
    printf("------------------------------------------------------\n");

    do {
      showMenu();
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

crt0(char *str)
{
  int argc = 0;
  printf("\n");
  argc = tokenize(str);

  main(argc, args);
}
