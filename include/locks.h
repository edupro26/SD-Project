/*

Sistemas Distribuidos - Projeto Fase 3
Grupo 8
Alexandre Pinto - 55958
Eduardo Proença  - 57551
Tiago Oliveira - 54979

*/

#ifndef LOCKS_H
#define LOCKS_H

#include <pthread.h>
#include <stdlib.h>

struct locks_t {
    pthread_mutex_t mutex;
    pthread_cond_t read, write;
    int reads, writes, wwrites;
};

struct locks_t* init_lock();

void readLock(struct locks_t *lock);

void leaveRead(struct locks_t *lock);

void writeLock(struct locks_t *lock);

void leaveWrite(struct locks_t *lock);

void destroy_lock(struct locks_t *lock);


#endif