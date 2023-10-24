/* 

Sistemas Distribuidos - Projeto Fase 2
Grupo 8
Alexandre Pinto - 55958
Eduardo Proença  - 57551
Tiago Oliveira - 54979

*/

#include <stdlib.h>
#include <string.h>


#include "client_stub-private.h"


struct rtable_t *rtable_connect(char *address_port) {
    if (address_port == NULL) {
        return NULL;
    }

    // Parse address and port
    char *address = strtok(address_port, ":");
    char *port = strtok(NULL, ":");
    if (address == NULL || port == NULL) {
        return NULL;
    }

    // Allocate memory for rtable
    struct rtable_t *rtable = (struct rtable_t *) malloc(sizeof(struct rtable_t));
    if (rtable == NULL) {
        return NULL;
    }

    // Set server address and port
    rtable->server_address = strdup(address);
    rtable->server_port = atoi(port);

    // Connect to server
    rtable->sockfd = network_connect(rtable);

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
    if (rtable == NULL || entry == NULL) {
        return -1;
    }

    // Create message
    MessageT *msg = (MessageT *) malloc(sizeof(MessageT));
    if (msg == NULL) {
        return -1;
    }

    message_t__init(msg);

    msg->opcode = MESSAGE_T__OPCODE__OP_PUT;
    msg->c_type = MESSAGE_T__C_TYPE__CT_ENTRY;

    // Create entry
    EntryT *entry_msg = (EntryT *) malloc(sizeof(EntryT));
    if (entry_msg == NULL) {
        message_t__free_unpacked(msg, NULL);
        return -1;
    }

    entry_t__init(entry_msg);
  

    msg->entry = entry_msg;

    // Send message
    MessageT *response = network_send_receive(rtable, msg);

    // Free message
    message_t__free_unpacked(msg, NULL);

    if (response == NULL) {
        return -1;
    }

    // Check response
    if (response->opcode == MESSAGE_T__OPCODE__OP_ERROR) {
        message_t__free_unpacked(response, NULL);
        return -1;
    }

    message_t__free_unpacked(response, NULL);

    return 0;
    
};

struct data_t *rtable_get(struct rtable_t *rtable, char *key) {
    if (rtable == NULL || key == NULL) {
        return NULL;
    }

    MessageT *msg = (MessageT *)malloc(sizeof(MessageT));
    message_t__init(msg);

    // Sends the request
    msg->opcode = MESSAGE_T__OPCODE__OP_GET;
    msg->c_type = MESSAGE_T__C_TYPE__CT_KEY;
    msg->key = strdup(key);

    MessageT *response = network_send_receive(rtable, msg);
    if (msg->key != NULL) {
        free(msg->key);
    }
    free(msg);

    if (response == NULL) {
        return NULL;
    }

    if (response->opcode == MESSAGE_T__OPCODE__OP_ERROR) {
        message_t__free_unpacked(response, NULL);
        return NULL;
    }

    if (response->c_type == MESSAGE_T__C_TYPE__CT_VALUE) {
        // Returns the data from the response
        struct data_t *data = (struct data_t *)malloc(sizeof(struct data_t));
        data->data = malloc(response->value.len);
        data->datasize = response->value.len;
        memcpy(data->data, response->value.data, data->datasize);

        message_t__free_unpacked(response, NULL);
        return data;
    } else {
        message_t__free_unpacked(response, NULL);
        return NULL;
    }
}

int rtable_del(struct rtable_t *rtable, char *key) {
    if (rtable == NULL || key == NULL) {
        return -1;
    }

    MessageT *msg = (MessageT *)malloc(sizeof(MessageT));
    message_t__init(msg);

    // Sends the request
    msg->opcode = MESSAGE_T__OPCODE__OP_DEL;
    msg->c_type = MESSAGE_T__C_TYPE__CT_KEY;
    msg->key = strdup(key);

    MessageT *response = network_send_receive(rtable, msg);
    if (msg->key != NULL) {
        free(msg->key);
    }
    free(msg);

    if (response == NULL) {
        return -1;
    }

    if (response->opcode == MESSAGE_T__OPCODE__OP_ERROR) {
        message_t__free_unpacked(response, NULL);
        return -1;
    }
    
    if (response->opcode == MESSAGE_T__OPCODE__OP_DEL &&
        response->c_type == MESSAGE_T__C_TYPE__CT_RESULT) {
        message_t__free_unpacked(response, NULL);
        return 0;
    } else {
        message_t__free_unpacked(response, NULL);
        return -1;
    }

}

char **rtable_get_keys(struct rtable_t *rtable) {
    if (rtable == NULL) {
        return NULL;
    }

    MessageT *msg = (MessageT *)malloc(sizeof(MessageT));
    message_t__init(msg);

    // Sends the request
    msg->opcode = MESSAGE_T__OPCODE__OP_GETKEYS;
    msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;

    MessageT *response = network_send_receive(rtable, msg);
    free(msg);

    if (response == NULL) {
        return NULL;
    }

    if (response->opcode == MESSAGE_T__OPCODE__OP_ERROR) {
        message_t__free_unpacked(response, NULL);
        return NULL;
    }

    if (response->opcode == MESSAGE_T__OPCODE__OP_GETKEYS &&
        response->c_type == MESSAGE_T__C_TYPE__CT_KEYS) {
        // Returns the keys from the response
        char **keys = (char **)malloc(sizeof(char *) * (response->n_keys + 1));
        for (int i = 0; i < response->n_keys; i++) {
            keys[i] = strdup(response->keys[i]);
        }
        keys[response->n_keys] = NULL;

        message_t__free_unpacked(response, NULL);
        return keys;
    } else {
        message_t__free_unpacked(response, NULL);
        return NULL;
}