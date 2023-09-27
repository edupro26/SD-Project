#include <stdlib.h>
#include "list-private.h"

struct list_t *list_create() {
    struct list_t *new_list = malloc(sizeof(struct list_t));

    if(new_list == NULL) {
        return NULL;
    }
    // List starts empty
    new_list->size = 0;

    return new_list;
}