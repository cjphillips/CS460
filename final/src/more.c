#include "users/ucode.c"
#include "include/type.h"

#define LINES 24

print_page(int fd);

print_line(int fd);

int main(int argc, char *argv[])
{
  int fd, r;
  char cmd, tty[64];

  if (argc > 1)
  {
    fd = open(argv[1], RDONLY);
  }
  else
  {
    fd = dup(0);
    close(0);
    gettty(tty);
    open(tty, RDONLY);
  }

  if (fd < 0)
  {
    printf("\"%s\": could not open file.\n", argv[1]);
    return -1;
  }

  /* print an initial page */
  r = print_page(fd);
  /* while there is still more to read, ask for commands */
  while(r > 0)
  {
    cmd = getc();

    switch(cmd)
    {
      case 'q':
        putc('\n');
        r = -1;
        break;
      case '\n':
      case '\r':
        r = print_line(fd);
        break;
      case 'd':
      case 'f':
      case ' ':
        r = print_page(fd);
        break;
    }
  }

  close(fd);

  return 0;
}

int print_page(int fd)
{
  char *cp;
  int row = 0, r, i;

  while((r = read(fd, cp, 1)) > 0)
  {
    if (*cp == '\n')
    {
      row++;
    }

    putc(*cp);

    if (row >= LINES)
    {
      break;
    }
  }

  if (r <= 0)
  {
    return 0;
  }

  return 1;
}

int print_line(int fd)
{
  char *cp;
  int row = 0, r, i;

  while((r = read(fd, cp, 1)) > 0)
  {
    if (*cp == '\n')
    {
      putc(*cp);
      break;
    }

    putc(*cp);
  }

  if (r <= 0)
  {
    return 0;
  }

  return 1;
}
