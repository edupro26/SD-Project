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

// Initates and returns the pointer of a locks_t struct
struct locks_t* init_lock();

// Function to be called when a thread wants to read. This will prevent the thread to read
// if there are threads writing or wanting to write
void readLock(struct locks_t *lock);

// Function to be called after a threads ends reading operation.
void leaveRead(struct locks_t *lock);

// Function to be called when a thread wants to write. This will prevent the thread to write 
// if there are thread reading or writing at the time
void writeLock(struct locks_t *lock);

// Function to be called when a thread ends a writing operation. This will signal other threads wanting
// to write.
void leaveWrite(struct locks_t *lock);

// Function that will destroy lock_t struct, liberating its memory
void destroy_lock(struct locks_t *lock);


#endif