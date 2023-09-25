#include <stdlib.h>
#include "entry.h"


struct entry_t *entry_create(char *key, struct data_t *data) {
    if (key == NULL || data == NULL) {
        return NULL;
    }

    // Allocs the memory for the new entry
    struct entry_t *new_entry = malloc(sizeof(struct entry_t));

    new_entry->key = key;
    new_entry->value = data;

    return new_entry;
}