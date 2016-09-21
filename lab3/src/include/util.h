#ifndef UTIL_H
#define UTIL_H

/*
int get_word(u16 segment, u16 offset);

void put_word(u16 word, u16 segment, u16 offset);*/

void do_kfork();

void do_tswitch();

void do_wait();

void do_exit();

#endif /* UTIL_H */
