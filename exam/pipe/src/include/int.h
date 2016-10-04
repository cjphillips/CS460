#ifndef INT_H
#define INT_H

int kgetpid();

int kps();

int kchname(char *name);

int kkfork();

int ktswitch();

int kkwait(int *status);

int kkexit(int exitValue);

int kcinth();

#endif /* INT_H */
