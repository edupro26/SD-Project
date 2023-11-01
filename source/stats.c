#include "stats.h"
#include <stdlib.h>

struct statistics_t global_stats = {0, 0, 0};
struct statistics_t *stats = &global_stats;

void init_statistics(void) {
    // Already initialized globally, but you can reset values here if needed
    stats->ops = 0;
    stats->clients = 0;
    stats->time = 0;
}


void update_statistics(int ops, int clients, int time) {
    stats->ops += ops;
    stats->clients += clients;
    stats->time += time;
}

struct statistics_t *get_statistics(void) {
    return stats;
}