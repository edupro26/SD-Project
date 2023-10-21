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

int main(int argc, char **argv) {
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

    // 1. Parse arguments
    if (argc != 2) {
        printf("Usage: ./table_client <server>:<port>\n");
        return -1;
    }
    
    char *server_address = strtok(argv[1], ":");
    char *server_port = strtok(NULL, ":");

    if (server_address == NULL || server_port == NULL) {
        printf("Usage: ./table_client <server>:<port>\n");
        return -1;
    }

    printf("Connecting to %s:%s...\n", server_address, server_port);

    while (1)
    {
        // Receive command
        char *command = NULL;
        size_t command_size = 0;
        ssize_t command_len = getline(&command, &command_size, stdin);
        if (command_len == -1) {
            printf("Error reading command\n");
            continue;
        }

        // Remove newline character
        command[command_len-1] = '\0';

        // Parse command
        char *command_name = strtok(command, " ");
        char *command_key = strtok(NULL, " ");
        char *command_data = strtok(NULL, " ");

        if (command_name == NULL) {
            printf("Error parsing command\n");
            continue;
        }

        // Execute command
        if (strcmp(command_name, "put") == 0) {
            if (command_key == NULL || command_data == NULL) {
                printf("Usage: put <key> <data>\n");
                continue;
            }
            printf("Executing command: put %s %s\n", command_key, command_data);
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
        } else if (strcmp(command_name, "getkeys") == 0) {
            printf("Executing command: getkeys\n");
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
    


    printf("Closing client...\n");
  
    

    return 0;
}

    