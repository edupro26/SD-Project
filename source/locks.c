/*

Sistemas Distribuidos - Projeto Fase 3
Grupo 8
Alexandre Pinto - 55958
Eduardo Proença  - 57551
Tiago Oliveira - 54979

*/

#include "locks.h"

struct locks_t* init_lock() {
    struct locks_t *lock = (struct locks_t *) malloc(sizeof(struct locks_t));
    pthread_mutex_init(&lock->mutex, NULL);
    pthread_cond_init(&lock->read, NULL);
    pthread_cond_init(&lock->write, NULL);
    lock->reads = lock->writes = lock->wwrites = 0;

    return lock;
}

void readLock(struct locks_t *lock) {
    pthread_mutex_lock(&lock->mutex);
    while (lock->writes > 0 || lock->wwrites > 0) {
        pthread_cond_wait(&lock->read, &lock->mutex);
    }
    lock->reads++;
    pthread_mutex_unlock(&lock->mutex);
}

void leaveRead(struct locks_t *lock) {
    pthread_mutex_lock(&lock->mutex);
    lock->reads--;
    if (lock->reads == 0 && lock->wwrites > 0) {
        pthread_cond_signal(&lock->write);
    }
    pthread_mutex_unlock(&lock->mutex);
}

void writeLock(struct locks_t *lock) {
    pthread_mutex_lock(&lock->mutex);
    lock->wwrites++;
    while (lock->reads > 0 || lock->writes > 0) {
        pthread_cond_wait(&lock->write, &lock->mutex);
    }
    lock->wwrites--;
    lock->writes++;
    pthread_mutex_unlock(&lock->mutex);
}

void leaveWrite(struct locks_t *lock) {
    pthread_mutex_lock(&lock->mutex);
    lock->writes--;
    if (lock->wwrites > 0) {
        pthread_cond_signal(&lock->write);
    } else {
        pthread_cond_broadcast(&lock->read);
    }
    pthread_mutex_unlock(&lock->mutex);
}

void destroy_lock(struct locks_t *lock) {
    pthread_mutex_destroy(&lock->mutex);
    pthread_cond_destroy(&lock->read);
    pthread_cond_destroy(&lock->write);
    free(lock);
}



