#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>

#include "serialization.h"


int keyArray_to_buffer(char **keys, char **keys_buf) {
    if (keys == NULL || keys_buf == NULL) {
        return -1;
    }

    int nkeys = 0;
    int size = 0;
    int i = 0;

    while (keys[i] != NULL) {
        nkeys++;
        size += strlen(keys[i]) + 1;
        i++;
    }

    size += sizeof(int);
    *keys_buf = (char *) malloc(size);

    if (*keys_buf == NULL) {
        return -1;
    }

    *((int *) *keys_buf) = htonl(nkeys);

    int offset = sizeof(int);

    for (i = 0; i < nkeys; i++) {
        strcpy(*keys_buf + offset, keys[i]);
        offset += strlen(keys[i]) + 1;
    }

    return size;
}


char** buffer_to_keyArray(char *keys_buf) {
    if (keys_buf == NULL) {
        return NULL;
    }

    int nkeys = ntohl(*((int *) keys_buf));

    char **keys = (char **) malloc((nkeys + 1) * sizeof(char *));

    if (keys == NULL) {
        return NULL;
    }

    int offset = sizeof(int);

    for (int i = 0; i < nkeys; i++) {
        keys[i] = strdup(keys_buf + offset);
        offset += strlen(keys_buf + offset) + 1;
    }

    keys[nkeys] = NULL;

    return keys;
}