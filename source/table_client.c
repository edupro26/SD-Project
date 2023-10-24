/* 

Sistemas Distribuidos - Projeto Fase 2
Grupo 8
Alexandre Pinto - 55958
Eduardo Proença  - 57551
Tiago Oliveira - 54979

*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "client_stub-private.h"
#include "entry.h"

/*
    Usage:
    ./table_client <ip_server>:<port_server>

    Commands:
    put <key> <data>
    get <key>
    del <key>
    size
    getkeys
    gettable
    quit

*/
int main(int argc, char **argv) {

    if (argc != 2) {
        printf("Usage: ./table_client <server>:<port>\n");
        return -1;
    }

    struct rtable_t *rtable = rtable_connect(argv[1]);
    if (rtable == NULL) {
        printf("Error connecting to server\n");
        printf("Usage: ./table_client <server>:<port>\n");
        return -1;
    }

    while (1)  {
        // Receive command
        printf("Enter a command:\n");
        char *command = NULL;
        size_t command_size = 0;
        ssize_t command_len = getline(&command, &command_size, stdin);
        if (command_len == -1)
            printf("Error reading command\n");

        // Remove newline character
        command[command_len-1] = '\0';
        // Parse command
        char *command_name = strtok(command, " ");
        char *command_key = strtok(NULL, " ");
        char *command_data = strtok(NULL, " ");

        if (command_name == NULL)
            printf("Error parsing command\n");
            
        // Execute command
        if (strcmp(command_name, "put") == 0) {
            if (command_key == NULL || command_data == NULL)
                printf("Usage: put <key> <data>\n");

            printf("Executing command: put %s %s\n", command_key, command_data);
            struct entry_t *entry = entry_create(command_key, command_data);
            int put = rtable_put(rtable, entry);
            if (put < 0)
                printf("Error executing command\n");

            entry_destroy(entry);
            printf("Object put");

        } else if (strcmp(command_name, "get") == 0) {
            if (command_key == NULL) {
                printf("Usage: get <key>\n");
                continue;
            }
            printf("Executing command: get %s\n", command_key);
        } else if (strcmp(command_name, "del") == 0) {
            if (command_key == NULL) {
                printf("Usage: del <key>\n");
                continue;
            }
            printf("Executing command: del %s\n", command_key);
        } else if (strcmp(command_name, "size") == 0) {
            printf("Executing command: size\n");
            int size = rtable_size(rtable);
            if (size < 0) {
                printf("Error executing command\n");
                continue;
            }
            printf("Table size: %d\n", size);
        } else if (strcmp(command_name, "getkeys") == 0) {
            printf("Executing command: getkeys\n");
            char **keys = rtable_get_keys(rtable);
            if (keys == NULL) {
                printf("Error executing command\n");
                continue;
            }
            printf("Keys:\n");
            for (int i = 0; keys[i] != NULL; i++) {
                printf("%s\n", keys[i]);
            }
            rtable_free_keys(keys);
        } else if (strcmp(command_name, "gettable") == 0) {
            printf("Executing command: gettable\n");
        } else if (strcmp(command_name, "quit") == 0) {
            printf("Executing command: quit\n");
            break;
        } else {
            printf("Unknown command\n");
            continue;
        }
    }
    

    int cls = rtable_disconnect(rtable);

    printf("Closing client...\n");
  
    

    return 0;
}

    