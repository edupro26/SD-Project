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
    if (key == NULL || data == NULL) return NULL; // Return null if key or data is null

    struct entry_t *ptr = (struct entry_t *) malloc(sizeof(struct entry_t));
    if(ptr == NULL) return NULL; // Return null if malloc fails
    
    ptr->key = key;
    ptr->value = data;

    return ptr;
}

int entry_destroy(struct entry_t *entry) {
    if(entry == NULL) return -1; // Return -1 if entry is null
    if (entry->key == NULL || entry->value == NULL) return -1; // Return -1 if entry->key or entry->value is null

    free(entry->key); // Free the key string
  
    data_destroy(entry->value); // Free the data using data_destroy from data.h
    
    free(entry);

    return 0;
    
}

struct entry_t *entry_dup(struct entry_t *entry) {
    if(entry == NULL) return NULL; // Return null if entry is null

    if(entry->key == NULL || entry->value == NULL) // Return null if entry->key or entry->value is null
        return NULL;

    // Creates a new entry with duplicated key and value
    struct entry_t *ptr = entry_create(strdup(entry->key), data_dup(entry->value));
    if(ptr == NULL) return NULL; // Return null if malloc fails

    return ptr;
}

int entry_replace(struct entry_t *entry, char *new_key, struct data_t *new_value) {
    // Return -1 if entry, new_key or new_value is null
    if (entry == NULL || new_key == NULL || new_value == NULL) {
        return -1;
    }

    // Return -1 if entry->key or entry->value is null
    if (entry->key == NULL || entry->value == NULL) return -1;

    free(entry->key); // Free the key string
  
    data_destroy(entry->value); // Free the data using data_destroy from data.h
 
    // Set the new key and value
    entry->key = new_key;
    entry->value = new_value;

    return 0;
}

int entry_compare(struct entry_t *entry1, struct entry_t *entry2) {
    if (entry1 == NULL || entry2 == NULL) return -2; // Return -2 if entry1 or entry2 is null

    return strcmp(entry1->key, entry2->key);
}
