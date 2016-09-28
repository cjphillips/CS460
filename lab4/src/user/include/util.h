#ifndef UTIL_H
#define UTIL_H

int showMenu();

char *strtok(char *s, char delim);

int findCmd(char *command);

int getpid();

int ps();

int chname();

int fork();

int exec();

int kswitch();

int geti();

int wait();

int exit();

int _exit(int exitValue);

#endif /* UTIL_H */
