#include "utility.c"

int color;
int (*fptr[ ])() = {(int (*)()) ps, chname, kfork, switch, wait, exit };

main()
{
  char name[NAME_SIZE];
  int pid;

  while(1)
  {
    pid = getpid();
    color = 0x0C;

    printf("--------------------------------------------------\n");
    printf("Process [%d] in U-Mode.\n");
    printf("Running in segment #%x\n", pid, getcs());
/*
    show_menu();

    printf("U-Command :: ");
    gets(name);

    if(!name[0])
    {
      continue;
    }
    cmd = find_cmd(name);
    fptr[cmd]();
  }*/
}
