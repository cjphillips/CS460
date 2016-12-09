#include "users/ucode.c"
#include "include/util.c"
#include "include/type.h"

int i_redir, o_redir;

int main(int argc, char *argv[])
{
  int fd, gd, i = 0, r;
  char buf[BUFSIZE], tty[64], *cp;

  if (argc > 2)
  {
    /* Read from file, output to second file */
    fd = open(argv[1], RDONLY);
    gd = open(argv[2], WRONLY);
  }
  else if (argc > 1)
  {
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

  gettty(tty);

  i_redir = get_is_input_redirected(tty);
  o_redir = get_is_output_redirected(tty);

  while ((r = get_line(fd, buf, i_redir, o_redir)) > 0)
  {
    cp = buf;
    while(*cp)
    {
      if (*cp >= 0x41 && *cp <= 0x5A)
      {
        *cp = (char) (*cp + 0x20);
      }

      write(gd, cp, 1);
      *cp++;
    }
  }

  close(fd);
  close(gd);

  return 0;
}
