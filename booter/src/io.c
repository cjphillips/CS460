#include "type.h"

int BASE = 10;
char *table = "0123456789ABCDEF";

int rpu(u16 x) // Recursively print
{
  char c;
  if (x)
  {
    c = table[x % BASE]; // according to the base value, looks up from table
    rpu(x / BASE);
    putc(c);
  }
}

int rpul(long x)
{
  char c;
  if (x)
  {
    c = table[x % BASE];
    rpul(x / BASE);
    putc(c);
  }
}

int printu(u16 x)
{
  if (x==0)
    putc('0');
  else
  {
    BASE = 10;
    rpu(x);
  }
  putc(' ');
}

int printl(long x)
{
  if (x == 0)
  {
    putc('0');
  }
  else
  {
    if (x < 0)
    {
      x *= -1;
      putc('-');
    }
    BASE = 10;
    rpul(x);
  }
  putc(' ');
}

int prints(char *s)
{
  while(*s) // loops through string and prints each character successively
  {
    putc(*s);
    s++;
  }
}

int printd(short x)
{
  if (x == 0)
  {
    putc('0');
  }
  else
  {
    if(x < 0)
    {
      x *= -1; // flips the sign to print positive value
      putc('-'); // prints the negative value
    }
    BASE = 10; // Changes the BASE value to 10
    rpu((u16)x); // prints
  }
}

int printo(short x)
{
  if (x == 0)
  {
    putc('0');
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
    putc('0');
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
          putc(*ip);
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
          putc('%');
          putc(*cp);
        }
        ip++; // Advance the value pointer to next position
        break;
      case '\n':
        prints("\n\r"); // Prints the newline + carriage return
        break;
      default:
        putc(*cp); // Simply places the character from fmt
        break;
      }
    cp++; // Move to next character in fmt
  }
}

void gets(char *str)
{
  int i = 0;
  char c;

  while(1)
  {
    c = getc();
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

int strcmp(char *left, char *right)
{
  int i = 0;

  while(left[i])
  {
    if (!right[i] || left[i] > right[i])
    {
      return 1;
    }
    if (left[i] < right[i])
    {
      return -1;
    }

    i++;
  }

  if (right[i])
  {
    return -1;
  }

  return 0;
}
