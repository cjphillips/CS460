#include "include/user.h"

char *cmds[] = { "ps", "chname", "kfork", "switch", "wait", "exit", "menu" };

int showMenu()
{
  printf("<<---------------------------------Menu--->>\n");
  printf(" ps, chname, kfork, switch, wait, exit, menu \n");
  printf("--------------------------------------------\n");
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
  int pid = getpid();

  printf("[PROC %d] New name: ", pid);
  gets(s);

  return syscall(2, s, 0);
}

int kfork()
{
  int child, pid;

  pid = getpid();
  printf("[PROC %d] Entering the kernel to fork a child.\n", pid);

  child = syscall(3, 0, 0);

  if (child >= 0)
  {
    printf("[PRPC %d] Forked child process [%d]\n", pid, child);
  }
}

int kswitch()
{
  return syscall(4, 0, 0);
}

int geti()
{
  char s[16];
  gets(s);

  return atoi(s);
}

int wait()
{
  int pid;
  int child, exitValue = -1;

  pid = getpid();

  printf("[PROC %d] Entering the kernel to wait for a child process to die.\n", pid);
  child = syscall(5, &exitValue, 0);

  printf("[PROC %d] Back from 'wait' call.\n", pid);

  if (child >= 0)
  {
    printf("[PROC %d] --> Found zombie process [%d].\n", pid, child);
    printf("[PROC %d] ----> Child process had an exit value of %d.\n", pid, exitValue);
  }
  else
  {
    printf("[PROC %d] --> No child processes.\n", pid);
    return -1;
  }
}

int exit()
{
  int exitValue;
  int pid = getpid();

  printf("[PROC %d] Enter an exit value: ", pid);
  exitValue = geti();

  printf("[PROC %d] Entering kernel to exit with a value of %d.\n", pid, exitValue);

  _exit(exitValue);
}

int _exit(int exitValue)
{
  return syscall(6, exitValue, 0);
}
