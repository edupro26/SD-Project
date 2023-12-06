/* 

Sistemas Distribuidos - Projeto Fase 4
Grupo 8
Alexandre Pinto - 55958
Eduardo Proença  - 57551
Tiago Oliveira - 54979

*/

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



#include "network_client.h"
#include "message-private.h"
#include "client_stub-private.h"


int network_connect(struct rtable_t *rtable) {
    // Verify if rtable is NULL
    if (rtable == NULL) {
        return -1;
    }

    // Verify if server address or port 
    if (rtable->server_address == NULL || rtable->server_port < 0) {
        return -1;
    }

    // Create socket struct
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(rtable->server_port);

    if (inet_pton(AF_INET, rtable->server_address, &server_addr.sin_addr) <= 0) {
        perror("inet_pton");
        return -1;
    }

    // Create socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return -1;
    }

    printf("Connecting to %s:%d\n", rtable->server_address, rtable->server_port);

    // Connect to server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        close(sockfd);
        return -1;
    }

    printf("Connected!\n");

    rtable->sockfd = sockfd;

    return 0;

}

MessageT *network_send_receive(struct rtable_t *rtable, MessageT *msg) {
    // Verify if rtable or msg are NULL
    if (rtable == NULL || msg == NULL) {
        return NULL;
    }

    int sockfd = rtable->sockfd; 
    unsigned char *buf = NULL;
    size_t len;
    ssize_t nbytes;

    // Message serialization
    len = message_t__get_packed_size(msg);
    buf = malloc(len);
    if (buf == NULL) {
        return NULL;
    }
    message_t__pack(msg, buf);

    printf("Sending message size\n");

    // Send message size (2 bytes)
    int16_t size = htons(len);
    if (write_all(sockfd, &size, sizeof(short)) != sizeof(short)) {
        perror("Error sending message size");
        free(buf);
        return NULL;
    }

    printf("Sending message\n");

    // Send serialized message
    nbytes = write_all(sockfd, buf, len);
    free(buf);  // Free buffer
    if ((size_t)nbytes != len) {
        printf("Error sending serialized message\n");
        return NULL;
    }

    printf("Message sent\n");

    // Receive response size (2 bytes)
    if (read_all(sockfd, &size, sizeof(short)) != sizeof(short)) {
        printf("Error receiving response size\n");
        return NULL;
    }

    printf("Received response size\n");

    len = ntohs(size);

    // Receive serialized response
    buf = malloc(len);
    if (buf == NULL) {
        return NULL;
    }
    nbytes = read_all(sockfd, buf, len);
    if ((size_t)nbytes != len) {
        printf("Error receiving serialized response\n");
        free(buf);
        return NULL;
    }

    printf("Received response\n");

    // Deserialize response
    MessageT *response = message_t__unpack(NULL, len, buf);
    free(buf);  // Free buffer

    return response;
}

int network_close(struct rtable_t *rtable) {
    // Verify if rtable is NULL
    if (rtable == NULL) {
        return -1;
    }

    // Close socket
    if (close(rtable->sockfd) == -1) {
        return -1;
    }

    return 0;  
}