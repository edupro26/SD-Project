/*

Sistemas Distribuidos - Projeto Fase 3
Grupo 8
Alexandre Pinto - 55958
Eduardo Proença  - 57551
Tiago Oliveira - 54979

*/

#ifndef MESSAGE_PRIVATE_H
#define MESSAGE_PRIVATE_H

#include <stddef.h>
#include <sys/types.h>

/*
These functions make sure that all bytes are read/written from/to a socket.
*/

// Read count bytes from fd into buf
ssize_t read_all(int fd, void *buf, size_t count);

// Write count bytes from buf into fd
ssize_t write_all(int fd, const void *buf, size_t count);

#endif 