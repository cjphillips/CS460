#include "include/user.h"
#include "include/util.h"

extern int getcs();

int color;
int (*fptr[ ])() = {ps, chname, kfork, kswitch, wait, exit, showMenu };

main()
{
  char name[NAMESIZE];
  int pid, cmd;

  while(1)
  {
    pid = getpid();
    color = 0x0C;

    printf("--------------------------------------------\n");
    printf("Process [%d] in U-Mode.\n");
    printf("Running in segment %x\n", pid, getcs());
    printf("--------------------------------------------\n");

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
