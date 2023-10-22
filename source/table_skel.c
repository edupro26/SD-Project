/* 

Sistemas Distribuidos - Projeto Fase 2
Grupo 8
Alexandre Pinto - 55958
Eduardo Proença  - 57551
Tiago Oliveira - 54979

*/


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
                struct data_t *data = data_create(msg->entry->value.len, msg->entry->value.data);
                int result = table_put(table, msg->entry->key, data);
                data_destroy(data);
                if (result == 0) {
                    msg->opcode = MESSAGE_T__OPCODE__OP_PUT;
                    msg->c_type = MESSAGE_T__C_TYPE__CT_RESULT;
                    msg->result = 0;
                } else {
                    msg->opcode = MESSAGE_T__OPCODE__OP_ERROR;
                    msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;
                }
            }
            break;

        case MESSAGE_T__OPCODE__OP_GET:
            if (msg->c_type == MESSAGE_T__C_TYPE__CT_KEY) {
                struct data_t *result_data = table_get(table, msg->key);
                if (result_data) {
                    msg->opcode = MESSAGE_T__OPCODE__OP_GET;
                    msg->c_type = MESSAGE_T__C_TYPE__CT_VALUE;
                    msg->value.data = result_data->data;
                    msg->value.len = result_data->datasize;
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
                    msg->opcode = MESSAGE_T__OPCODE__OP_DEL;
                    msg->c_type = MESSAGE_T__C_TYPE__CT_RESULT;
                    msg->result = 0;
                } else if (result == 1) {
                    // Entry not found
                    msg->opcode = MESSAGE_T__OPCODE__OP_DEL;
                    msg->c_type = MESSAGE_T__C_TYPE__CT_RESULT;
                    msg->result = 1;
                } else {
                    msg->opcode = MESSAGE_T__OPCODE__OP_ERROR;
                    msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;
                }
            }
            break;

        case MESSAGE_T__OPCODE__OP_SIZE:
            {
            int size = table_size(table);
            if (size >= 0) {
                msg->opcode = MESSAGE_T__OPCODE__OP_SIZE;
                msg->c_type = MESSAGE_T__C_TYPE__CT_RESULT;
                msg->result = size;
            } else {
                msg->opcode = MESSAGE_T__OPCODE__OP_ERROR;
                msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;
            }
            }
            break;

        case MESSAGE_T__OPCODE__OP_GETKEYS:
            {
            char **keys = table_get_keys(table);
            if (keys) {
                // Count number of keys
                int count;
                for (count = 0; keys[count] != NULL; count++);
                
                msg->opcode = MESSAGE_T__OPCODE__OP_GETKEYS;
                msg->c_type = MESSAGE_T__C_TYPE__CT_KEYS;
                msg->keys = keys;
                msg->n_keys = count;
            } else {
                msg->opcode = MESSAGE_T__OPCODE__OP_ERROR;
                msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;
            }
            }
            break;


        default:
            return -1;  // Unknown operation
    }

    return 0;
}