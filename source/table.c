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