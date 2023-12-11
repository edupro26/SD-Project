/*

Sistemas Distribuidos - Projeto Fase 4
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
#include "zk_server.h"


int sockfd;
struct table_t *table;


void stop_server(int signum) {
    if (signum != SIGINT) {
        return;
    }

    table_skel_destroy(table);
    destroy_zk();
    int cls = network_server_close(sockfd);

    if (cls < 0) {
        printf("Error closing server\n");
        exit(-1);
    }

    printf("Server closed\n");
    exit(0);
}

int main(int argc, char **argv) {
    signal(SIGINT, stop_server);
    signal(SIGPIPE, SIG_IGN);
    // Check if 2 arguments were passed
    if (argc != 4) {
        printf("Usage: ./table_server <port> <n_lists> <zk_ip:port>\n");
        return -1;
    }

    // Verify if first argument is a valid port
    unsigned short port = atoi(argv[1]);
    if (port <= 1023) {
        printf("Invalid port\n");
        printf("Usage: ./table_server <port> <n_lists> <zk_ip:port>\n");
        return -1;
    }

    // Check if the second argument is a valid number of lists
    int n_lists = atoi(argv[2]);
    if (n_lists <= 0) {
        printf("Invalid number of lists\n");
        printf("Usage: ./table_server <port> <n_lists> <zk_ip:port>\n");
        return -1;
    }

    char *zk_ip_port = argv[3];

    // Start server
    sockfd = network_server_init(port, zk_ip_port);

    if (sockfd < 0) {
        printf("Error starting server\n");
        return -1;
    }

    printf("Server ready, waiting for connections\n");

    // Initialize table
    table = table_skel_init(n_lists);

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


    return 0;
}



