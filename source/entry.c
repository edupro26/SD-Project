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

#include "entry.h"

struct entry_t *entry_create(char *key, struct data_t *data) {
    struct entry_t *ptr;
    if (key == NULL || data == NULL) return NULL;

    ptr = (struct entry_t *) malloc(sizeof(struct entry_t));
    if(ptr == NULL) return NULL;
    
    ptr->key = key;
    ptr->value = data;

    return ptr;
}

int entry_destroy(struct entry_t *entry) {
    if(entry != NULL){
        if(entry->key == NULL || entry->value == NULL)
            return -1;

        data_destroy(entry->value);
        free(entry->key);

        free(entry);
        return 0;
    }
    
    return -1;
}

struct entry_t *entry_dup(struct entry_t *entry) {
    struct entry_t *ptr;
    if(entry == NULL) return NULL;

    if(entry->key == NULL || entry->value == NULL)
        return NULL;

    ptr = entry_create(entry->key, entry->value);
    if(ptr == NULL) return NULL;

    return ptr;
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
