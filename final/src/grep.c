#include "users/ucode.c"
#include "include/type.h"

int check_pattern(char *str);

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
  while((r = read(fd, cp, 1)) > 0)
  {
    line[i++] = *cp;

    if (*cp == '\n')
    {
      /* An entire line has been read, check it for the pattern */
      line[i-1] = 0;
      check_pattern(line, argv[1]);

      for(i = 0; i < NAME_LENGTH; i++)
      {
        line[i] = 0;
      }
      i = 0;
    }
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
