#include <stdlib.h>
#include "table-private.h"

struct table_t *table_create(int n) {
    if (n <= 0) {
        return NULL;
    }
    
    struct table_t *new_table = malloc(sizeof(struct table_t));
    if (new_table == NULL) {
        return NULL;
    }

    new_table->size = n;
    new_table->lists = malloc(n*sizeof(struct list_t *));
    if (new_table->lists == NULL) {
        free(new_table);
        return NULL;
    }

    for (int i = 0; i < n; i++) {
        new_table->lists[i] = list_create();
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
    if (table == NULL) {
        return -1;
    }

    // frees each list
    for (int i = 0; i < table->size; i++) {
        list_derstroy(table->lists[i]);
    }
    // frees the array of lists
    free(table->lists);
    free(table);
    return 0;
}

int table_put(struct table_t *table, char *key, struct data_t *value) {
    if(table == NULL || key == NULL || value == NULL) {
        return -1;
    }

    int index = hash_code(key, table->size);

    struct entry_t *entry = entry_create(strdup(key), data_dup(value));

    if (entry == NULL) {
        return -1;
    }

    int result = list_add(table->lists[index], entry);

    // If the adding of entry doesn't go as expected, the entry is destroyed
    if (result == -1) {
        entry_destroy(entry);
    }

    return result;
}

struct data_t *table_get(struct table_t *table, char *key) {
    if (table == NULL || key == NULL) {
        return NULL;
    }

    int index = hash_code(key, table->size);
    struct list_t *list = table->lists[index];

    if (list == NULL) {
        return NULL;
    }

    struct entry_t *entry = list_get(list, key);

    if (entry == NULL) {
        return NULL;
    }

    return data_dup(entry->value);
}

int table_remove(struct table_t *table, char *key) {
    if (table == NULL || key == NULL) {
        return -1;
    }

    int index = hash_code(key, table->size);
    struct list_t *list = table->lists[index];

    if (list == NULL) {
        return 1;
    }

    struct entry_t *removed_entry = list_remove(list, key);

    if (removed_entry != NULL) {
        entry_destroy(removed_entry);
        return 0;
    } else {
        return 1;
    }
}

int table_size(struct table_t *table) {
    if (table == NULL) {
        return -1;
    }

    int tab_size = 0;
    
    for (int i = 0; i < table->size; i++) {
        struct list_t *list = table->lists[i];

        // Counts every entry on a list
        if (list != NULL) {
            tab_size += list_size(list);
        }
    }
    return tab_size;
}