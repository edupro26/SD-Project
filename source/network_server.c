#include "network_server.h"
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
/* Função para preparar um socket de receção de pedidos de ligação
 * num determinado porto.
 * Retorna o descritor do socket ou -1 em caso de erro.
 */
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

    while (1) {
        // 1. Accept a connection from a client
        if ((client_socket = accept(listening_socket, NULL, NULL)) < 0) {
            perror("accept");
            return -1;
        }

        // 2. Receive a message from the client
        if ((msg = network_receive(client_socket)) == NULL) {
            perror("network_receive");
            close(client_socket);
            return -1;
        }

        // 3. Process the message
        if ((response = invoke(table, msg)) == NULL) {
            perror("invoke");
            close(client_socket);
            return -1;
        }

        // 4. Send the response to the client
        if ((result = network_send(client_socket, response)) < 0) {
            perror("network_send");
            close(client_socket);
            return -1;
        }

        // 5. Free message and response
        free_message(msg);
        free_message(response);

        // 6. Close client socket
        close(client_socket);
    }

    return 0;
}

MessageT *network_receive(int client_socket) {
    int msg_size;
    char *buffer;
    MessageT *msg;

    // 1. Read message size
    if (read_all(client_socket, (char *)&msg_size, _INT) < 0) {
        perror("read_all");
        return NULL;
    }

    // 2. Allocate memory for message
    if ((buffer = (char *)malloc(msg_size)) == NULL) {
        perror("malloc");
        return NULL;
    }

    // 3. Read message
    if (read_all(client_socket, buffer, msg_size) < 0) {
        perror("read_all");
        free(buffer);
        return NULL;
    }

    // 4. Deserialize message
    msg = buffer_to_message(buffer, msg_size);

    // 5. Free buffer
    free(buffer);

    return msg;
}

int network_send(int client_socket, MessageT *msg) {
    int msg_size;
    char *buffer;

    // 1. Serialize message
    if ((buffer = message_to_buffer(msg, &msg_size)) == NULL) {
        perror("message_to_buffer");
        return -1;
    }

    // 2. Send message size
    if (write_all(client_socket, (char *)&msg_size, _INT) < 0) {
        perror("write_all");
        free(buffer);
        return -1;
    }

    // 3. Send message
    if (write_all(client_socket, buffer, msg_size) < 0) {
        perror("write_all");
        free(buffer);
        return -1;
    }

    // 4. Free buffer
    free(buffer);

    return 0;
}

int network_server_close(int socket) {
    if (close(socket) < 0) {
        perror("close");
        return -1;
    }

    return 0;
}