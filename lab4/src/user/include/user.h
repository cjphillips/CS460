#ifndef USER_H
#define USER_H

#define NAMESIZE 32
#define FILE_LENGTH 64

typedef unsigned char   u8;
typedef unsigned short u16;
typedef unsigned long  u32;

extern int getcs();
extern int syscall();

char *args[NAMESIZE];

int color;

#endif /* USER_H */
