/*

Sistemas Distribuidos - Projeto Fase 4
Grupo 8
Alexandre Pinto - 55958
Eduardo Proença  - 57551
Tiago Oliveira - 54979

*/

#ifndef ZK_SERVER_H
#define ZK_SERVER_H

#include <stdio.h>
#include <string.h>


#include "table.h"
#include "zookeeper/zookeeper.h"

#define ZDATALEN 1024 * 1024

static zhandle_t *zh;

static int is_connected;

static char *root_path = "/chain";

static char *watcher_ctx = "ZooKeeper Data Watcher";

// Initialize the zookeeper connection
void zk_init(char *ip, short port, struct table_t *table_pointer, char *address_port);

// Watcher function for children events
void zk_children_handler(zhandle_t *zzh, int type, int state, const char *path, void *watcherCtx);

// Watcher function for the connection to zookeeper
void zk_connection_watcher(zhandle_t *zzh, int type, int state, const char *path, void *watcherCtx);

// Given a list of nodes, returns the node with the next id of the given node_name
char *zk_closest_node(char *node_name, char **node_list, int node_list_size);

// Fill the table with the data of the node with the given ip_port
void fill_table(char *ip_port, struct table_t *table_pointer);

// Make put operation in the next node. If there isnt a next node returns 0, if there is and the operation is successful returns 1, otherwise returns -1
int next_node_put(char *key, struct data_t *data);

// Make del operation in the next node. If there isnt a next node returns 0, if there is and the operation is successful returns 1, otherwise returns -1
int next_node_del(char *key);

#endif