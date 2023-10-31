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

#endif