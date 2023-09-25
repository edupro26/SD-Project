#include <stdlib.h>
#include <data.h>


struct data_t *data_create(int size, void *data) {
    struct data_t *new_data = (struct data_t *) malloc(sizeof(struct data_t));
    if (new_data == NULL) {
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
    if (data == NULL || data->data == NULL) {
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
