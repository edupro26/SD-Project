#ifndef ZK_CLIENT_H
#define ZK_CLIENT_H

#include <stdio.h>
#include <string.h>


#include "table.h"
#include "client_stub-private.h"
#include "zookeeper/zookeeper.h"

struct rtable_pair_t {
    char *head_name;
    char *tail_name;
    struct rtable_t* read;
    struct rtable_t* write;
};

#define ZDATALEN 1024 * 1024

// Initialize the zookeeper connection and the connections to the nodes for read and write. Returns a struct with the names of the head and tail nodes and the connections to the nodes for read and write
struct rtable_pair_t *zk_init(char *address_port);

// Watcher function for children events. Calls the function to make the connections to the nodes for read and write when events occur
void zk_children_handler(zhandle_t *zh, int type, int state, const char *path, void *watcherCtx);

// Watcher function for the connection to zookeeper
void zk_connection_watcher(zhandle_t *zh, int type, int state, const char *path, void *watcherCtx);

// Given a list of nodes, returns the node with the lowest id
char *zk_get_head(char **node_list, int size);

// Given a list of nodes, returns the node with the highest id
char *zk_get_tail(char **node_list, int size);

// Returns the data of the node with the given name
char *zk_get_data(char *node_name);

// Make connections to the nodes for read and write. Returns 0 if the connections are successful, -1 otherwise
int make_connections(char *head_name, char *tail_name, struct rtable_pair_t *rtable_pair);



#endif