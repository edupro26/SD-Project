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