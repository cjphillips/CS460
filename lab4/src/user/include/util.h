#ifndef UTIL_H
#define UTIL_H

int show_menu();

int findCmd(char *command);

int getpid();

int ps();

int chname();

int kfork();

int kswitch();

int geti();

int wait();

int exit();

int _exit(int exitValue);

#endif /* UTIL_H */
