int get_line(int fd, char *buf, int i_redir, int o_redir)
{
  int i = 0, r;
  char *cp;

  while((r = read(fd, cp, 1)) > 0)
  {
    if (fd)
    {
      if (*cp == '\n')
      {
        buf[i++] = '\n';

        if (!o_redir)
        {
          buf[i++] = '\r';
        }

        break;
      }
    }
    else
    {
      if (*cp == '\r' || *cp == '\n')
      {
        if (!i_redir)
        {
          write(2, "\r", 1);
          write(2, "\n", 1);
        }

        if (!o_redir)
        {
          buf[i++] = '\r';
        }

        buf[i++] = '\n';
        break;
      }

      if (!i_redir)
      {
        write(2, cp, 1);
      }
    }

    buf[i++] = *cp;
  }

  buf[i] = 0;

  return i;
}

int get_is_input_redirected(char *tty)
{
  struct stat st_tty;
  struct stat file_s0;

  stat(tty, &st_tty);
  fstat(0, &file_s0);

  if (st_tty.st_dev != file_s0.st_dev ||
      st_tty.st_ino != file_s0.st_ino)
  {
    return 1;
  }

  return 0;
}

int get_is_output_redirected(char *tty)
{
  struct stat st_tty;
  struct stat file_s1;

  stat(tty, &st_tty);
  fstat(1, &file_s1);

  if (st_tty.st_dev != file_s1.st_dev ||
      st_tty.st_ino != file_s1.st_ino)
  {
    return 1;
  }

  return 0;
}
