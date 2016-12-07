#include "users/ucode.c"
#include "include/type.h"

int child;
int stdin, stdout, stderr;

main(int argc, char *argv[])
{
    stdin = open("/dev/tty0", RDONLY);
    stdout = open("/dev/tty0", WRONLY);
    stderr = open("/dev/tty0", ERROR);

    printf("[CJP_INIT]: Forking a login task on the console\n");

    child = fork();
    if (child)
    {
      parent();
    }
    else
    {
      exec("login /dev/tty0");
    }
}

int parent()
{
    int pid, status;
    while(1)
    {
        pid = wait(&status);

        if (pid == child)
        {
            child = fork();

            if (child)
            {
              continue;
            }
            else
            {
              exec("login /dev/tty0");
            }
        }
    }
}
