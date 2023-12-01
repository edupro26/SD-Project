/*

Sistemas Distribuidos - Projeto Fase 4
Grupo 8
Alexandre Pinto - 55958
Eduardo Proença  - 57551
Tiago Oliveira - 54979

*/

#ifndef _STATISTICS_PRIVATE_H_
#define _STATISTICS_PRIVATE_H_

#include <stdlib.h>

struct statistics_t
{
    int ops;
    int clients;
    int time;
};

// Extern variable that will allow every method that needs to read or write to stats data
// acccess to it
extern struct statistics_t *stats;

// Function that will initiate the statistics_t struct and allocate memory for it
struct statistics_t* init_statistics();

// Function that will free the memory allocated for the statistics_t struct
void destroy_statistics();

#endif