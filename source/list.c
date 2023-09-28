#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "list-private.h"

struct list_t *list_create() {
    struct list_t *new_list = malloc(sizeof(struct list_t));

    if (new_list == NULL) {
        return NULL;
    }
    // List starts empty
    new_list->size = 0;
    new_list->head = NULL;

    return new_list;
}

int list_destroy(struct list_t *list) {
    if (list == NULL) {
        return -1;
    }

    struct node_t *current = list->head;
    struct node_t *next;

    while (current != NULL) {
        next = current->next;
        //free(current);
        current = next;
    }

    free(list);
    return 0;
}

int list_add(struct list_t *list, struct entry_t *entry) {
    if (list == NULL || entry == NULL) {
        return -1;
    }

    struct node_t *new_node = malloc(sizeof(struct node_t));
    if (new_node == NULL) {
        return -1;
    }

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
            //free(current);
            //current = new_node;
            current->entry = entry;
            current->next = new_node->next;
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
    if (list == NULL || key == NULL) {
        return -1;
    }

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
    if (list == NULL || key == NULL) {
        return NULL;
    }

    struct node_t *current = list->head;
    
    while (current != NULL) {
        if (strcmp(current->entry->key, key) == 0) {
            return current->entry;
        }
        current = current->next;
    }

    return NULL;
}

int list_size(struct list_t *list) {
    if (list == NULL) {
        return -1;
    }

    return list->size;
}

char **list_get_keys(struct list_t *list) {
    if (list == NULL || list->size == 0) {
        return NULL;
    }

    char **keys = malloc((list->size + 1) * sizeof(char *));
    if (keys == NULL) {
        return NULL;
    }

    struct node_t *current = list->head;
    int i = 0;

    while (current != NULL) {
        keys[i] = strdup(current->entry->key);
        current = current->next;
        i++;
    }

    keys[i] = NULL;
    return keys;
}

int list_free_keys(char **keys) {
    if (keys == NULL) {
        return -1;
    }

    int i = 0;
    while (keys[i] != NULL) {
        free(keys[i]);
        i++;
    }

    free(keys);
    return 0;
}

