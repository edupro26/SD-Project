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
        printf("Enter a command: ");
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
            if (command_key == NULL || command_data == NULL) {
                printf("Usage: put <key> <data>\n");
                continue;
            }
            struct data_t *data = data_create(strlen(command_data), command_data);
            struct entry_t *entry = entry_create(command_key, data);
            int put = rtable_put(rtable, entry);
            if (put < 0) {
                printf("Error executing command\n");
                continue;
            }
        } 
        else if (strcmp(command_name, "get") == 0) {
            if (command_key == NULL) {
                printf("Usage: get <key>\n");
                continue;
            }
            struct data_t *data = rtable_get(rtable, command_key);
            if (data == NULL) {
                printf("Error executing command\n");
                continue;
            }
            printf("Data: %s\n", (char *) data->data);
        } 
        else if (strcmp(command_name, "del") == 0) {
            if (command_key == NULL) {
                printf("Usage: del <key>\n");
                continue;
            }
            int del = rtable_del(rtable, command_key);
            if (del < 0) {
                printf("Error executing command\n");
                continue;
            }
            printf("Entry removed\n");
        } 
        else if (strcmp(command_name, "size") == 0) {
            int size = rtable_size(rtable);
            if (size < 0) {
                printf("Error executing command\n");
                continue;
            }
            printf("Table size: %d\n", size);
        } 
        else if (strcmp(command_name, "getkeys") == 0) {
            char **keys = rtable_get_keys(rtable);
            if (keys == NULL) {
                printf("Error executing command\n");
                continue;
            }
            for (int i = 0; keys[i] != NULL; i++) {
                printf("%s\n", keys[i]);
            }
            rtable_free_keys(keys);
        } 
        else if (strcmp(command_name, "gettable") == 0) {
            struct entry_t **entries = rtable_get_table(rtable);
            if (entries == NULL) {
                printf("Error executing command\n");
                continue;
            }
            for (int i = 0; entries[i] != NULL; i++) {
                printf("%s :: %s\n", entries[i]->key, (char *) entries[i]->value->data);
            }
            rtable_free_entries(entries);       
        } 
        else if (strcmp(command_name, "quit") == 0) {
            int cls = rtable_disconnect(rtable);
            if(cls < 0){
                perror("Error closing connection to server");
                break;
            }
            printf("Closing client...\n");
            break;
        } 
        else {
            printf("Invalid command\n");
            printf("Usage: put <keys> <value> | get <keys> | del <key> | size | getkeys | gettable | quit\n");
            continue;
        }
    }

    return 0;
}

    