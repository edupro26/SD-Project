/*

Sistemas Distribuidos - Projeto Fase 2
Grupo 8
Alexandre Pinto - 55958
Eduardo Proença  - 57551
Tiago Oliveira - 54979

*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "data.h"
#include "client_stub-private.h"


struct rtable_t *rtable_connect(char *address_port) {
    if (address_port == NULL)
        return NULL;

    // Parse address and port
    char *address = strtok(address_port, ":");
    char *port = strtok(NULL, ":");
    if (address == NULL || port == NULL) {
        printf("Usage: ./table_client <server>:<port>\n");
        return NULL;
    }

    // Allocate memory for rtable
    struct rtable_t *rtable = (struct rtable_t *) malloc(sizeof(struct rtable_t));
    if (rtable == NULL)
        return NULL;

    // Set server address and port
    char hostbuff[256];
    int name = gethostname(hostbuff, sizeof(hostbuff));
    if (name < -0){
        perror("Error connecting to server");
        free(rtable->server_address);
        free(rtable);
        return NULL;
    }

    if(strcmp(address, hostbuff) == 0){
        char *IPbuff;
        struct hostent *host = gethostbyname(hostbuff);
        if (host == NULL) {
            perror("Error connecting to server");
            free(rtable->server_address);
            free(rtable);
            return NULL;
        }
        IPbuff = inet_ntoa(*((struct in_addr*)host->h_addr_list[0]));
        rtable->server_address = strdup(IPbuff);
    }
    else{
        rtable->server_address = strdup(address);
    }
    rtable->server_port = atoi(port);

    if (network_connect(rtable) < 0) {
        perror("Error connecting to server");
        free(rtable->server_address);
        free(rtable);
        return NULL;
    }

    if (rtable->sockfd < 0) {
        free(rtable->server_address);
        free(rtable);
        return NULL;
    }

    return rtable;
}

int rtable_disconnect(struct rtable_t *rtable) {
    if (rtable == NULL) {
        return -1;
    }

    int cls = network_close(rtable);

    if (cls < 0) {
        return -1;
    }

    free(rtable->server_address);
    free(rtable);

    return 0;
}

int rtable_put(struct rtable_t *rtable, struct entry_t *entry){
    if (rtable == NULL || entry == NULL)
        return -1;

    // Create message
    MessageT msg;
    message_t__init(&msg);
    msg.opcode = MESSAGE_T__OPCODE__OP_PUT;
    msg.c_type = MESSAGE_T__C_TYPE__CT_ENTRY;
    msg.key = entry->key;
    msg.value.len = entry->value->datasize;
    msg.value.data = entry->value->data;

    // Sends the request
    MessageT *response = network_send_receive(rtable, &msg);

    free(entry->value);
    free(entry);

    // Check response
    if (response->opcode == MESSAGE_T__OPCODE__OP_ERROR) {
        message_t__free_unpacked(response, NULL);
        return -1;
    }
    if (response == NULL) {
        message_t__free_unpacked(response, NULL);
        return -1;
    }
    
    if (response->opcode == MESSAGE_T__OPCODE__OP_PUT + 1 && response->c_type == MESSAGE_T__C_TYPE__CT_NONE) {
        message_t__free_unpacked(response, NULL);
        return 0;
    } 
    else {
        message_t__free_unpacked(response, NULL);
        return -1;
    }
}

struct data_t *rtable_get(struct rtable_t *rtable, char *key) {
    if (rtable == NULL || key == NULL)
        return NULL;

    MessageT msg;// = (MessageT *)malloc(sizeof(MessageT));
    message_t__init(&msg);
    msg.opcode = MESSAGE_T__OPCODE__OP_GET;
    msg.c_type = MESSAGE_T__C_TYPE__CT_KEY;
    msg.key = key;

    // Sends the request
    MessageT *response = network_send_receive(rtable, &msg);

    // Check response
    if (response == NULL) {
        message_t__free_unpacked(response, NULL);
        return NULL;
    }
    if (response->opcode == MESSAGE_T__OPCODE__OP_ERROR) {
        message_t__free_unpacked(response, NULL);
        return NULL;
    }

    if (response->opcode == MESSAGE_T__OPCODE__OP_GET+1 && response->c_type == MESSAGE_T__C_TYPE__CT_VALUE) {
        struct data_t *data_temp = data_create(response->value.len, response->value.data);
        struct data_t *data = data_dup(data_temp);
        free(data_temp);
        message_t__free_unpacked(response, NULL);
        return data;
    } else {
        message_t__free_unpacked(response, NULL);
        return NULL;
    }
}

int rtable_del(struct rtable_t *rtable, char *key) {
    if (rtable == NULL || key == NULL)
        return -1;

    MessageT msg;
    message_t__init(&msg);
    msg.opcode = MESSAGE_T__OPCODE__OP_DEL;
    msg.c_type = MESSAGE_T__C_TYPE__CT_KEY;
    msg.key = key;

    // Sends the request
    MessageT *response = network_send_receive(rtable, &msg);

    // Check response
    if (response == NULL) {
        return -1;
    }
    if (response->opcode == MESSAGE_T__OPCODE__OP_ERROR) {
        message_t__free_unpacked(response, NULL);
        return -1;
    }

    if (response->opcode == MESSAGE_T__OPCODE__OP_DEL + 1 && response->c_type == MESSAGE_T__C_TYPE__CT_NONE) {
        message_t__free_unpacked(response, NULL);
        return 0;
    } else {
        message_t__free_unpacked(response, NULL);
        return -1;
    }
}

int rtable_size(struct rtable_t *rtable) {
    if (rtable == NULL)
        return -1;

    MessageT msg;// = (MessageT *)malloc(sizeof(MessageT));
    message_t__init(&msg);
    msg.opcode = MESSAGE_T__OPCODE__OP_SIZE;
    msg.c_type = MESSAGE_T__C_TYPE__CT_NONE;

    // Sends the request
    MessageT *response = network_send_receive(rtable, &msg);

    // Check response
    if (response == NULL) {
        message_t__free_unpacked(response, NULL);
        return -1;
    }
    if (response->opcode == MESSAGE_T__OPCODE__OP_ERROR) {
        message_t__free_unpacked(response, NULL);
        return -1;
    }

    if (response->opcode == MESSAGE_T__OPCODE__OP_SIZE + 1 && response->c_type == MESSAGE_T__C_TYPE__CT_RESULT) {
        int size = response->result;
        message_t__free_unpacked(response, NULL);
        return size;
    } else {
        message_t__free_unpacked(response, NULL);
        return -1;
    }
}

char **rtable_get_keys(struct rtable_t *rtable) {
    if (rtable == NULL)
        return NULL;

    MessageT msg;
    message_t__init(&msg);
    msg.opcode = MESSAGE_T__OPCODE__OP_GETKEYS;
    msg.c_type = MESSAGE_T__C_TYPE__CT_NONE;

    // Sends the request
    MessageT *response = network_send_receive(rtable, &msg);

    // Check response
    if (response == NULL) {
        message_t__free_unpacked(response, NULL);
        return NULL;
    }
    if (response->opcode == MESSAGE_T__OPCODE__OP_ERROR) {
        message_t__free_unpacked(response, NULL);
        return NULL;
    }

    if (response->opcode == MESSAGE_T__OPCODE__OP_GETKEYS+1 && response->c_type == MESSAGE_T__C_TYPE__CT_KEYS) {
        char **keys = (char **) malloc((response->n_keys + 1) * sizeof(char *));
        for (int i = 0; i < (int)response->n_keys; i++) {
            keys[i] = strdup(response->keys[i]);
        }
        keys[response->n_keys] = NULL;
        message_t__free_unpacked(response, NULL);
        return keys;
    } else {
        message_t__free_unpacked(response, NULL);
        return NULL;
    }
}

void rtable_free_keys(char **keys) {
    if (keys == NULL) {
        return;
    }

    for (int i = 0; keys[i] != NULL; i++) {
        free(keys[i]);
    }

    free(keys);
}

struct entry_t **rtable_get_table(struct rtable_t *rtable) {
    if (rtable == NULL)
        return NULL;

    MessageT msg;
    message_t__init(&msg);
    msg.opcode = MESSAGE_T__OPCODE__OP_GETTABLE;
    msg.c_type = MESSAGE_T__C_TYPE__CT_NONE;

    // Sends the request
    MessageT *response = network_send_receive(rtable, &msg);

    // Check response
    if (response == NULL) {
        message_t__free_unpacked(response, NULL);
        return NULL;
    }
    if (response->opcode == MESSAGE_T__OPCODE__OP_ERROR) {
        message_t__free_unpacked(response, NULL);
        return NULL;
    }

    if (response->opcode == MESSAGE_T__OPCODE__OP_GETTABLE+1 && response->c_type == MESSAGE_T__C_TYPE__CT_TABLE) {
        struct entry_t **entries = (struct entry_t **)malloc(sizeof(struct entry_t *) * (response->n_entries + 1));
        if (entries == NULL) {
            message_t__free_unpacked(response, NULL);
            return NULL;
        }

        for (int i = 0; i < (int)response->n_entries; i++) {
            struct data_t *data_temp = data_create(response->entries[i]->value.len, response->entries[i]->value.data);
            struct data_t *data = data_dup(data_temp);
            char *key = strdup(response->entries[i]->key);
            entries[i] = entry_create(key, data);
            free(data_temp);
        }    
        entries[response->n_entries] = NULL;

        message_t__free_unpacked(response, NULL);
        return entries;
    } else {
        message_t__free_unpacked(response, NULL);
        return NULL;
    }
}

void rtable_free_entries(struct entry_t **entries) {
    if (entries == NULL) {
        return;
    }

    for (int i = 0; entries[i] != NULL; i++) {
        entry_destroy(entries[i]);
    }

    free(entries);
}
