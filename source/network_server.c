#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "network_server.h"



int network_server_init(short port) {
    int sockfd;
    int opt = 1;  // option for setsockopt
    struct sockaddr_in address;

    // 1. Socket Creation
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        return -1;
    }

    // 2. Set Socket Options to reuse address and port
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        close(sockfd);
        return -1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // 3. Bind the socket to the specified port
    if (bind(sockfd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(sockfd);
        return -1;
    }

    // 4. Listen for incoming connections
    if (listen(sockfd, 5) < 0) {  // backlog of 5, which means up to 5 pending connections will be queued
        perror("listen");
        close(sockfd);
        return -1;
    }

    return sockfd;
}

int network_main_loop(int listening_socket, struct table_t *table) {
    int client_socket;
    MessageT *msg;
    MessageT *response;
    int result;


    return 0;
}

MessageT *network_receive(int client_socket) {
    int msg_size;
    char *buffer;
    MessageT *msg;

   return NULL;
}

int network_send(int client_socket, MessageT *msg) {
    int msg_size;
    char *buffer;

    

    return 0;
}

int network_server_close(int socket) {
    if (close(socket) < 0) {
        perror("close");
        return -1;
    }

    return 0;
}