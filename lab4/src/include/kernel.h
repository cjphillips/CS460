#ifndef KERNEL_H
#define KERNEL_H

PROC *kfork(char *filename);

int kexit(int exitValue);

#endif /* KERNEL_H */
