#include "users/ucode.c"
#include "include/util.c"
#include "include/type.h"

int o_redir, i_redir;

int main(int argc, char *argv[])
{
  char *cp, buf[NAME_LENGTH], tty[64];
  int fd = 0, r;

  /* If a file has been given, open that file
     If no file name was given, read from stdin */
  gettty(tty);

  if (argc > 1)
  {
    fd = open(argv[1], O_RDONLY);
  }
  else
  {
    fd = 0;
  }

  if (fd < 0)
  {
    printf("\"%s\": could not open file.\n", argv[1]);
    return -1;
  }

  i_redir = get_is_input_redirected(tty);
  o_redir = get_is_output_redirected(tty);

  while ((r = get_line(fd, buf, i_redir, o_redir)) > 0)
  {
    write(1, buf, r);
  }

  close(fd);

  return 0;
}
