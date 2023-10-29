/* 

Sistemas Distribuidos - Projeto Fase 2
Grupo 8
Alexandre Pinto - 55958
Eduardo Proença  - 57551
Tiago Oliveira - 54979

*/

#include <stdlib.h>
#include <string.h>

#include "table_skel.h"


struct table_t *table_skel_init(int n_lists) {
    // Create table
    struct table_t *table = table_create(n_lists);
    if (table == NULL) {
        return NULL;
    }

    return table;
}


int table_skel_destroy(struct table_t *table) {
    // Verify if table is NULL
    if (table == NULL) {
        return -1;
    }

    // Destroy table and all memory allocated
    return table_destroy(table);
}

int invoke(MessageT *msg, struct table_t *table) {
    if (msg == NULL || table == NULL) {
        return -1;
    }

    switch (msg->opcode) {
        case MESSAGE_T__OPCODE__OP_PUT:
            if (msg->c_type == MESSAGE_T__C_TYPE__CT_ENTRY) {
                struct data_t *data = data_create(msg->value.len, msg->value.data);
                int result = table_put(table, msg->key, data);
                if (result == 0) {
                    msg->opcode = MESSAGE_T__OPCODE__OP_PUT + 1;
                    msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;
                    msg->result = 0;
                } else {
                    msg->opcode = MESSAGE_T__OPCODE__OP_ERROR;
                    msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;
                }
                free(data);
            }
            break;

        case MESSAGE_T__OPCODE__OP_GET:
            if (msg->c_type == MESSAGE_T__C_TYPE__CT_KEY) {
                struct data_t *result_data = table_get(table, msg->key);

                if (result_data) {
                    msg->opcode = MESSAGE_T__OPCODE__OP_GET+1;
                    msg->c_type = MESSAGE_T__C_TYPE__CT_VALUE;
                    msg->value.data = malloc(result_data->datasize);
                    if (!msg->value.data) {
                        msg->opcode = MESSAGE_T__OPCODE__OP_ERROR;
                        msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;
                        break;
                    } else {
                        memcpy(msg->value.data, result_data->data, result_data->datasize);
                        msg->value.len = result_data->datasize;
                    }
                    data_destroy(result_data);
                } else {
                    msg->opcode = MESSAGE_T__OPCODE__OP_ERROR;
                    msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;
                }
            }
            break;

        case MESSAGE_T__OPCODE__OP_DEL:
            if (msg->c_type == MESSAGE_T__C_TYPE__CT_KEY) {
                int result = table_remove(table, msg->key);
                if (result == 0) {
                    msg->opcode = MESSAGE_T__OPCODE__OP_DEL+1;
                    msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;
                    msg->result = 0;
                } else if (result == 1) {
                    msg->opcode = MESSAGE_T__OPCODE__OP_ERROR;
                    msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;
                    msg->result = 1;
                } else {
                    msg->opcode = MESSAGE_T__OPCODE__OP_ERROR;
                    msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;
                }
            }
            break;

        case MESSAGE_T__OPCODE__OP_SIZE:
            if (msg->c_type == MESSAGE_T__C_TYPE__CT_NONE) {
                int size = table_size(table);
                if (size >= 0) {
                    msg->opcode = MESSAGE_T__OPCODE__OP_SIZE+1;
                    msg->c_type = MESSAGE_T__C_TYPE__CT_RESULT;
                    msg->result = size;
                } else {
                    msg->opcode = MESSAGE_T__OPCODE__OP_ERROR;
                    msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;
                }
            }
            break;

        case MESSAGE_T__OPCODE__OP_GETKEYS:
            if (msg->c_type == MESSAGE_T__C_TYPE__CT_NONE) {
                char **keys = table_get_keys(table);
                if (keys) {
                    int count;
                    for (count = 0; keys[count] != NULL; count++);
                    msg->opcode = MESSAGE_T__OPCODE__OP_GETKEYS+1;
                    msg->c_type = MESSAGE_T__C_TYPE__CT_KEYS;
                    msg->keys = keys;
                    msg->n_keys = count;
                } else {
                    msg->opcode = MESSAGE_T__OPCODE__OP_ERROR;
                    msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;
                }
            }
            break;
        
        case MESSAGE_T__OPCODE__OP_GETTABLE:
            if (msg->c_type == MESSAGE_T__C_TYPE__CT_NONE) {
                // Get all keys
                char **keys = table_get_keys(table);
                int num_keys = table_size(table);

                // Create array of entries and allocate memory
                msg->entries = malloc(num_keys * sizeof(EntryT *));
                if (!msg->entries) {
                    table_free_keys(keys);
                    msg->opcode = MESSAGE_T__OPCODE__OP_ERROR;
                    return -1;
                }

                for (int i = 0; i < num_keys; i++) {
                    struct data_t *data = table_get(table, keys[i]);

                    // Create entry
                    EntryT *entry = malloc(sizeof(EntryT));
                    if (!entry) {
                        // Free all memory allocated if error
                        for (int j = 0; j < i; j++) {
                            free(msg->entries[j]);
                        }
                        free(msg->entries);
                        table_free_keys(keys);
                        msg->opcode = MESSAGE_T__OPCODE__OP_ERROR;
                        return -1;
                    }
                    entry_t__init(entry);
                    entry->key = strdup(keys[i]); 
                    entry->value.data = malloc(data->datasize);
                    if (!entry->value.data) {
                        msg->opcode = MESSAGE_T__OPCODE__OP_ERROR;
                        msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;
                        break;
                    } else {
                        memcpy(entry->value.data, data->data, data->datasize);
                        entry->value.len = data->datasize;  
                        msg->entries[i] = entry;
                    }
                    data_destroy(data);
                }
                
                table_free_keys(keys);
                msg->n_entries = num_keys;
                msg->opcode = MESSAGE_T__OPCODE__OP_GETTABLE+1;
                msg->c_type = MESSAGE_T__C_TYPE__CT_TABLE;
                return 0;
            }
            break;

        default:
            msg->opcode = MESSAGE_T__OPCODE__OP_ERROR;
            msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;
            return -1;  // Unknown operation
    }

    return 0;
}