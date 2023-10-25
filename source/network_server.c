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
#include "message-private.h"
#include "table_skel.h"



int network_server_init(short port) {
    int sockfd;
    int opt = 1;  // option for setsockopt
    struct sockaddr_in address;

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        return -1;
    }

    // Fill in address struct for binding
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // Bind socket
    if (bind(sockfd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(sockfd);
        return -1;
    }

    // Listen for incoming connections
    if (listen(sockfd, 5) < 0) {  // backlog of 5, which means up to 5 pending connections will be queued
        perror("listen");
        close(sockfd);
        return -1;
    }

    return sockfd;
}

int network_main_loop(int listening_socket, struct table_t *table) {
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_socket;
    MessageT *request;

    while ((client_socket = accept(listening_socket, (struct sockaddr*)&client_addr, &client_len)) != -1) { // Keep the server running

        printf("New client connected\n");

        while (1) {

            // Receive a message from the client
            request = network_receive(client_socket);
        if (!request) {
            perror("Failed to receive message from client");
            close(client_socket);
            break;
        }

        // Process the message received on table_skel
        int response = invoke(request, table);

        if (response < 0) {
            perror("Failed to process client message");
            close(client_socket);
            break;
        }

        // Send the response to the client
        if (network_send(client_socket, request) < 0) {
            perror("Failed to send response to client");
        }
        message_t__free_unpacked(response, NULL);  // Free the memory of the generated response

        }

        // Close client socket
        close(client_socket);
        
    }

    return 0;
}

MessageT *network_receive(int client_socket) {
    int bytes_read;

    // Read the size of the incoming message (2 bytes)
    int16_t message_size;
    bytes_read = read_all(client_socket, &message_size, sizeof(int16_t));

    if (bytes_read < sizeof(int16_t) || message_size <= 0) {
        perror("Failed to read the size of the incoming message or invalid size");
        return NULL;
    }

    // Allocate a buffer based on the size of the incoming message
    uint8_t *buffer = malloc(ntohs(message_size));
    if (!buffer) {
        perror("Failed to allocate memory for the incoming message");
        return NULL;
    }

    // Read message into the buffer
    bytes_read = read_all(client_socket, buffer, ntohs(message_size));

    if (bytes_read != htons(message_size)) {
        perror("Failed to read the full message from the client");
        free(buffer);
        return NULL;
    }

    // Deserialize the message
    MessageT *message = message_t__unpack(NULL, bytes_read, buffer);

    // Free the buffer
    free(buffer);

    if (!message) {
        perror("Failed to unpack the received message");
        return NULL;
    }

    return message;

}

int network_send(int client_socket, MessageT *msg) {
    if (!msg) {
        perror("Message is NULL");
        return -1;
    }

    // Serialize the message
    size_t message_size = message_t__get_packed_size(msg);
    uint8_t *buffer = malloc(message_size);
    if (!buffer) {
        perror("Failed to allocate memory for serialized message");
        return -1;
    }

    message_t__pack(msg, buffer);

    // Send the size of the message (2 bytes)
    int16_t size_to_send = htons(message_size);
    if (write_all(client_socket, &size_to_send, sizeof(int16_t)) != sizeof(int16_t)) {
        perror("Failed to send the size of the message");
        free(buffer);
        return -1;
    }

    // Send message
    if (write_all(client_socket, buffer, message_size) != message_size) {
        perror("Failed to send the serialized message");
        free(buffer);
        return -1;
    }

    free(buffer);
    return 0;
}

int network_server_close(int socket) {

    // Close socket
    if (close(socket) < 0) {
        perror("Failed to close the socket");
        return -1;
    }

    return 0;
}