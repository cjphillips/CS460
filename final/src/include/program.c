#define NAME_LENGTH  256
#define BUF_SIZE    1024

char *cmds[] = { "pwd", "exit", "cat" };

int menu()
{
  printf("<<-----------------------------------------------Menu--->>\n");
  printf(" pwd cat exit \n");
  printf("----------------------------------------------------------\n");
}

int findcmd(char *command)
{
  int i = 0;
  for(i = 0; cmds[i] != 0; i++)
  {
    if (strcmp(command, cmds[i]) == 0)
    {
      return i;
    }
  }

  return -1;
}

int _pwd()
{
  char dir[NAME_LENGTH];
  getcwd(dir);
  printf("%s\n", dir);

  return 0;
}

int _exit()
{
  exit(1);
}

int _exec(char *line)
{
  int pid, status;
  if ((pid = fork()) < 0)
  {
    printf("Could not fork process for \"cat\" command.\n");
    return -1;
  }

  if (pid)
  {
    pid = wait(&status);
  }
  else
  {
    exec(line);
  }
}
