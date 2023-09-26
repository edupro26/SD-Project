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

/* Função que duplica uma entry, reservando a memória necessária para a
 * nova estrutura.
 * Retorna a nova entry ou NULL em caso de erro.
 */
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

