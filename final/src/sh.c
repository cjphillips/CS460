#include "include/program.c"
#include "users/ucode.c"

char *user, *home, *command, cwd[NAME_LENGTH];
int uid, gid;

int handle(char *line, int *pd);

int run(char *line);

int scan_pipe(char *line, char *head, char *tail);

int scan_redirect(char *line, char *head, char *tail);

int redirect(char *file, int type);


int main(int argc, char *argv[])
{
  char input[NAME_LENGTH];
  int pid, status;
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

    if (strlen(input) > 0)
    {
      if (strcmp(input, "exit") == 0)
      {
        exit(0);
      }
      else if (input[0] == 'c' && input[1] == 'd')
      {
        _cd(input, home);
      }
      else
      {
        pid = fork();

        if (pid)
        {
          pid = wait(&status);
        }
        else
        {
          handle(input, 0);
        }
      }
    }
  }

  return 0;
}

int handle(char *line, int *pd)
{
  char head[NAME_LENGTH], tail[NAME_LENGTH];
  int haspipe, lpd[2], pid;

  if (pd)
  {
    /* A pipe has been passed in */
    close(pd[0]);
    dup2(pd[1], 1);
    close(pd[1]);
  }

  haspipe = scan_pipe(line, head, tail);

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
    run(head);
  }
}

int run(char *line)
{
  char bin[NAME_LENGTH], file[NAME_LENGTH];
  int i, argcount, handle, hasredirects;

  hasredirects = scan_redirect(line, bin, file);

  if (hasredirects)
  {
    redirect(file, hasredirects);
  }

  handle = findcmd(bin);

  /*if (handle < 0)
  {
    printf("Invalid input: \"%s\" not found.\n", cmd);
  }*/
  //else
  {
    switch(handle)
    {
      case 0:  _pwd();       break;
      case 1:  _cd(line, home);    break;
      default: exec(bin);  break;
    }
  }
}

int save()
{/*
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

  printf("In run: command = %s\n", command);*/
}

int redirect(char *file, int type)
{
  switch(type)
  {
    case 1: /* '<' ... redirect input */
      close(0);
      open(file, O_RDONLY);
      break;
    case 2: /* '>' ... redirect output */
      close(1);
      open(file, O_WRONLY | O_CREAT, 0777);
      break;
    case 3: /* '>>' ... redirect output with appending */
      close(1);
      open(file, O_WRONLY | O_CREAT | O_APPEND, 0777);
      break;
    default:
      printf("Unrecognized redirect symbol in command line.\n");
      return -1;
  }

  return 0;
}

int scan_pipe(char *line, char *head, char *tail)
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
    /* No pipe was found */
    for(i = 0; line[i]; i++)
    {
      head[i] = line[i];
    }
    head[i] = 0;
    tail = 0;
  }
  else
  {
    for(i = 0; i < pipe_index; i++)
    {
      head[i] = line[i];
    }

    i = (head[i - 1] == ' ') ? i - 1 : i;
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

int scan_redirect(char *line, char *bin, char *file)
{
  int i, r_index = -1, type = 0;

  for(i = 0; line[i]; i++)
  {
    if (line[i] == '<')
    {
      /* Get the right-most redirect symbol index */
      r_index = i;
      type = 1;
      break;
    }
    if (line[i] == '>')
    {
      r_index = i;
      type = line[i + 1] == '>' ? 3 : 2;
      break;
    }
  }

  if (r_index < 0)
  {
    /* No redirect was found */
    for(i = 0; line[i]; i++)
    {
      bin[i] = line[i];
    }
    bin[i] = 0;
    file = 0;
  }
  else
  {
    for(i = 0; i < r_index; i++)
    {
      bin[i] = line[i];
    }

    i = (bin[i - 1] == ' ') ? i - 1 : i;
    bin[i] = 0; /* trim the end */

    r_index = (type == 3) ? r_index + 2 : r_index + 1;

    if (line[r_index] == ' ') r_index++; /* trim the beginning */

    for(; line[i]; i++)
    {
      file[i - r_index] = line[i];
    }
    file[i - r_index] = 0;
  }

  return type;
}
