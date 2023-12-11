/*

Sistemas Distribuidos - Projeto Fase 4
Grupo 8
Alexandre Pinto - 55958
Eduardo Proença  - 57551
Tiago Oliveira - 54979

*/


#include <signal.h>

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
#include <pthread.h>
#include <sys/time.h>

#include "network_server.h"
#include "message-private.h"
#include "table_skel.h"
#include "stats.h"
#include "locks.h"
#include "zk_server.h"

// Create a variable to sore the table_t pointer
struct table_t *table_ptr;
struct locks_t *locks_stats_ptr;

struct ClientSocketNode {
    int* i;
    pthread_t thr;
    struct ClientSocketNode* next;
};

struct ClientSocketNode* client_socket_list = NULL;

// Save port as global variable to pass to the zookeeper
short server_port;

char *zk_address;


/* Function to retrieve the ip or hostmane of the machine  */
char *get_ip() {
    char hostname[1024];
    char *ip = malloc(16);
    if (gethostname(hostname, 1024) < 0) {
        perror("Failed to get hostname");
        return NULL;
    }

    struct hostent *host_entry;
    if ((host_entry = gethostbyname(hostname)) == NULL) {
        perror("Failed to get host by name");
        return NULL;
    }

    if (inet_ntop(AF_INET, host_entry->h_addr_list[0], ip, 16) == NULL) {
        perror("Failed to convert ip to string");
        return NULL;
    }

    return ip;
}


int network_server_init(short port, char *zk_ip_port) {
    int sockfd;
    int opt = 1;  // option for setsockopt
    struct sockaddr_in address;

    // Save the zk_ip_port
    zk_address = zk_ip_port;

    stats = init_statistics();
    locks_stats_ptr = init_lock();

    

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        return -1;
    }

    // Set socket options to reuse the address
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        close(sockfd);
        return -1;
    }

    server_port = port;


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

void *handle_client(void *arg) {

    writeLock(locks_stats_ptr);
    stats->clients++;
    leaveWrite(locks_stats_ptr);

    MessageT *request;
    int client_socket = *(int *)arg;

    while (1) {

        // Receive a message from the client
        request = network_receive(client_socket);
        if (request == NULL) {
            close(client_socket);
            break;
        }

        // Count the time invoke takes with gettimeofday()
        struct timeval start, end;
        gettimeofday(&start, NULL);

        // Process the message received on table_skel
        int response = invoke(request, table_ptr);

        gettimeofday(&end, NULL);
        // Add the time to the stats in microseconds
        int timeToAdd = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);

        // Only and time if the it is not a stats operation
        if (request->opcode != MESSAGE_T__OPCODE__OP_STATS + 1) {
            writeLock(locks_stats_ptr);
            stats->time += timeToAdd;
            leaveWrite(locks_stats_ptr);
        }

        if (response < 0) {
            perror("Failed to process client message");
            close(client_socket);
            break;
        }

        // Send the response to the client
        if (network_send(client_socket, request) < 0) {
            perror("Failed to send response to client");
        }

    }

    // Close client socket
    close(client_socket);
    
    writeLock(locks_stats_ptr);
    stats->clients--;
    leaveWrite(locks_stats_ptr);

    printf("Client connection closed\n");

    // Exit thread
    pthread_exit(NULL);

    return 0;   
}

int network_main_loop(int listening_socket, struct table_t *table) {
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_socket;
    // Save the table_t pointer
    table_ptr = table;

    char *ip = get_ip();

    // Initialize the zookeeper, passing the ip:port and pointer to the table_t
    zk_init(ip, server_port, table_ptr, zk_address);

    free(ip);

    printf("Zookeeper initialized\n");

    while ((client_socket = accept(listening_socket, (struct sockaddr*)&client_addr, &client_len)) != -1) { // Keep the server running

        struct ClientSocketNode* new_node = (struct ClientSocketNode*)malloc(sizeof(struct ClientSocketNode));

        if (new_node == NULL) {
        perror("malloc");
        close(client_socket); // Close the new client socket on error
        return -1;
        }    

        printf("Client connection established\n");
        int *i = malloc(sizeof(int));
        *i = client_socket;
        new_node->i = i;
        pthread_create(&new_node->thr, NULL, &handle_client, new_node->i);
        
        // Save the node with the thread and socket id to ensure the liberation of resources 
        // when the server terminates
        new_node->next = client_socket_list;
        client_socket_list = new_node;
        


        
    }

    return 0;
}



MessageT *network_receive(int client_socket) {
    int bytes_read;

    // Read the size of the incoming message (2 bytes)
    int16_t message_size;
    bytes_read = read_all(client_socket, &message_size, sizeof(int16_t));

    if (bytes_read <= 0 || message_size <= 0)
        return NULL;

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

    //  If op is not STATS, increment the number of operations
   if (message->opcode != MESSAGE_T__OPCODE__OP_STATS) {
        writeLock(locks_stats_ptr);
        stats->ops++;
        leaveWrite(locks_stats_ptr);
   }
    // TODO: check if an operation that result in an error should increment the number of operations

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
    message_t__free_unpacked(msg, NULL);

    // Send the size of the message (2 bytes)
    int16_t size_to_send = htons(message_size);
    if (write_all(client_socket, &size_to_send, sizeof(int16_t)) != sizeof(int16_t)) {
        perror("Failed to send the size of the message");
        free(buffer);
        return -1;
    }

    // Send message
    if (write_all(client_socket, buffer, message_size) != (ssize_t) message_size) {
        perror("Failed to send the serialized message");
        free(buffer);
        return -1;
    }

    free(buffer);
    return 0;
}

int network_server_close(int socket) {

    struct ClientSocketNode* current = client_socket_list;

    while (current != NULL) {
        

        struct ClientSocketNode* temp = current;
        current = current->next;
        

        if (temp->i != NULL) {
                // Order the thread to close 
                pthread_cancel(temp->thr);
                
                // Wait for the thread to close and liberate its resources
                pthread_join(temp->thr, NULL);

            free(temp->i);
        }

        free(temp);
    }


    // Close socket
    if (close(socket) < 0) {
        perror("Failed to close the socket");
        return -1;
    }

    // Destroy locks
    destroy_lock(locks_stats_ptr);

    // Destroy stats
    destroy_statistics();

    return 0;
}
