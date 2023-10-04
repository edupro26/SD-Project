/* 

Sistemas Distribuidos - Projeto Fase 1 
Grupo 8
Alexandre Pinto - 55958
Eduardo Proença  - 57551
Tiago Oliveira - 54979

*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>

#include "serialization.h"


int keyArray_to_buffer(char **keys, char **keys_buf) {

    // Check if keys is NULL or if keys is empty
    if (keys == NULL || keys_buf == NULL) return -1;

    int nkeys = 0;
    int size = 0;
    int i = 0;

    // Count the number of keys and the size of the buffer
    while (keys[i] != NULL) {
        nkeys++;
        size += strlen(keys[i]) + 1;
        i++;
    }

    
    size += sizeof(int); // Add the size of the number of keys
    *keys_buf = (char *) malloc(size); // Allocate memory for the buffer


    // Retturn -1 in case of error during the allocation
    if (*keys_buf == NULL) return -1;

    *((int *) *keys_buf) = htonl(nkeys); // Store the number of keys in the buffer

    int offset = sizeof(int); // Offset to store the keys

    // Store the keys in the buffer
    for (i = 0; i < nkeys; i++) {
        strcpy(*keys_buf + offset, keys[i]);
        offset += strlen(keys[i]) + 1;
    }

    return size;
}


char** buffer_to_keyArray(char *keys_buf) {

    // Run if keys_buf is NULL
    if (keys_buf == NULL) return NULL;

    // Get the number of keys in the buffer
    int nkeys = ntohl(*((int *) keys_buf));

    // Allocate memory for the keys array
    char **keys = (char **) malloc((nkeys + 1) * sizeof(char *));

    // Return NULL in case of error during the allocation
    if (keys == NULL) {
        return NULL;
    }

    int offset = sizeof(int);

    // Store the keys in the array
    for (int i = 0; i < nkeys; i++) {
        keys[i] = strdup(keys_buf + offset);
        offset += strlen(keys_buf + offset) + 1;
    }

    keys[nkeys] = NULL;

    return keys;
}