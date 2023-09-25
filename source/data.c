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