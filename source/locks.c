/*

Sistemas Distribuidos - Projeto Fase 3
Grupo 8
Alexandre Pinto - 55958
Eduardo Proença  - 57551
Tiago Oliveira - 54979

*/

#include "locks.h"

static pthread_mutex_t mutex;
static pthread_mutex_t mutex_stats;

void locks_init() {
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&mutex_stats, NULL);
}

void locks_destroy() {
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&mutex_stats);
}

void locks_lock() {
    pthread_mutex_lock(&mutex);
}

void locks_unlock() {
    pthread_mutex_unlock(&mutex);
}

void locks_lock_stats() {
    pthread_mutex_lock(&mutex_stats);
}

void locks_unlock_stats() {
    pthread_mutex_unlock(&mutex_stats);
}


typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t read, put;
    int reads, puts, wputs;
} locks_t;
locks_t lock;

void init_data_lock(locks_t *lock) {
    pthread_mutex_init(&lock->mutex, NULL);
    pthread_cond_init(&lock->read, NULL);
    pthread_cond_init(&lock->put, NULL);
    lock->reads = lock->puts = lock->wputs = 0;
}

void get(locks_t *lock) {
    pthread_mutex_lock(&lock->mutex);
    while (lock->puts > 0 || lock->wputs > 0) {
        pthread_cond_wait(&lock->read, &lock->mutex);
    }
    lock->reads++;
    pthread_mutex_unlock(&lock->mutex);
}

void leaveGet(locks_t *lock) {
    pthread_mutex_lock(&lock->mutex);
    lock->reads--;
    if (lock->reads == 0 && lock->wputs > 0) {
        pthread_cond_signal(&lock->put);
    }
    pthread_mutex_unlock(&lock->mutex);
}

void put(locks_t *lock) {
    pthread_mutex_lock(&lock->mutex);
    lock->wputs++;
    while (lock->reads > 0 || lock->puts > 0) {
        pthread_cond_wait(&lock->put, &lock->mutex);
    }
    lock->wputs--;
    lock->puts++;
    pthread_mutex_unlock(&lock->mutex);
}

void leavePut(locks_t *lock) {
    pthread_mutex_lock(&lock->mutex);
    lock->puts--;
    if (lock->wputs > 0) {
        pthread_cond_signal(&lock->put);
    } else {
        pthread_cond_broadcast(&lock->read);
    }
    pthread_mutex_unlock(&lock->mutex);
}

