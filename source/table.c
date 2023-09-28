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
