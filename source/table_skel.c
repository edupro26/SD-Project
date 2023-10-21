/* 

Sistemas Distribuidos - Projeto Fase 2
Grupo 8
Alexandre Pinto - 55958
Eduardo Proença  - 57551
Tiago Oliveira - 54979

*/


#include "table_skel.h"

struct table_t *table_skel_init(int n_lists) {
    // Create table
    struct table_t *table = table_create(n_lists);
    if (table == NULL) {
        return NULL;
    }

    return table;
}


int table_skel_destroy(struct table_t *table) {
    // Verify if table is NULL
    if (table == NULL) {
        return -1;
    }

    // Destroy table and all memory allocated
    return table_destroy(table);
}

int invoke(MessageT *msg, struct table_t *table) {
    return 0;
}