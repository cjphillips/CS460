#ifndef WAIT_H
#define WAIT_H

int ksleep(int event);

int kwakeup(int event);

int kwait(int *status);

#endif /* WAIT_H */
