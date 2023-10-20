/* 

Sistemas Distribuidos - Projeto Fase 2
Grupo 8
Alexandre Pinto - 55958
Eduardo Proença  - 57551
Tiago Oliveira - 54979

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "network_client.h"

int network_connect(struct rtable_t *rtable) {
    struct sockaddr_in server;

    // Create socket
    if ((rtable->socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Erro ao criar socket");
        return -1;
    } 

    server.sin_family = AF_INET;
    server.sin_port = htons(rtable->server_port); // Set server port

    // Convert IP
    if (inet_pton(AF_INET, rtable->server_address, &server.sin_addr) < 1) {
        printf("Erro ao converter IP\n");
        close(rtable->sockfd);
        return -1;
    }

    // Make connection
    if (connect(sockfd,(struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Erro ao conectar-se ao servidor");
        close(sockfd);
        return -1;
    }

    return 0;
}

MessageT *network_send_receive(struct rtable_t *rtable, MessageT *msg); {
    
}