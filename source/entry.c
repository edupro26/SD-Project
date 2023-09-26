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

int entry_destroy(struct entry_t *entry) {
    if (entry == NULL) {
        return -1;
    }
    free(entry->key);
    data_destroy(entry->value);
    free(entry);
    return 0;
}

struct entry_t *entry_dup(struct entry_t *entry) {
    if (entry == NULL) {
        return NULL;
    }

    struct entry_t *new_entry = malloc(sizeof(struct entry_t));
    if (new_entry == NULL) {
        return NULL;
    }

    new_entry->key = entry->key;
    new_entry->value = data_dup(entry->value);

    return new_entry;
}

int entry_replace(struct entry_t *entry, char *new_key, struct data_t *new_value) {
    if (entry == NULL || new_key == NULL || new_value == NULL) {
        return -1;
    }

    free(entry->key);
    data_destroy(entry->value);

    entry->key = new_key;
    entry->value = new_value;

    return 0;
}

int entry_compare(struct entry_t *entry1, struct entry_t *entry2) {
    if (entry1 == NULL || entry2 == NULL) {
        return -2;
    }

    return strcmp(entry1->key, entry2->key);
}
