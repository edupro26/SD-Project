/* 

Sistemas Distribuidos - Projeto Fase 4
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
    if (data == NULL || size <= 0) return NULL; // Return null if data is null or size is 0 or less

    struct data_t *ptr = (struct data_t *) malloc(sizeof(struct data_t));
    if (ptr == NULL) return NULL; // Return null if malloc fails
    
    ptr->datasize = size;
    ptr->data = data;

    return ptr;
}

int data_destroy(struct data_t *data) {
    if (data != NULL){
        if (data->data != NULL) free(data->data); // Free the data if it is not null
    }
    else{
        return -1; // Return -1 if data is null
    }

    free(data);
    return 0;
}

struct data_t *data_dup(struct data_t *data) {
    // Return null if data is null or data size is 0 or less or data->data is null
    if (data == NULL || data->datasize <= 0 || data->data == NULL) 
        return NULL;

    struct data_t *ptr = (struct data_t *) malloc(sizeof(struct data_t));
    if (ptr == NULL) return NULL; // Return null if malloc fails

    ptr->datasize = data->datasize;
    ptr->data = malloc(data->datasize); // Allocate memory for the data

    // Return null if malloc fails and free the struct
    if(ptr->data == NULL) {
        free(ptr);
        return NULL;
    }

    memcpy(ptr->data, data->data, data->datasize); // Copy the data to the new struct
    return ptr;
}

int data_replace(struct data_t *data, int new_size, void *new_data) {
    if (data == NULL) return -1; // Return -1 if data is null

    // If new_size is bigger than 0 and new_data is not null, free the data and allocate memory for the new data
    if(new_size > 0 && new_data != NULL){
        free(data->data);
        data->datasize = new_size;
        data->data = malloc(new_size);
        if (data->data == NULL) return -1;

        return 0;
    }

    memcpy(data->data, new_data, new_size); // If new_size is 0 or less, copy the new data to the struct
    return -1;
}
