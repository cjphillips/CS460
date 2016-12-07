#include "users/ucode.c"
#include "include/type.h"

int read_file(int fd);

int read_cat(int fd);

int get_line(int fd, char *buf, int count);

int main(int argc, char *argv[])
{
  char *cp, buf[NAME_LENGTH];
  int fd = 0, r;

  /* If a file has been given, open that file
     If no file name was given, read from stdin */

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
/*
  while((r = read(fd, cp, 1)) > 0)
  {
    write(1, cp, 1);

    if (*cp == '\r')
    {
      printf("HER\n");
      write(1, '\n', 1);
    }

    if(*cp == '\n')
    {
      write(1, '\r', 1);
    }
  }*/

  while ((r = get_line(fd, buf, NAME_LENGTH)) > 0)
  {
    //putc('\r');
    //putc('\n');
    write(1, buf, r);
  }

  close(fd);

  return 0;
}

int get_line(int fd, char *buf, int count)
{
  int i = 0, r, total = 0;
  char *cp;

  while((r = read(fd, cp, 1)) > 0)
  {
    total++;
    //if (!fd)
    //{
    //  putc(*cp);
    //}

    if (*cp == '\n')
    {
      //if (!fd)
      //{
      //  putc('\n');
      //}
      buf[i++] = '\n';
      buf[i++] = '\r';

      total++;
      break;
    }

    buf[i] = *cp;
    i++;
  }

  buf[i] = 0;

  return i;
}

int get_line_file(int fd, char *buf, int count)
{
  char *cp, buf[NAME_LENGTH];
  int i = 0, r, count;

  printf("reading file!\n");

  while(1)
  //while((r = read(fd, cp, 1)) > 0)
  {
    gets(cp);
    count += 1;
    buf[i++] = *cp;

    if (*cp == '\r')
    {
      buf[i] = 0;
      i = 0;
      while(i < count)
      {
        putc(buf[i++]);
      }
      putc('\n');
      count = 0;
    }
  }

  return r;
}

int read_file(int fd)
{
  char *cp, buf[NAME_LENGTH];
  int i = 0, r, count;

  printf("reading file!\n");

  while((r = read(fd, cp, 1)) > 0)
  {
    count += r;
    buf[i++] = *cp;

    if (*cp == '\r')
    {
      buf[i] = 0;
      i = 0;
      while(i < count)
      {
        putc(buf[i++]);
      }
      putc('\n');
      count = 0;
    }
    /*
    while(*cp)
    {/*
      if (*cp == '\r')
      {
        putc('\n');
      }
      else
      {
        putc(*cp);
      }
      putc(*cp);
      *cp++;
    }

    for(i = 0; i < NAME_LENGTH; i++)
    {
      buf[i] = 0;
    }*/
  }

  return r;
}

int read_cat(int fd)
{
  char *cp, buf[NAME_LENGTH];
  int i = 0, r;

  while((r = read(fd, cp, 1)) > 0)
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
