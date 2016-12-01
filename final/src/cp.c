#include "users/ucode.c"
#include "include/type.h"

int main(int argc, char *argv[])
{
  int fd, gd, n;
  char buf[BUFSIZE];

  if (argc < 3)
  {
    printf("\"cp\": invalid number of arguments.\n");
    return -1;
  }

  if ((fd = open(argv[1], RDONLY)) < 0)
  {
    printf("Could not open \"%s\".", argv[1]);
    return -2;
  }

  if ((gd = open(argv[2], WRONLY)) < 0)
  {
    printf("Could not open \"%s\".", argv[2]);
    return -3;
  }

  while((n = read(fd, buf, BUFSIZE)) > 0)
  {
    if (n < 0)
    {
      break;
    }

    write(gd, buf, n);
  }

  close(fd);
  close(gd);

  return 0;
}
