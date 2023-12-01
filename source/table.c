/* 

Sistemas Distribuidos - Projeto Fase 4
Grupo 8
Alexandre Pinto - 55958
Eduardo Proença  - 57551
Tiago Oliveira - 54979

*/

#include <stdlib.h>
#include <string.h>

#include "table-private.h"
#include "table.h"
#include "data.h"
#include "list.h"

int hash_code(char *key, int n) {
    int count = 0;
    
    for (int i = 0; key[i] != '\0'; i++) {
        count += key[i];
    }

    return count % n;
}


struct table_t *table_create(int n) {

    // Return null if n is 0 or less
    if (n <= 0) return NULL;
    
    struct table_t *new_table = malloc(sizeof(struct table_t));
    if (new_table == NULL) return NULL; // Return null if malloc fails

    new_table->size = n;
    new_table->lists = malloc(n*sizeof(struct list_t *));

    // If malloc for the array of lists fails, the table is destroyed and null is returned
    if (new_table->lists == NULL) {
        free(new_table);
        return NULL;
    }

    // Creates each list
    for (int i = 0; i < n; i++) {
        new_table->lists[i] = list_create();
        // If the creation of a list fails, the table is destroyed and null is returned
        if (new_table->lists[i] == NULL) {
            for (int j = 0; j < i; j++) {
                list_destroy(new_table->lists[j]);
            }
            free(new_table->lists);
            free(new_table);
            return NULL;
        }
    }
    return new_table;
}

int table_destroy(struct table_t *table) {
    if (table == NULL) return -1; // Return -1 if table is null

    // Frees each list
    for (int i = 0; i < table->size; i++) {
        list_destroy(table->lists[i]);
    }
    
    free(table->lists); // Frees the array of lists
    free(table); // Frees the table
    return 0;
}

int table_put(struct table_t *table, char *key, struct data_t *value) {
    // Return -1 if table, key or value are null
    if(table == NULL || key == NULL || value == NULL) {
        return -1;
    }

    int index = hash_code(key, table->size);

    struct entry_t *entry = entry_create(strdup(key), data_dup(value));

    if (entry == NULL) return -1; // Return -1 if entry creation fails

    int result = list_add(table->lists[index], entry); // Adds the entry to the list

    // If the adding of entry doesn't go as expected, the entry is destroyed
    if (result == -1) {
        entry_destroy(entry);
    }

    return result;
}

struct data_t *table_get(struct table_t *table, char *key) {
    if (table == NULL || key == NULL) return NULL; // Return null if table or key are null

    int index = hash_code(key, table->size);
    struct list_t *list = table->lists[index];

    if (list == NULL) return NULL; // Return null if the list is null

    struct entry_t *entry = list_get(list, key); // Gets the entry from the list

    if (entry == NULL) return NULL; // Return null if the entry is null

    return data_dup(entry->value); // Returns a duplicate of the entry's value
}

int table_remove(struct table_t *table, char *key) {
    if (table == NULL || key == NULL) return -1; // Return -1 if table or key are null

    
    int index = hash_code(key, table->size);
    struct list_t *list = table->lists[index];

    if (list == NULL) return 1; // Return 1 if the list is not found
    
    return list_remove(list, key); // Calls the list's remove function and returns its result
}

int table_size(struct table_t *table) {
    if (table == NULL) return -1; // Return -1 if table is null

    int tab_size = 0;
    
    // Counts every entry on a list
    for (int i = 0; i < table->size; i++) {
        struct list_t *list = table->lists[i];

        if (list != NULL) {
            tab_size += list_size(list);
        }
    }
    return tab_size;
}

char **table_get_keys(struct table_t *table) {
    if (table == NULL) return NULL; // Return null if table is null

    char **keys = malloc((table_size(table) + 1) * sizeof(char *));
    if (keys == NULL) return NULL; // Return null if malloc fails

    int index = 0;

    // Gets the keys from each list
    for (int i = 0; i < table->size; i++) {
        struct list_t *list = table->lists[i];

        // In each valid list, gets the keys
        if (list != NULL) {
            char **list_keys = list_get_keys(list);
            int list_keys_size = list_size(list);

            if (list_keys != NULL) {
                for (int j = 0; j < list_keys_size; j++) {
                    keys[index++] = list_keys[j];
                }
                free(list_keys);
            }
        }
    }
    // The last element of the array must be null
    keys[index]= NULL;
    return keys;
}

int table_free_keys(char **keys) {
    if (keys == NULL) return -1; // Return -1 if keys is null

    // Free the memory allocated for each key
    for (int i = 0;keys[i] != NULL; i++) {
        free(keys[i]);
    }
    free(keys);

    return 0;
}