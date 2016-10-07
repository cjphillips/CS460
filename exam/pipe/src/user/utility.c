#include "include/user.h"

char *cmds[] = { "ps", "chname", "fork", "exec", "switch", "wait", "exit", "menu",
                  "pfd", "pipe", "close", "write", "read" };

int pd[2];

int showMenu()
{
  printf("<<---------------------------------------Menu--->>\n");
  printf(" ps, chname, fork, exec, switch, wait, exit, menu \n");
  printf(" pfd, pipe, write, read \n");
  printf("--------------------------------------------------\n");
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

int tokenize(char *s)
{
  int i = 0;

  while(*s)
  {
    while(*s == ' ')
    {
      *s++ = 0;
    }
    if (*s)
    {
      args[i++] = s;
    }
    while(*s && *s != ' ')
    {
      s++;
    }

    if (*s)
    {
      *s = 0;
      s++;
    }
    else
    {
      break;
    }
  }
  args[i] = 0;

  return i;
}

int getpid()
{
  return syscall(0, 0, 0);
}

int ps()
{
  return syscall(1, 0, 0);
}

int pfd()
{
  return syscall(10, 0, 0);
}

int chname()
{
  char s[NAMESIZE];
  int pid = getpid();

  printf("\n[PROC %d] New name: ", pid);
  gets(s);

  return syscall(2, s, 0);
}

int exec()
{
  int r;
  char filename[FILE_LENGTH];

  printf("\nEnter command: ");
  gets(filename);

  r = syscall(9, filename, 0, 0);

  printf("Error: \"exec %s\" failed.\n", filename);
}

int fork()
{
  int child, pid;

  pid = getpid();
  printf("[PROC %d] Entering the kernel to fork a child.\n", pid);

  child = syscall(3, 0, 0);

  if (child > 0)
  {
    printf("[PROC %d] Forked child process [%d]\n", pid, child);
  }
}

int close()
{
  int fd, pid = getpid();

  printf("\n[PROC %d] FD to close: ");
  fd = geti();

  syscall(14, fd, 0, 0);
}

int write()
{
  int fd, length, pid;
  char text[BUFSIZE];
  pid = getpid();

  printf("\n[PROC %d] Write FD: ", pid);
  fd = geti();

  printf("\n[PROC %d] Text: ", pid);
  gets(text);

  length = strlen(text);

  syscall(12, fd, text, length);
}

int read()
{
  int fd, count, pid, read, i;
  char text[BUFSIZE];
  pid = getpid();

  printf("\n[PROC %d] Read FD: ", pid);
  fd = geti();

  printf("\n[PROC %d] Nr. Bytes to read: ", pid);
  count = geti();

  if (count > BUFSIZE)
  {
    printf("Count of [%d] is too large for the read buffer.\n", count);
    return -1;
  }

  read = syscall(13, fd, text, count);

  if (read > 0)
  {
    printf("[PROC %d] From pipe: ", pid);
    for(i = 0; i < read; i++)
    {
      putc(text[i]);
    }
    printf("\n");
  }
}

int pipe()
{
  syscall(11, pd);
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

  printf("\n[PROC %d] Enter an exit value: ", pid);
  exitValue = geti();

  printf("[PROC %d] Entering kernel to exit with a value of %d.\n", pid, exitValue);

  _exit(exitValue);
}

int _exit(int exitValue)
{
  return syscall(6, exitValue, 0, 0);
}
