#include "include/user.h"
#include "include/util.h"
#include "include/io.h"

int (*fptr[ ])() = {ps, chname, fork, exec, kswitch, wait, exit, showMenu };
char *cmds[10];

main(int argc, char *argv[])
{
  char name[NAMESIZE];
  int pid, cmd;
  pid = getpid();
  color = pid + 0x04;
  while(1)
  {
    printf("------------------------------------------------------\n");
    printf("[PROC %d - USER ONE CODE] in segment %x.\n", pid, getcs());
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
  int i = 0, argc;
  char *arg;

  if (str)
  {
    printf("str is not empty, str = %d\n", str);
  }
  else
  {
    printf("str is def empty\n");
  }

  printf("HERE!\n");

  arg = strtok(str, ' ');

  printf("AFTER FIRST STRTOK with arg=%s\n", arg);

  while(arg)
  {
    cmds[i] = arg;
    arg = strtok(0, ' ');
    i++;
  }

  argc = i;

  /* Finish initializing the cmd vector */
  while(i < 10) { cmds[i] = 0; }

  main(argc, cmds);
}
