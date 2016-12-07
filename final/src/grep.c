#include "users/ucode.c"
#include "include/type.h"

int check_pattern(char *line, char *pattern);

int get_line(int fd, char *buf, int count);

int main(int argc, char *argv[])
{
  int fd, i, r;
  char line[NAME_LENGTH], *cp;

  if (argc > 2)
  {
    fd = open(argv[2], O_RDONLY);
  }
  else if (argc > 1)
  {
    fd = 0;
  }
  else
  {
    printf("Usage: grep pattern [filename]\n");
    return -1;
  }

  if (fd < 0)
  {
    printf("\"%s\": could not open file.\n", argv[2]);
    return -1;
  }

  i = 0;
  while((r = get_line(fd, line, NAME_LENGTH)) > 0)
  {
    check_pattern(line, argv[1]);
  }

  if (i > 0)
  {
    /* All characters may have been read but not checked. */
    line[i] = 0;
    check_pattern(line, argv[1]);
  }

  close(fd);
}

int check_pattern(char *line, char *pattern)
{
  char *token, pieces[NAME_LENGTH][64], orig[NAME_LENGTH];
  int count = 0, i;

  //printf("LINE: %s\n", line);

  strcpy(orig, line);

  token = strtok(line, " ");
  while(token)
  {
    /* build the string array */
    strcpy(pieces[count++], token);
    token = strtok(0, " ");
  }

  for(i = 0; i < count; i++)
  {
    /* determine if any of the strings match the given pattern */
    if (strcmp(pieces[i], pattern) == 0)
    {
      /* The pattern has been found in the line -> print the whole line */
      printf("%s\n", orig);
      return count;
    }
  }

  return 0;
}

int get_line(int fd, char *buf, int count)
{
  int i = 0, r, total = 0;
  char *cp;

  while((r = read(fd, cp, 1)) > 0)
  {
    total++;

    if (*cp == '\n')
    {
      break;
    }

    buf[i] = *cp;
    i++;
  }

  buf[i] = 0;

  return i;
}
