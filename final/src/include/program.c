#define NAME_LENGTH  256
#define BUF_SIZE    1024

char *cmds[] = { "pwd", "cd", "exit" };

int menu()
{
  printf("<<-----------------------------------------------Menu--->>\n");
  printf(" pwd cat exit \n");
  printf("----------------------------------------------------------\n");
}

int findcmd(char *command)
{
  int i = 0;
  char buf[NAME_LENGTH];

  for(i = 0; command[i] && command[i] != ' '; i++)
  {
    buf[i] = command[i];
  }
  buf[i] = 0;

  for(i = 0; cmds[i] != 0; i++)
  {
    if (strcmp(buf, cmds[i]) == 0)
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

int _cd(char *line, char *home)
{
  int i = 0;
  char dir[NAME_LENGTH];
  while(*line++ != ' ');

  while(*line && *line != ' ')
  {
    dir[i++] = *line++;
  }
  dir[i] = 0;

  if (dir && strlen(dir))
  {
    if (chdir(dir) < 0)
    {
      printf("\"%s\": not a valid directory.\n", dir);
    }
  }
  else
  {
    chdir(home);
  }
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
