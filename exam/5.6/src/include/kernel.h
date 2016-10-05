#ifndef KERNEL_H
#define KERNEL_H

int fork();

PROC *kfork(char *filename);

int kexit(int exitValue);

int khop(u16 segment);

#endif /* KERNEL_H */
