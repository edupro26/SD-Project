/*

Sistemas Distribuidos - Projeto Fase 3
Grupo 8
Alexandre Pinto - 55958
Eduardo Proença  - 57551
Tiago Oliveira - 54979

*/

#include <stdio.h>
#include <stdlib.h>

#include <signal.h>
#include "network_server.h"
#include "table_skel.h"
#include "locks.h"


int main(int argc, char **argv) {
    signal(SIGPIPE, SIG_IGN);
    // Check if 2 arguments were passed
    if (argc != 3) {
        printf("Usage: ./table_server <port> <n_lists>\n");
        return -1;
    }

    // Verify if first argument is a valid port
    unsigned short port = atoi(argv[1]);
    if (port <= 1023) {
        printf("Invalid port\n");
        printf("Usage: ./table_server <port> <n_lists>\n");
        return -1;
    }

    // Check if the second argument is a valid number of lists
    int n_lists = atoi(argv[2]);
    if (n_lists <= 0) {
        printf("Invalid number of lists\n");
        printf("Usage: ./table_server <port> <n_lists>\n");
        return -1;
    }

    locks_init();

    // Start server
    int sockfd = network_server_init(port);

    if (sockfd < 0) {
        printf("Error starting server\n");
        return -1;
    }

    printf("Server ready, waiting for connections\n");

    // Initialize table
    struct table_t *table = table_skel_init(n_lists);

    if (table == NULL) {
        printf("Error initializing table\n");
        return -1;
    }

    // Main loop
    network_main_loop(sockfd, table);

    // Destroy table
    table_skel_destroy(table);

    // Close server
    int cls = network_server_close(sockfd);

    if (cls < 0) {
        printf("Error closing server\n");
        return -1;
    }

    locks_destroy();

    return 0;
}

