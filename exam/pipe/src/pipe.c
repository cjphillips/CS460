#include "include/type.h"
#include "include/wait.h"

int show_pipe(PIPE *p)
{
  int i, j;
  printf("Head: %d   Tail: %d   Data: %d   Room: %d\n",
    p->head,
    p->tail,
    p->data,
    p->room);

  printf("<---------------------------------PIPE CONTENTS---->\n");
  for(i = 0, j = p->tail; i < p->data; i++, j++)
  {
    if (j == PSIZE)
    {
      j = 0;
    }
    putc(p->buf[j]);
  }
  printf("\n<-------------------------------------------------->\n");
}

int _readBytes(PIPE *p, char *buf, int count)
{
  int read = 0;

  while(read < count && p->data != 0)
  {
    put_byte(p->buf[p->tail], running->uss, buf + read);
    (p->data)--;
    (p->tail)++;
    (p->room)++;
    read++;
    if (p->tail == PSIZE)
    {
      /* Reset the circular link */
      p->tail = 0;
    }
  }

  return read;
}

int read_pipe(int fd, char *buf, int count)
{
  int read = 0;
  PIPE *p;

  if (fd < 0 || fd >= NFD)
  {
    printf("[KERNEL] Invalid file descriptor (%d).\n", fd);
    return -1;
  }

  if (!running->fd[fd])
  {
    printf("[KERNEL] File descriptor (%d) not open.\n", fd);
    return -2;
  }

  p = running->fd[fd]->ppipe;
  if (!p)
  {
    printf("[KERNEL] Pipe not active.\n");
    return -3;
  }

  printf("[KERNEL] Attempting to read %d bytes ... \n", count);

  if (count <= 0)
  {
    printf("success.\n");
    printf("After pipe read:\n");
    show_pipe(p);
    return 0;
  }

  while(count)
  {
    while(p->data)
    {
      show_pipe(p);
      read = _readBytes(p, buf, count);
      count -= read;
      if (count == 0) break;
    }

    if (read)
    {
      /* some data has been read */
      printf("success: %d bytes read.\n", read);
      kwakeup(p->data);
      printf("After pipe read:\n");
      show_pipe(p);
      return read;
    }

    /* The pipe has no data */
    if (p->nWriter)
    {
      printf("[KERNEL] Pipe empty; reader going to sleep.\n");
      /* If the pipe still has a writer,
         wakeup all writers.
         Then sleep for data. */
      kwakeup(p->room);
      ksleep(p->data);
      continue;
    }

    /* Pipe has no write and no data */
    printf("After pipe read:\n");
    show_pipe(p);
    return 0;
  }
}

int _writeBytes(PIPE *p, char *buf, int count)
{
  int written = 0;

  while(written < count && p->room != 0)
  {
    p->buf[p->head] = (char)get_byte(running->uss, buf + written);
    (p->data)++;
    (p->head)++;
    (p->room)--;
    written++;
    if (p->head == PSIZE)
    {
      /* Reset the circular link */
      p->head = 0;
    }
  }

  return written;
}

int write_pipe(int fd, char *buf, int count)
{
  int written = 0;
  PIPE *p;

  if (fd < 0 || fd >= NFD)
  {
    printf("[KERNEL] Invalid file descriptor (%d).\n", fd);
    return -1;
  }

  if (!running->fd[fd])
  {
    printf("[KERNEL] File descriptor (%d) not open.\n", fd);
    return -2;
  }

  p = running->fd[fd]->ppipe;
  if (!p)
  {
    printf("[KERNEL] Pipe not active.\n");
    return -3;
  }

  printf("Before pipe write:\n");
  show_pipe(p);

  printf("[KERNEL] Attempting to write %d bytes ... \n", count);

  while(count)
  {
    if (!p->nReader)
    {
      kexit(BROKEN_PIPE);
    }

    while(p->room)
    {
      written = _writeBytes(p, buf, count);
      count -= written;

      if (!count) break;
    }

    kwakeup(p->room);
    if (count == 0)
    {
      printf("After pipe write:\n");
      show_pipe(p);
      return written;
    }

    printf("[KERNEL] Writer sleeping; need more bytes.\n");
    /* Still has data to write but the pipe has no room */
    ksleep(p->room);
    tswitch();
  }
}

int kpipe(int pd[2])
{
  int i, j, k;
  int fd1, fd2;

  printf("[KERNEL] Creating pipe ... ");

  /* Find an opened file descriptor in the running proc */
  for(i = 0; i < NFD; i++)
  {
    if (!running->fd[i]) break;
  }
  if (i == NFD || i + 1 == NFD)
  {
    printf("no more FDs.\n");
    return -1;
  }

  /* Find a second opened file descriptor in the running proc */
  for(j = i + 1; j < NFD; j++)
  {
    if (!running->fd[j]) break;
  }
  if (j == NFD)
  {
    printf("no more FDs.\n");
    return -2;
  }

  for(fd1 = 0; fd1 < NOFT; fd1++)
  {
    if (oft[fd1].refCount == 0) break;
  }
  if (fd1 == NOFT || fd1 + 1 == NOFT)
  {
    printf("no more openings in the OFT.\n");
    return -3;
  }

  for(fd2 = fd1 + 1; fd2 < NOFT; fd2++)
  {
    if (oft[fd2].refCount == 0) break;
  }
  if (fd2 == NOFT)
  {
    printf("no more openings in the OFT.\n");
    return -4;
  }

  for(k = 0; k < NPIPE; k++)
  {
    if (!pipe[k].busy) break;
  }
  if (k == NPIPE)
  {
    printf("no more available pipes.\n");
    return -5;
  }

  /* Allocate both ends of the pipe */
  pipe[k].nReader = 1;
  pipe[k].nWriter = 1;
  pipe[k].busy    = 1;
  pipe[k].head    = pipe[k].tail = pipe[k].data = 0;
  pipe[k].room    = PSIZE;

  oft[fd1].refCount++;
  oft[fd1].mode   = READ_PIPE;
  oft[fd1].ppipe  = &pipe[k];

  oft[fd2].refCount++;
  oft[fd2].mode   = WRITE_PIPE;
  oft[fd2].ppipe  = &pipe[k];

  running->fd[i]  = &oft[fd1];
  running->fd[j]  = &oft[fd2];

  /* place the fds into the user's pipe array */
  put_word(fd1, running->uss, &pd[0]);
  put_word(fd2, running->uss, &pd[1]);

  printf("done with FDs [%d %d].\n", fd1, fd2);
}

int close_pipe(int fd)
{
  PIPE *p;
  OFT  *t;

  if (fd < 0 || fd >= NFD)
  {
    printf("[KERNEL] Invalid file descriptor (%d).\n", fd);
    return -1;
  }

  printf("[KERNEL] Closing [%d] ... ", fd);

  t = running->fd[fd];
  t->refCount--;
  running->fd[fd] = 0;
  p = t->ppipe;

  if (t->mode == WRITE_PIPE)
  {
    p->nWriter--;

    if (p->nWriter == 0)
    {
      if (p->nReader <= 0)
      {
        /* Deallocate the pipe */
        p->busy = 0;
      }
    }
    kwakeup(p->data);
  }
  else /* t->mode == READ_PIPE */
  {
    p->nReader--;

    if (p->nReader == 0)
    {
      if (p->nWriter <= 0)
      {
        /* Deallocate the pipe */
        p->busy = 0;
      }
    }
    kwakeup(p->room);
  }

  printf("done.\n");
}
