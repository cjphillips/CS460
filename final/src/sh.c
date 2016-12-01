#include "include/program.c"
#include "users/ucode.c"

char *user, *home, *command, input[NAME_LENGTH], cwd[NAME_LENGTH];
int uid, gid;

int handleCommand(char *cmd, char *args[]);

int main(int argc, char *argv[])
{
  int i, argcount;
  char *token, *command, args[64][NAME_LENGTH], input[NAME_LENGTH], linefull[NAME_LENGTH];
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

    strcpy(linefull, input);
    command = strtok(input, " ");

    i = argcount = 0;
    token = strtok(0, " ");
    while(token)
    {
      strcpy(args[i++], token);
      //strcpy(out[i], token);
      token = strtok(0, " ");
      argcount++;
    }

    handleCommand(command, args, linefull);
  }

  return 0;
}

int handleCommand(char *cmd, char *args[], char *input)
{
  int handle = findcmd(cmd);

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
      default: _exec(cmd, args, input);  break;
    }
  }
}
