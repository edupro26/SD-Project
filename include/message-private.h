#ifndef MESSAGE_PRIVATE_H
#define MESSAGE_PRIVATE_H

#include <stddef.h>
#include <sys/types.h>


ssize_t read_all(int fd, void *buf, size_t count);

ssize_t write_all(int fd, const void *buf, size_t count);

#endif 