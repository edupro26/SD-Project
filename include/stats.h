#ifndef _STATISTICS_PRIVATE_H_
#define _STATISTICS_PRIVATE_H_

struct statistics_t
{
    int ops;
    int clients;
    int time;
};

// Global variable with 0 as default value
struct statistics_t *stats;

struct statistics_t *statistics_init();

struct statistics_t *statistics_get();

void statistics_destroy();

#endif