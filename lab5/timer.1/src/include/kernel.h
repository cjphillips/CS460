#ifndef KERNEL_H
#define KERNEL_H

int fork();

PROC *kfork(char *filename);

int kexit(int exitValue);

#endif /* KERNEL_H */
