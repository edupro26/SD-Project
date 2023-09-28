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
    if (data == NULL || size <= 0) return NULL;

    struct data_t *ptr = (struct data_t *) malloc(sizeof(struct data_t));
    if (ptr == NULL) return NULL;
    
    ptr->datasize = size;
    ptr->data = data;

    return ptr;
}

int data_destroy(struct data_t *data) {
    if (data != NULL){
        if(data->data != NULL) //Possible error in
            free(data->data);   // this code
    }
    else{
        return -1;
    }

    free(data);
    return 0;
}

struct data_t *data_dup(struct data_t *data) {
    if (data == NULL || data->datasize <= 0 || data->data == NULL) 
        return NULL;

    struct data_t *ptr = (struct data_t *) malloc(sizeof(struct data_t));
    if (ptr == NULL) return NULL;

    ptr->datasize = data->datasize;
    ptr->data = malloc(data->datasize);

    if(ptr->data == NULL) {
        free(ptr);
        return NULL;
    }

    memcpy(ptr->data, data->data, data->datasize);
    return ptr;
}

int data_replace(struct data_t *data, int new_size, void *new_data) {
    if (data == NULL) return -1;

    if(new_size > 0 && new_data != NULL){
        free(data->data);
        data->datasize = new_size;
        data->data = malloc(new_size);
        if (data->data == NULL) return -1;

        return 0;
    }

    memcpy(data->data, new_data, new_size);
    return -1;
}
