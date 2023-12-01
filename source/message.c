/*

Sistemas Distribuidos - Projeto Fase 4
Grupo 8
Alexandre Pinto - 55958
Eduardo Proença  - 57551
Tiago Oliveira - 54979

*/

#include <unistd.h>
#include <errno.h>

#include "message-private.h"

ssize_t read_all(int fd, void *buf, size_t count) {
    size_t bytes_read = 0;
    while (bytes_read < count) {
        ssize_t result = read(fd, (char *)buf + bytes_read, count - bytes_read);
        if (result < 0) {
            if (errno == EINTR) continue; 
            return -1; 
        }
        if (result == 0) return result;
        bytes_read += result;
    }
    return bytes_read;
}


ssize_t write_all(int fd, const void *buf, size_t count) {
    size_t bytes_written = 0;
    while (bytes_written < count) {
        ssize_t result = write(fd, (const char *)buf + bytes_written, count - bytes_written);
        if (result < 0) {
            if (errno == EINTR) continue; 
            return -1; 
        }
        if (result == 0) return result;
        bytes_written += result;
    }
    return bytes_written;
}