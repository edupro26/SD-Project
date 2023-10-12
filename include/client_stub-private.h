#ifndef _CLIENT_STUB_PRIVATE_H
#define _CLIENT_STUB_PRIVATE_H

#include "client_stub.h"
#include "network_client.h"

struct rtable_t {
    char *server_address;
    int server_port;
    int sockfd;
};

#endif