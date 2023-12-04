#ifndef ZK_CLIENT_H
#define ZK_CLIENT_H

#include <stdio.h>
#include <string.h>


#include "table.h"
#include "client_stub-private.h"
#include "zookeeper/zookeeper.h"

struct rtable_pair_t {
    struct rtable* read;
    struct rtable* write;
};

#define ZDATALEN 1024 * 1024

static zhandle_t *zh;

static int is_connected;

static char *root_path = "/chain";

static char *watcher_ctx = "ZooKeeper Data Watcher";

struct rtable_pair_t *zk_init(char *address_port);

void zk_children_handler(zhandle_t *zh, int type, int state, const char *path, void *watcherCtx);

void zk_connection_watcher(zhandle_t *zh, int type, int state, const char *path, void *watcherCtx);

char *zk_get_head(char **node_list, int size);

char *zk_get_tail(char **node_list, int size);

char **zk_get_data(char *node_name);

int make_connections(char *address_head, char *address_tail, struct rtable_pair_t *rtable_pair);



#endif