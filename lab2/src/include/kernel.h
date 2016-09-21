#ifndef KERNEL_H
#define KERNEL_H

int init();

int scheduler();

int body();

PROC *kfork();

int kwait();

int kexit();

#endif /* KERNEL_H */
