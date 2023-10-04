/* 

Sistemas Distribuidos - Projeto Fase 1 
Grupo 8
Alexandre Pinto - 55958
Eduardo Proença  - 57551
Tiago Oliveira - 54979

*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "list-private.h"

struct list_t *list_create() {
    struct list_t *list = (struct list_t *) malloc(sizeof(struct list_t));
    
    if (list == NULL) return NULL; // Return null if malloc fails

    // List starts empty
    list->size = 0;
    list->head = NULL;

    return list;
}

int list_destroy(struct list_t *list) {
    if (list == NULL) return -1; // Return -1 if list is null

    struct node_t *current = list->head;
    struct node_t *temp;

    // Free all nodes
    if(list->size > 0){
        while(current->next != NULL){
            temp = current->next;
            entry_destroy(current->entry);
            free(current);
            current = temp;
            list->size--;
        }
        entry_destroy(current->entry);
        free(current);
    }
    free(list);

    return 0;
}

int list_add(struct list_t *list, struct entry_t *entry) {
    if (list == NULL || entry == NULL) return -1; // Return -1 if list or entry is null

    struct node_t *new_node = (struct node_t *) malloc(sizeof(struct node_t));
    if (new_node == NULL) return -1; // Return -1 if malloc fails

    new_node->entry = entry;
    new_node->next = NULL;

    // If list is empty, add node as head
    if (list->size == 0) {
        list->head = new_node;
        list->size++;
        return 0;
    }

    // If list is not empty, find the right place to insert the node
    struct node_t *current = list->head;
    struct node_t *previous = NULL;

    while (current != NULL) {
        int compare = entry_compare(entry, current->entry);

        // If entry is smaller than current, insert before current
        if (compare < 0) {
            // If previous is NULL, insert at head
            if (previous == NULL) {
                new_node->next = current;
                list->head = new_node;
            } else {
                previous->next = new_node;
                new_node->next = current;
            }
            list->size++;
            return 0;
        }

        // If entry is equal to current, replace current
        if (compare == 0) {
            new_node->next = current->next;
            entry_destroy(current->entry);
            current->entry = entry;
            current->next = new_node->next;
            free(new_node);
            return 1;
        }

        // If entry is bigger than current, continue
        previous = current;
        current = current->next;
    }

    // If entry is bigger than all entries, insert at tail
    previous->next = new_node;
    list->size++;
    return 0;
}

int list_remove(struct list_t *list, char *key) {
    if (list == NULL || key == NULL) return -1; // Return -1 if list or key is null

    struct node_t *current = list->head;
    struct node_t *previous = NULL;

    while (current != NULL) {
        if (strcmp(current->entry->key, key) == 0) {
            // If previous is NULL, remove head
            if (previous == NULL) {
                list->head = current->next;
            } else {
                previous->next = current->next;
            }
            entry_destroy(current->entry);
            free(current);
            list->size--;
            return 0;
        }
        previous = current;
        current = current->next;
    }

    return 1;
}


struct entry_t *list_get(struct list_t *list, char *key) {
    if (list == NULL || key == NULL) return NULL; // Return null if list or key is null

    struct node_t *current = list->head;
    
    // Loop through list until key is found
    while (current != NULL) {
        if (strcmp(current->entry->key, key) == 0) {
            return current->entry;
        }
        current = current->next;
    }

    return NULL;
}

int list_size(struct list_t *list) {
    if (list == NULL) return -1; // Return -1 if list is null

    return list->size;
}

char **list_get_keys(struct list_t *list) {
    if (list == NULL || list->size == 0) return NULL; // Return null if list is null or empty

    char **keys = malloc((list->size + 1) * sizeof(char *));
    if (keys == NULL) return NULL; // Return null if malloc fails

    struct node_t *current = list->head;
    int i = 0;

    // Loop through list and add keys to array
    while (current != NULL) {
        keys[i] = strdup(current->entry->key);
        current = current->next;
        i++;
    }

    keys[i] = NULL;
    return keys;
}

int list_free_keys(char **keys) {
    if (keys == NULL) return -1; // Return -1 if keys is null

    int i = 0;

    // Free all keys
    while (keys[i] != NULL) {
        free(keys[i]);
        i++;
    }

    free(keys);
    return 0;
}

