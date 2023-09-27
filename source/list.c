#include <stdlib.h>
#include "list-private.h"

struct list_t *list_create() {
    struct list_t *new_list = malloc(sizeof(struct list_t));

    if (new_list == NULL) {
        return NULL;
    }
    // List starts empty
    new_list->size = 0;

    return new_list;
}

int list_destroy(struct list_t *list) {
    if (list == NULL) {
        return -1;
    }

    struct node_t *current = list->head;
    struct node_t *next;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    free(list);
    return 0;
}
