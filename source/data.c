/* 

Sistemas Distribuidos - Projeto Fase 1 
Grupo 8
Alexandre Pinto - 55958
Eduardo Proença  - 57551
Tiago Oliveira - 54979

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "data.h"

struct data_t *data_create(int size, void *data) {
    if (data == NULL || size <= 0) {
        return NULL;
    }

    struct data_t *new_data = (struct data_t *) malloc(sizeof(struct data_t));

    if ( new_data == NULL) {
        return NULL;
    }
    
    new_data->datasize = size;
    new_data->data = data;
    return new_data;
}


int data_destroy(struct data_t *data) {
    if (data == NULL) {
        return -1;
    }
    free(data->data);
    free(data);
    return 0;
}


struct data_t *data_dup(struct data_t *data) {
    if (data == NULL || data->data == NULL || data->datasize <= 0) {
        return NULL;
    }

    struct data_t *new_data = (struct data_t *) malloc(sizeof(struct data_t));
    if (new_data == NULL) {
        return NULL;
    }

    new_data->datasize = data->datasize;
    // Allocs the memory of the new data structure
    new_data->data = malloc(data->datasize);
    if(new_data->data == NULL) {
        // In case of error, frees the memory previously allocated
        free(new_data);
        return NULL;
    }
    // If everything goes right, copies the data to the new data structure
    memcpy(new_data->data, data->data, data->datasize);

    return new_data;
}


int data_replace(struct data_t *data, int new_size, void *new_data) {
    if (data == NULL || new_data == NULL || new_size <= 0 || data->datasize <= 0) {
        return -1;
    }
    // Frees the memory of the old data
    free(data->data);
    // Updates the new size of the data
    data->datasize = new_size;
    // Allocs the memory for the new data
    data->data = malloc(new_size);

    if (data->data == NULL) {
        return -1;
    }

    // If the data is valid, copies the new data to the data structure
    memcpy(data->data, new_data, new_size);

    return 0;
}
