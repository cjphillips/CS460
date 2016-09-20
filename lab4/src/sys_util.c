#include "include/type.h"

int get_word(u16 segment, u16 offset)
{
  u16 word;
  setds(segment);
  word = *(u16 *)offset;
  setds(0x1000);

  return word;
}

void put_word(u16 word, u16 segment, u16 offset)
{
  setds(segment);
  *(u16 *)offset = word;

  setds(0x1000);
}

void do_kfork()
{
  printf("FORKING\n");
}

void do_tswitch()
{
  printf("SWITCHING\n");
}

void do_wait()
{
  printf("WAITING\n");
}

void do_exit()
{
  printf("EXITING\n");
}
