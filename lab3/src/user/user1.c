#include "include/user.h"
#include "include/util.h"

extern int getcs();

int color;
int (*fptr[ ])() = {ps, chname, kfork, kswitch, wait, exit, showMenu };

main()
{
  char name[NAMESIZE];
  int pid, cmd;
  pid = getpid();
  color = pid + 0x0A;
  while(1)
  {
    printf("--------------------------------------------\n");
    printf("[PROC %d] in segment %x.\n", pid, getcs());
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
