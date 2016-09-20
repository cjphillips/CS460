#include "include/user.h"

char *cmds[] = { "ps", "chname", "kfork", "switch", "wait", "exit" };

int show_menu()
{
  printf("////////////////////////---Menu---/////\n");
  printf(" ps, chname, kfork, switch, wait, exit \n");
  printf("///////////////////////////////////////\n");
}

int findCmd(char *command)
{
  int i = 0;

  for(i = 0; cmds[i] != 0; i++)
  {
    if (strcmp(command, cmds[i]) == 0) {
      return i;
    }
  }

  return -1;
}

int getpid()
{
  return syscall(0, 0, 0);
}

int ps()
{
  return syscall(1, 0, 0);
}

int chname()
{
  char s[NAMESIZE];

  printf("New name: ");
  gets(s);
  return syscall(2, s, 0);
}

int kfork()
{
  int child, pid;

  pid = getpid();
  printf("Process [%d] entering the kernel to fork a child.\n", pid);

  child = syscall(3, 0, 0);

  printf("Process [%d] forked child process [%d]\n", pid, child);
}

int kswitch()
{
  return syscall(4, 0, 0);
}

int geti()
{
  int intValue;

  char c = getc();
  intValue = atoi(c);

  return intValue;
}

int wait()
{
  int child, exitValue;

  printf("Process [%d] entering the kernel to wait for a child process to die.\n", getpid());
  child = syscall(5, &exitValue, 0);

  printf("Process [%d] back from 'wait' call.\n", getpid());
  printf("--> Found zombie process [%d].\n", child);

  if (child >= 0)
  {
    printf("----> Child process had an exit value of %d.\n", exitValue);
  }
}

int exit()
{
  int exitValue;

  printf("Enter an exit value: ");
  exitValue = geti();

  printf("Entering kernel to die with an exit value of %d.\n", exitValue);

  _exit(exitValue);
}

int _exit(int exitValue)
{
  return syscall(6, exitValue, 0);
}
