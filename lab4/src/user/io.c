#include "include/user.h"

int BASE = 10;
char *table = "0123456789ABCDEF";

int rpu(u16 x) // Recursively print
{
  char c;
  if (x)
  {
    c = table[x % BASE]; // according to the base value, looks up from table
    rpu(x / BASE);
    syscall(8, c, color, 0);
  }
}

int rpul(long x)
{
  char c;
  if (x)
  {
    c = table[x % BASE];
    rpul(x / BASE);
    syscall(8, c, color, 0);
  }
}

int printu(u16 x)
{
  if (x==0)
    syscall(8, '0', color, 0);
  else
  {
    BASE = 10;
    rpu(x);
  }
  syscall(8, ' ', color, 0);
}

int printl(long x)
{
  if (x == 0)
  {
    syscall(8, '0', color, 0);
  }
  else
  {
    if (x < 0)
    {
      x *= -1;
      syscall(8, '-', color, 0);
    }
    BASE = 10;
    rpul(x);
  }
  syscall(8, ' ', color, 0);
}

int prints(char *s)
{
  while(*s) // loops through string and prints each character successively
  {
    syscall(8, *s, color, 0);
    s++;
  }
}

int printd(short x)
{
  if (x == 0)
  {
    syscall(8, '0', color, 0);
  }
  else
  {
    if(x < 0)
    {
      x *= -1; // flips the sign to print positive value
      syscall(8, '-', color, 0); // prints the negative value
    }
    BASE = 10; // Changes the BASE value to 10
    rpu((u16)x); // prints
  }
}

int printo(short x)
{
  if (x == 0)
  {
    syscall(8, '0', color, 0);
  }
  else
  {
      BASE = 8; // changes the BASE value to 8 for lookup table
      rpu((u16)x); // prints
  }
}

int printx(short x)
{
  prints("0x");
  if (x == 0)
  {
    syscall(8, '0', color, 0);
  }
  else
  {
    BASE = 16; // changes the BASE value to 16
    rpu((u16)x); // prints
  }
}

int printf(char *fmt, ...)
{
  char *cp = fmt; // point to the beginning of fmt
  int *ip = &fmt + 1; // point to the first arguement to be formatted
  long *ip2;

  while (*cp) // While the fmt string is still valid
  {
    switch(*cp)
    {
      case '%': // Case for a value to format
        cp++; // advance to successive char (for format type)
        if(*cp == 'c') // Printing single character
          syscall(8, *ip, color, 0);
        else if(*cp == 'd') // Printing decimal
          printd(*ip);
        else if(*cp == 'u') // Printing unsigned int
          printu(*ip);
        else if(*cp == 'o') // Printing octal
          printo(*ip);
        else if(*cp == 'x') // Printing hex
          printx(*ip);
        else if(*cp == 's') // Printing string
          prints(*ip);
      	else if(*cp == 'l')
        {
          ip2 = (long *)ip;
      	  printl(*ip2);
          ip++;
        }
        else // Otherwise, simple print out the format as a string
        {
          syscall(8, '%', color, 0);
          syscall(8, *cp, color, 0);
        }
        ip++; // Advance the value pointer to next position
        break;
      case '\n':
        prints("\n\r"); // Prints the newline + carriage return
        break;
      default:
        syscall(8, *cp, color, 0); // Simply places the character from fmt
        break;
      }
    cp++; // Move to next character in fmt
  }
}

int atoi(char *str)
{
  int result = 0;
  int i = 0;

  BASE = 10;

  while(str[i])
  {
    result = result * BASE + str[i] - '0';
    i++;
  }

  return result;
}

void gets(char *str)
{
  int i = 0;
  char c;

  while(1)
  {
    c = syscall(7, 0, 0, 0);
    printf("%c", c);
    if (c == '\r')
    {
      break;
    }

    str[i] = c;
    i++;
  }
  str[i] = '\0';
}
