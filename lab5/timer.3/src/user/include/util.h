#ifndef UTIL_H
#define UTIL_H

int showMenu();

int tokenize(char *path, char *delim, char **out);

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

int itimer();

int _exit(int exitValue);

#endif /* UTIL_H */
