#ifndef USER_H
#define USER_H

#define NAMESIZE 32

typedef unsigned char   u8;
typedef unsigned short u16;
typedef unsigned long  u32;

extern int getcs();
extern int syscall();

int color;

#endif /* USER_H */
