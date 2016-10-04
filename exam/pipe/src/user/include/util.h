#ifndef UTIL_H
#define UTIL_H

int showMenu();

int tokenize(char *path, char *delim, char **out);

int findCmd(char *command);

int getpid();

int ps();

int chname();

int fork();

int write();

int read();

int pfd();

int pipe();

int close();

int exec();

int kswitch();

int geti();

int wait();

int exit();

int _exit(int exitValue);

#endif /* UTIL_H */
