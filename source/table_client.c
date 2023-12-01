/* 

Sistemas Distribuidos - Projeto Fase 3
Grupo 8
Alexandre Pinto - 55958
Eduardo Proença  - 57551
Tiago Oliveira - 54979

*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>

#include "client_stub-private.h"
#include "entry.h"
#include "stats.h"
#include "zookeeper/zookeeper.h"

static zhandle_t *zh;

struct rtable_pair_t {
    struct rtable* read;
    struct rtable* write;
};

struct String_vector children;

struct rtable_pair_t *zk_init(char *address_port) {
    struct rtable_pair_t *rtable_pair = (struct rtable_pair_t *) malloc(sizeof(struct rtable_pair_t));
    const char* zoo_root = "/"; 
    // Get nodes from zookeeper
    // If one node, read and write point to the same node
    // if two or more nodes, read points to the first node and write points to the last node

    // Connect to zookeeper
    zh = zookeeper_init(address_port, NULL, 10000, 0, 0, 0);

    if (zh == NULL) {
        printf("Error connecting to zookeeper\n");
        return NULL;
    }

    // Get children
    int retval = zoo_get_children(zh, zoo_root, 0, &children);
    
    if (retval != ZOK)	{
		fprintf(stderr, "Error retrieving znode from path %s!\n", zoo_root);
	    exit(EXIT_FAILURE);
	}

    // Print children
    printf("Children: ");
    for (int i = 0; i < children.count; i++) {
        printf("%s ", children.data[i]);
    }
    printf("\n");

    return rtable_pair;
}

int main(int argc, char **argv) {
    signal(SIGPIPE, SIG_IGN);

    if (argc != 2) {
        printf("Usage: ./table_client <server>:<port>\n");
        return -1;
    }

    // Connect to zookeeper
    struct rtable_pair_t *rtable_pair = zk_init(argv[1]);

    struct rtable_t *rtable = rtable_connect(argv[1]);
    if (rtable == NULL) {
        return -1;
    }

    while (1)  {
        char *command = NULL;
        size_t command_size = 0;
        ssize_t command_len;

        // Receive command
        printf("Enter a command: ");
        command_len = getline(&command, &command_size, stdin);
        if (command_len == -1) {
            perror("Error reading command\n");
            free(command);
        }

        // Remove newline character
        command[command_len-1] = '\0';

        // Parse command
        char *command_name = strtok(command, " ");
        char *command_key = strtok(NULL, " ");
        char *command_data = strtok(NULL, " ");
        if (command_name == NULL) {
            perror("Error parsing command\n");
            free(command);
        }
            
        // put <key> <data>
        if (strcmp(command_name, "put") == 0) {
            if (command_key == NULL || command_data == NULL) {
                printf("Invalid arguments. Usage: put <key> <value>\n");
                free(command);
                continue;
            }
            struct data_t *data = data_create(strlen(command_data), command_data);
            struct entry_t *entry = entry_create(command_key, data);
            int put = rtable_put(rtable, entry);
            if (put < 0) {
                printf("Error in rtable_put\n");
                free(command);
                continue;
            }
            free(command);
        } 
        // get <key>
        else if (strcmp(command_name, "get") == 0) {
            if (command_key == NULL) {
                printf("Invalid arguments. Usage: get <key>\n");
                free(command);
                continue;
            }
            struct data_t *data = rtable_get(rtable, command_key);
            if (data == NULL) {
                printf("Error in rtable_get or key not found!\n");
                free(command);
                continue;
            }
            printf("Data: ");
            for (int i = 0; i < data->datasize; i++) {
                char c = ((char *)data->data)[i];
                printf("%c", c);
            }
            printf("\n");
            data_destroy(data);
            free(command);
        }
        // del <key> 
        else if (strcmp(command_name, "del") == 0) {
            if (command_key == NULL) {
                printf("Invalid arguments. Usage: del <key>\n");
                free(command);
                continue;
            }
            int del = rtable_del(rtable, command_key);
            if (del < 0) {
                printf("Error in rtable_del\n");
                free(command);
                continue;
            }
            printf("Entry removed\n");
            free(command);
        } 
        // size
        else if (strcmp(command_name, "size") == 0) {
            int size = rtable_size(rtable);
            if (size < 0) {
                printf("Error in rtable_size\n");
                free(command);
                continue;
            }
            printf("Table size: %d\n", size);
            free(command);
        } 
        // getkeys
        else if (strcmp(command_name, "getkeys") == 0) {
            char **keys = rtable_get_keys(rtable);
            if (keys == NULL) {
                printf("Error in rtable_get_keys\n");
            }
            if (keys[0] == NULL) {
                free(command);
                continue;
            }
            for (int i = 0; keys[i] != NULL; i++) {
                printf("%s\n", keys[i]);
            }
            rtable_free_keys(keys);
            free(command);
        } 
        // gettable
        else if (strcmp(command_name, "gettable") == 0) {
            struct entry_t **entries = rtable_get_table(rtable);
            if (entries == NULL) {
                printf("Error executing rtable_get_table\n");
            }
            if (entries[0] == NULL) {
                free(command);
                continue;
            }
            for (int i = 0; entries[i] != NULL; i++) {
                printf("%s :: ", entries[i]->key);
                for (int j = 0; j < entries[i]->value->datasize; j++) {
                    char c = ((char *)entries[i]->value->data)[j];
                    printf("%c", c);
                }
                printf("\n");
            }
            rtable_free_entries(entries);
            free(command);       
        } 
        // stats
        else if (strcmp(command_name, "stats") == 0) {
            struct statistics_t *stats = rtable_stats(rtable);
            if (stats == NULL) {
                printf("Error executing rtable_stats\n");
                // Free memory
                break;
            }
            printf("Stats\n");
            printf("Ops: %d\n", stats->ops);
            printf("Clients: %d\n", stats->clients);
            printf("Total time: %d\n", stats->time);
    
            // Free stats memory
            free(stats);
            
            free(command);
        }
        // quit
        else if (strcmp(command_name, "quit") == 0) {
            int cls = rtable_disconnect(rtable);
            if(cls < 0){
                perror("Error closing connection to server\n");
                free(command);
                break;
            }
            printf("Closing client...\n");
            free(command);
            break;
        } 
        else {
            printf("Invalid command\n");
            printf("Usage: put <keys> <value> | get <keys> | del <key> | size | getkeys | gettable | quit\n");
            free(command);
            continue;
        }
    }

    return 0;
}

    