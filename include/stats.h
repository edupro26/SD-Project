/*

Sistemas Distribuidos - Projeto Fase 3
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

extern struct statistics_t *stats;

struct statistics_t* init_statistics();

void destroy_statistics();

#endif