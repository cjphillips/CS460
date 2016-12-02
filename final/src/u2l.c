#include "users/ucode.c"
#include "include/type.h"

void read_stdin(int gd);

void read_file(int fd, int gd);

int main(int argc, char *argv[])
{
  int fd, gd, i = 0, r;
  char buf[BUFSIZE], out[BUFSIZE];

  if (argc > 2)
  {
    /* Read from file, output to second file */
    fd = open(argv[1], RDONLY);
    gd = open(argv[2], WRONLY);
  }
  else if (argc > 1)
  {
    printf("HERE!\n");
    /* Read from file, output to stdout */
    fd = open(argv[1], RDONLY);
    gd = 1;
  }
  else
  {
    /* Read from stdin, output to stdout */
    fd = 0;
    gd = 1;
  }

  if (fd < 0)
  {
    printf("\"%s\": cannot open file.", argv[1]);
    return -1;
  }
  if (gd < 0)
  {
    printf("\"%s\": cannot open file.", argv[2]);
    return -1;
  }

  fd == 0 ? read_stdin(gd) : read_file(fd, gd);

  close(fd);
  close(gd);

  return 0;
}

void read_stdin(int gd)
{
  char *cp, buf[BUFSIZE];
  int r, i;

  while((r = read(0, cp, 1)) > 0)
  {
    buf[i] = *cp;

    if (buf[i] >= 65 && buf[i] <= 90)
    {
      buf[i] = (char)(buf[i] + 0x20);
    }
    i++;
    putc(*cp);
    if (*cp == '\r')
    {
      buf[i] = 0;
      putc('\n');

      gd == 1 ? printf("%s\n", buf) : write(gd, buf, r);

      for(i = 0; i < BUFSIZE; i++)
      {
        buf[i] = 0;
      }

      i = 0;
    }
  }
}

void read_file(int fd, int gd)
{
  char *cp, buf[BUFSIZE];
  int i = 0, r;

  while((r = read(fd, buf, BUFSIZE)) > 0)
  {
    buf[r] = 0;
    cp = buf;

    while(*cp)
    {
      if (*cp == '\r')
      {
        gd == 1 ? putc('\n') : write(gd, '\n', 1);
        gd == 1 ? putc('\r') : write(gd, '\r', 1);
      }
      else
      {
        if (*cp >= 65 && *cp <= 90)
        {
          *cp = (char)(*cp + 0x20);
        }

        gd == 1 ? putc(*cp) : write(gd, *cp, 1);
      }

      *cp++;
    }
  }
}
