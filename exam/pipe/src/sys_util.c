#include "include/type.h"

pfd()
{
  int i, count = 0;

  printf("-------------------------------FDs-------\n");
  printf(" FD           Ref. Count          Mode\n");
  for(i = 0; i < NFD; i++)
  {
    if (running->fd[i]->refCount > 0)
    {
      count++;
      printf(" %d               %d             ", i, running->fd[i]->refCount);
      switch(running->fd[i]->mode)
      {
        case READ_PIPE:  printf("Read\n");  break;
        case WRITE_PIPE: printf("Write\n"); break;
      }
    }
  }
  if (count == 0) printf(" No files open.\n");
  printf("-----------------------------------------\n");
}

/*
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
}*/

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
