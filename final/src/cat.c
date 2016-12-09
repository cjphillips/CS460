#include "users/ucode.c"
#include "include/type.h"

int get_line(int fd, char *buf, int count);

int get_input_redirected(char *tty);

int get_output_redirected(char *tty);

int o_redir, i_redir;

int main(int argc, char *argv[])
{
  char *cp, buf[NAME_LENGTH], tty[64];
  int fd = 0, r;

  /* If a file has been given, open that file
     If no file name was given, read from stdin */

  gettty(tty);

    //printf("HERE\n\n");

  if (argc > 1)
  {
    //printf("HERE! argv[1] = %s\n", argv[1]);
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
  //printf("fd = %d\n", fd);

  //remove_carriage = get_input_redirected(tty);
  i_redir = get_input_redirected(tty);
  o_redir = get_output_redirected(tty);
  //printf("ir = %d or = %d fd = %d\n", i_redir, o_redir, fd);


  //printf("\necho = %d remove_carriage = %d fd = %d\n", echo, remove_carriage, fd);

  while ((r = get_line(fd, buf, NAME_LENGTH)) > 0)
  {
    write(1, buf, r);
  }
    //printf("ir = %d or = %d fd = %d\n", i_redir, o_redir, fd);

    //printf("\necho = %d remove_carriage = %d fd = %d\n", echo, remove_carriage, fd);
  //get_input_redirected(tty);
  //get_output_redirected(tty);
  //printf("tty = %s\n", tty);
  //printf("\nremove_carriage = %d", remove_carriage);
  close(fd);

  return 0;
}

int get_line(int fd, char *buf, int count)
{
  int i = 0, r;
  char *cp, end_line;

  end_line = i_redir ? '\n' : '\r';
  if (fd)
  {
    end_line = '\n';
  }

  while((r = read(fd, cp, 1)) > 0)
  {
    if (*cp == end_line)
    {
      if (!i_redir && !fd)
      {
        //printf("HERE2\n");
        putc(*cp);
        putc('\n');
      }

      if (!o_redir)
      {
        buf[i++] = '\r';
      }

      buf[i++] = '\n';

      break;
    }
    else
    {
      if (!i_redir && !fd)
      {
        putc(*cp);
      }
    }

    buf[i++] = *cp;
  }

  buf[i] = 0;

  return i;
}

int get_input_redirected(char *tty)
{
  struct stat st_tty;
  struct stat file_s;

  stat(tty, &st_tty);
  fstat(0, &file_s);

  //printf("INPUT;\n");
  //printf("st_tty.st_dev = %d\n", st_tty.st_dev);
  //printf("st_tty.st_ino = %d\n", st_tty.st_ino);
  //printf("file_s.st_dev = %d\n", file_s.st_dev);
  //printf("file_s.st_ino = %d\n", file_s.st_ino);

  if (st_tty.st_dev != file_s.st_dev &&
      st_tty.st_ino != file_s.st_ino)
  {
    return 1;
  }

  return 0;
}

int get_output_redirected(char *tty)
{
  struct stat st_tty;
  struct stat file_s;

  stat(tty, &st_tty);
  fstat(1, &file_s);

  //printf("OUTPUT;\n");
  //printf("st_tty.st_dev = %d\n", st_tty.st_dev);
  //printf("st_tty.st_ino = %d\n", st_tty.st_ino);
  //printf("file_s.st_dev = %d\n", file_s.st_dev);
  //printf("file_s.st_ino = %d\n", file_s.st_ino);

  if (st_tty.st_dev != file_s.st_dev &&
      st_tty.st_ino != file_s.st_ino)
  {
    return 1;
  }

  return 0;
}
