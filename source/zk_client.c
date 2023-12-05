/*

Sistemas Distribuidos - Projeto Fase 4
Grupo 8
Alexandre Pinto - 55958
Eduardo Proença  - 57551
Tiago Oliveira - 54979

*/

#include "zk_client.h"

struct rtable_pair_t *connections;

typedef struct String_vector zoo_string;
struct String_vector children;

struct rtable_pair_t *zk_init(char *address_port) {
    zoo_set_debug_level(ZOO_LOG_LEVEL_ERROR);

    // Allocate memory for connections
    connections = (struct rtable_pair_t *)malloc(sizeof(struct rtable_pair_t));
    connections->head_name = NULL;
    connections->tail_name = NULL;
    connections->read = NULL;
    connections->write = NULL;

    /* INITIALIZE ZOOKEEPER CONNECTION */

    zh = zookeeper_init(address_port, zk_connection_watcher, 10000, 0, 0, 0);

    if (zh == NULL) {
        printf("Error connecting to zookeeper\n");
        exit(-1);
    }

    sleep(3); // Wait for connection to be established

    if (is_connected) {
        printf("Connected to zookeeper!\n");

        // Check if chain node exists
        if(ZNONODE == zoo_exists(zh, root_path, 0, NULL)) {
           
            fprintf(stderr, "No servers available!\n");
            return NULL;
        
        }

        // Get list of children nodes
        zoo_string *children_list = (zoo_string *)malloc(sizeof(zoo_string));
        int zoo_data_len = ZDATALEN;
        if (ZOK != zoo_wget_children(zh, root_path, zk_children_handler, NULL, children_list)) {
            fprintf(stderr, "Error getting children of znode %s!\n", root_path);
            return NULL;
        }

        printf("Got available servers!\n");

        // If no children, return NULL
        if (children_list->count == 0) {
            fprintf(stderr, "No servers available!\n");
            return NULL;
        }

        // Get head and tail of the list
        char *tail = zk_get_tail(children_list->data, children_list->count);
        char *head = zk_get_head(children_list->data, children_list->count);

        printf("Got head and tail!\n");

        // Make connections for read and write
        int connected = make_connections(head, tail, connections);

        printf("Made connections!\n");

        if (connected == -1) {
            fprintf(stderr, "Error connecting to servers!\n");
            return NULL;
        }

        return connections;

    }
}

void zk_connection_watcher(zhandle_t *zzh, int type, int state, const char *path, void *watcherCtx) {
    if (type == ZOO_SESSION_EVENT) {
        if (state == ZOO_CONNECTED_STATE) {
            is_connected = 1;
        } else {
            fprintf(stderr, "Session expired. Shutting down...\n");
            zookeeper_close(zh);
            exit(-1);
        }
    }
}

void zk_children_handler(zhandle_t *zh, int type, int state, const char *path, void *watcherCtx) {
    zoo_string *children_list = (zoo_string *)malloc(sizeof(zoo_string));
    int zoo_data_len = ZDATALEN;

    if (type == ZOO_CHILD_EVENT) {
        if (state == ZOO_CONNECTED_STATE) {
        
            // Get list of children nodes and set watcher
            if (ZOK != zoo_wget_children(zh, root_path, zk_children_handler, watcherCtx, children_list)) {
                fprintf(stderr, "Error getting children of znode %s!\n", root_path);
                return;
            }

            // Get head and tail of the list
            char *tail = zk_get_tail(children_list->data, children_list->count);
            char *head = zk_get_head(children_list->data, children_list->count);

            // Make connections for read and write
            int connected = make_connections(head, tail, connections);

            if (connected == -1) {
                fprintf(stderr, "Error connecting to servers!\n");
                return;
            }

    
            //free(tail);
            //free(head);

    }
    }
}


char *zk_get_tail(char **node_list, int size) {
    // Get the node with the highest id
    char *tail = node_list[0];
    int i;
    for (i = 1; i < size; i++) {
        if (strcmp(node_list[i], tail) > 0) {
            tail = node_list[i];
        }
    }

    return tail;
}

char *zk_get_head(char **node_list, int size) {
    // Get the node with the lowest id
    char *head = node_list[0];
    int i;
    for (i = 1; i < size; i++) {
        if (strcmp(node_list[i], head) < 0) {
            head = node_list[i];
        }
    }

    return head;
}

char **zk_get_data(char *node_name) {
    // Allocate memory for node path
    char *node_path = (char *)malloc(strlen(root_path) + strlen("/") + strlen(node_name) + 1);

    // Create node path
    strcpy(node_path, root_path);
    strcat(node_path, "/");
    strcat(node_path, node_name);

    // Allocate memory for node data
    char *node_data = malloc(ZDATALEN * sizeof(char));
    int zoo_data_len = ZDATALEN;

    // Get data from node
    if (ZOK != zoo_get(zh, node_path, 0, node_data, &zoo_data_len, NULL)) {
        fprintf(stderr, "Error getting data from znode %s!\n", node_path);
        return NULL;
    }

    free(node_path);


    return node_data;
}


int make_connections(char *head_name, char *tail_name, struct rtable_pair_t *rtable_pair) {
    printf("Making connections\n");

    // Check if rtable_pair is null
    if (rtable_pair == NULL) {
        printf("rtable_pair is NULL\n");
        return -1;
    }

    // If current head is null or different from the new head, connect to the new head
    if (rtable_pair->head_name == NULL || strcmp(rtable_pair->head_name, head_name) != 0) {
        printf("Head is null\n");
        printf("Head name: %s\n", head_name);

        char *address = zk_get_data(head_name);

        printf("Got data from head\n");
        printf("Address: %s\n", address);

        // Save the new connection in the rtable_pair and update the head name
        rtable_pair->write = rtable_connect(address);
        rtable_pair->head_name = head_name;
    } 

    // If current tail is null or different from the new tail, connect to the new tail
    if (rtable_pair->tail_name == NULL || strcmp(rtable_pair->tail_name, tail_name) != 0) {
        printf("Tail is null\n");
        printf("Tail name: %s\n", tail_name);

        char *address = zk_get_data(tail_name);

        printf("Got data from tail\n");
        printf("Address: %s\n", address);

        // Save the new connection in the rtable_pair and update the tail name
        rtable_pair->read = rtable_connect(address);
        rtable_pair->tail_name = tail_name;
    }

    return 0;
}