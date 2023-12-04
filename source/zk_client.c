/*

Sistemas Distribuidos - Projeto Fase 4
Grupo 8
Alexandre Pinto - 55958
Eduardo Proença  - 57551
Tiago Oliveira - 54979

*/

#include "zk_client.h"

struct rtable_pair_t *connections = NULL;

typedef struct String_vector zoo_string;
struct String_vector children;

struct rtable_pair_t *zk_init(char *address_port) {
    zoo_set_debug_level(ZOO_LOG_LEVEL_ERROR);


    zh = zookeeper_init(address_port, zk_connection_watcher, 10000, 0, 0, 0);

    if (zh == NULL) {
        printf("Error connecting to zookeeper\n");
        exit(-1);
    }

    sleep(3); // Wait for connection to be established

    if (is_connected) {
        if(ZNONODE == zoo_exists(zh, root_path, 0, NULL)) {
           
            fprintf(stderr, "No servers available!\n");
            exit(-1);
        
        }



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
            // Get children

            if (ZOK != zoo_wget_children(zh, root_path, zk_children_handler, watcherCtx, children_list)) {
                fprintf(stderr, "Error getting children of znode %s!\n", root_path);
                return;
            }

            char *tail = zk_get_tail(children_list->data, children_list->count);
            char *head = zk_get_head(children_list->data, children_list->count);

            char *address_tail = zk_get_data(tail);
            char *address_head = zk_get_data(head);

            if (connections == NULL) {
                connections = (struct rtable_pair_t *)malloc(sizeof(struct rtable_pair_t));
                make_connections(address_head, address_tail, connections);
            } else {
                /*
                if (strcmp(address_head, connections->read->address_port) != 0 || strcmp(address_tail, connections->write->address_port) != 0) {
                    connections->read = rtable_connect(address_head);
                    connections->write = rtable_connect(address_tail);
                }
                */
            }

            free(address_head);
            free(address_tail);
            free(tail);
            free(head);

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
    char *node_path = (char *)malloc(strlen(root_path) + strlen("/") + strlen(node_name) + 1);
    strcpy(node_path, root_path);
    strcat(node_path, "/");
    strcat(node_path, node_name);

    char *node_data = malloc(ZDATALEN * sizeof(char));
    int zoo_data_len = ZDATALEN;

    if (ZOK != zoo_get(zh, node_path, 0, node_data, &zoo_data_len, NULL)) {
        fprintf(stderr, "Error getting data from znode %s!\n", node_path);
        return NULL;
    }

    free(node_path);


    return node_data;
}


int make_connections(char *address_head, char *address_tail, struct rtable_pair_t *rtable_pair) {
    rtable_pair->read = rtable_connect(address_head);
    rtable_pair->write = rtable_connect(address_tail);

    if (rtable_pair->read == NULL || rtable_pair->write == NULL) {
        fprintf(stderr, "Error connecting to servers!\n");
        return -1;
    }

    return 0;
}