#include "users/ucode.c"
#include "include/type.h"

int read_file(int fd);

int read_stdin();

int main(int argc, char *argv[])
{
  char *cp, buf[NAME_LENGTH];
  int fd = 0, r, i = 0;

  /* If a file has been given, open that file
     If no file name was given, read from stdin */
  if (argc > 1)
  {
    if ((fd = open(argv[1], RDONLY)) < 0)
    {
      printf("Could not open \"%s\": does the file exist?\n", argv[1]);
      exit(1);
    }
  }

  r = fd ? read_file(fd) : read_stdin();

  if (r < 0)
  {
    printf("Could not read from \"%s\".\n", argv[1]);
    exit(2);
  }

  close(fd);
  exit(0);
}

int read_file(int fd)
{
  char *cp, buf[NAME_LENGTH];
  int i = 0, r;

  while((r = read(fd, buf, NAME_LENGTH)) > 0)
  {
    buf[r] = 0;
    cp = buf;

    while(*cp)
    {
      if (*cp == '\r')
      {
        putc('\n');
        putc('\r');
      }
      else
      {
        putc(*cp);
      }

      *cp++;
    }

    for(i = 0; i < NAME_LENGTH; i++)
    {
      buf[i] = 0;
    }
  }

  return r;
}

int read_stdin()
{
  char *cp, buf[NAME_LENGTH];
  int i = 0, r;

  while((r = read(0, cp, 1)) > 0)
  {
    buf[i++] = *cp;
    putc(*cp);
    if (*cp == '\r')
    {
      buf[i] = 0;
      putc('\n');
      printf("%s\n", buf);

      for(i = 0; i < NAME_LENGTH; i++)
      {
        buf[i] = 0;
      }

      i = 0;
    }
  }

  return r;
}
