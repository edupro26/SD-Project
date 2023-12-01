/*

Sistemas Distribuidos - Projeto Fase 4
Grupo 8
Alexandre Pinto - 55958
Eduardo Proença  - 57551
Tiago Oliveira - 54979

*/

#include "stats.h"
#include <stdlib.h>

#include "locks.h"

struct statistics_t *stats;


struct statistics_t* init_statistics() {
    struct statistics_t *stats = (struct statistics_t *) malloc(sizeof(struct statistics_t));
    // Already initialized globally, but you can reset values here if needed
    stats->ops = 0;
    stats->clients = 0;
    stats->time = 0;

    return stats;
}

void destroy_statistics() {
    free(stats);
}