#include "users/ucode.c"
#include "include/type.h"

char *tty;
int stdin, stdout, stderr;

char username[NAME_LENGTH], password[NAME_LENGTH], cwd[NAME_LENGTH], fullName[NAME_LENGTH];
char init_program[NAME_LENGTH], execline[BUFSIZE];
int uid, gid;

int validate(char *username, char *password);

int main(int argc, char *argv[])
{
  tty = argv[1];

  /* Make sure that this process is independent of other processes (close just in case)*/
  close(0);
  close(1);
  close(2);

  /* Open all streams on this tty */
  stdin = open(tty, RDONLY);
  stdin = open(tty, WRONLY);
  stdin = open(tty, ERROR);

  /* Store the tty string in PROC.tty[] for putc() */
  settty(tty);

  /* Since stdout is set, printf (which calls puts()) is available to this tty */
  printf("[CJP_LOGIN]: %s opened for stdin.\n", tty);
  printf("[CJP_LOGIN]: %s opened for stdout.\n", tty);
  printf("[CJP_LOGIN]: %s opened for stderr.\n", tty);

  /* Ignore Control-C interrupts so that it instead kills other procs on this tty but not the main shell */
  signal(2, 1);

  while(1)
  {
    printf("login: ");
    gets(username);

    printf("password: ");
    gets(password);

    if (validate(username, password) > 0)
    {
      chuid(uid, gid);
      chdir(cwd);

      sprintf(execline, "%s %s %d %d %s",
        init_program,
        username,
        uid,
        gid,
        cwd);

      printf("LINE: %s\n", execline);

      exec(execline);
    }

    printf("Invalid login ... try again.\n");
  }

  return 0;
}

int validate(char *username, char *password)
{
  char buf[BUFSIZE], *token;
  int found = -1, passwd = open("/etc/passwd", RDONLY);

  if (!passwd)
  {
    printf("Unable to locate '/etc/passwd'.\n");
    return -1;
  }

  read(passwd, buf, BUFSIZE);

  token = strtok(buf, ":\n");
  while(token)
  {
    /* See if the token matches the given name */
    if (strcmp(token, username) == 0)
    {
      /* See if the corresponding username is valid with the given password */
      token = strtok(0, ":\n");

      if (strcmp(token, password) == 0)
      {
        /* The provided username and password match an entry */

        /* Set the user's ID */
        token = strtok(0, ":\n");
        if (token) uid = atoi(token);

        /* Set the user's Group ID */
        token = strtok(0, ":\n");
        if (token) gid = atoi(token);

        /* Get the user's full name */
        token = strtok(0, ":\n");
        if (token) strcpy(fullName, token);

        /* Get the user's cwd (home directory) */
        token = strtok(0, ":\n");
        if (token) strcpy(cwd, token);

        /* Get the user's initial program to execute */
        token = strtok(0, ":\n");
        if (token) strcpy(init_program, token);

        found = 1;
      }
    }

    /* This user was not found to fulfill the user and password given. Try the next! */
    token = strtok(0, ":\n");
  }

  close(passwd);
  return found;
}
