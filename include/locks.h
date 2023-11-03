#ifndef LOCKS_H
#define LOCKS_H

#include <pthread.h>

void locks_init();
void locks_destroy();
void locks_lock();
void locks_unlock();
void locks_lock_stats();
void locks_unlock_stats();

#endif