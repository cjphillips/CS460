#include "include/program.c"
#include "users/ucode.c"

char *user, *home, *command, input[NAME_LENGTH], cwd[NAME_LENGTH];
int uid, gid;

int handle(char *line, int *pd);

int run(char *line);

int scan(char *line, char *head, char *tail);


int main(int argc, char *argv[])
{
  int i, argcount;
  char linefull[NAME_LENGTH];
  user = argv[1];
  uid = atoi(argv[2]);
  gid = atoi(argv[3]);
  home = argv[4];

  printf("-----------------------------------------------------------\n");
  printf("  Welcome back %s!\n\n", user);
  printf("  Home ....... %s\n", home);
  printf("  UID ........ %d\n", uid);
  printf("  GID ........ %d\n", gid);
  printf("-----------------------------------------------------------\n");

  while(1)
  {
    getcwd(cwd);
    printf("%s [%s] $ ", user, cwd);

    gets(input);
    handle(input);
  }

  return 0;
}

int handle(char *line, int *pd)
{
  char orig[NAME_LENGTH], head[NAME_LENGTH], tail[NAME_LENGTH];
  int haspipe, lpd[2], pid;

  if (pd)
  {
    /* A pipe has been passed in */
    close(pd[0]);
    dup2(pd[1], 1);
    close(pd[1]);
  }

  strcpy(orig, line);
  haspipe = scan(line, head, tail);

  if (haspipe)
  {
    pipe(lpd);
    pid = fork();
    if (pid)
    {
      /* parent reads */
      close(lpd[1]);
      dup2(lpd[0], 0);
      close(lpd[0]);
      run(tail);
    }
    else
    {
      /* Child writes. There may be more piping involved! */
      handle(head, lpd);
    }
  }
  else
  {
    run(orig);
  }
}

int run(char *line)
{
  char *token, *command, orig[NAME_LENGTH], args[64][NAME_LENGTH];
  int i, argcount, handle;

  strcpy(orig, line);

  command = strtok(line, " ");

  i = argcount = 0;
  token = strtok(0, " ");
  while(token)
  {
    strcpy(args[i++], token);
    //strcpy(out[i], token);
    token = strtok(0, " ");
    argcount++;
  }

  handle = findcmd(command);

  /*if (handle < 0)
  {
    printf("Invalid input: \"%s\" not found.\n", cmd);
  }*/
  //else
  {
    switch(handle)
    {
      case 0:  _pwd();       break;
      case 1:  _exit();      break;
      default: _exec(orig);  break;
    }
  }
}

int scan(char *line, char *head, char *tail)
{
  int i, pipe_index = -1;
  for(i = 0; line[i]; i++)
  {
    if (line[i] == '|')
    {
      /* Get the right-most pipe (|) index */
      pipe_index = i;
    }
  }

  if (pipe_index < 0)
  {
    head = line;
    tail = 0;
  }
  else
  {
    for(i = 0; i < pipe_index; i++)
    {
      head[i] = line[i];
    }

    i = (head[i] == ' ') ? i - 1 : i;
    head[i] = 0; /* trim the end */

    pipe_index++;
    if (line[pipe_index] == ' ') pipe_index++; /* trim the beginning */

    for(; line[i]; i++)
    {
      tail[i - pipe_index] = line[i];
    }
    tail[i - pipe_index] = 0;
  }

  return tail ? 1 : 0;
}
