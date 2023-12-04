/*

Sistemas Distribuidos - Projeto Fase 4
Grupo 8
Alexandre Pinto - 55958
Eduardo Proença  - 57551
Tiago Oliveira - 54979

*/

#include "table.h"
#include "entry.h"
#include "zk_server.h"
#include "client_stub-private.h"

typedef struct String_vector zoo_string; 
struct String_vector children;

char *this_node = NULL;
char *next_node = NULL;
char *next_node_data = NULL;
// Pointer to rtable
struct table_t *table_ptr2 = NULL;

struct rtable_t *rtable;

void zk_init(char *ip, short port, struct table_t *table_pointer, char *address_port) {
    // Save rtble pointer
    table_ptr2 = table_pointer;

    // Convert port to string
    char port_str[6];
    sprintf(port_str, "%d", port);

    // Deactivate zookeeper logs
    zoo_set_debug_level(ZOO_LOG_LEVEL_ERROR);
    zoo_string* children_list =	NULL;

    /* -------------- !!! CHANGE ZOOKEEPER ADRESS !!! --------------- */

    // Connect to Zookeeper
    zh = zookeeper_init(address_port, zk_connection_watcher, 10000, 0, 0, 0);

    if (zh == NULL) {
        printf("Error connecting to zookeeper\n");
        exit(-1);
    }
    sleep(3);
    if (is_connected) {
    if (ZNONODE == zoo_exists(zh, root_path, 0, NULL)) {
			fprintf(stderr, "%s doesn't exist! \
				Please start ZChildMaker.\n", root_path);
			exit(EXIT_FAILURE);
		}
		char node_path[120] = "";
		strcat(node_path,root_path); 
		strcat(node_path,"/node"); 
		int new_path_len = 1024;
		char* new_path = malloc (new_path_len);
		
            char* data = malloc (ZDATALEN);
            // Put IP and port in data
            strcat(data, ip);
            strcat(data, ":");
            strcat(data, port_str);
			if (ZOK != zoo_create(zh, node_path, data, strlen(data), & ZOO_OPEN_ACL_UNSAFE, ZOO_EPHEMERAL | ZOO_SEQUENCE, new_path, new_path_len)) {
				fprintf(stderr, "Error creating znode from path %s!\n", node_path);
			    exit(EXIT_FAILURE);
			}
			fprintf(stderr, "Created a node with the path: %s\n", new_path); 
            this_node = (char *) malloc(strlen(new_path));
            strcpy(this_node, new_path);
            // Remove path
            this_node = strrchr(this_node, '/n');


		free (new_path);
    }
    next_node_data = malloc(ZDATALEN * sizeof(char));

    children_list =	(zoo_string *) malloc(sizeof(zoo_string));
        if (is_connected) {
            printf("Connection to zookeeper established\n");

           if (ZNONODE == zoo_exists(zh, root_path, 0, NULL)) {
				if (ZOK == zoo_create( zh, root_path, NULL, -1, & ZOO_OPEN_ACL_UNSAFE, 0, NULL, 0)) {
					fprintf(stderr, "%s created!\n", root_path);
				} else {
					fprintf(stderr,"Error Creating %s!\n", root_path);
					exit(EXIT_FAILURE);
				} 
			}

            printf("Getting children\n");

            if (ZOK != zoo_wget_children(zh, root_path, &zk_children_handler, watcher_ctx, children_list)) {
				fprintf(stderr, "Error setting watch at %s!\n", root_path);
			}

            printf("Got children\n");

            // If already exists a node, fill the table with the data from the first node. Get the data from the next node
            if (children_list->count > 1) {
                printf("Already exists a node\n");

                // Get data from first node
                char *first_node_path = (char *) malloc(strlen(root_path) + strlen("/") + strlen(children_list->data[0]) + 1);
                strcpy(first_node_path, root_path);
                strcat(first_node_path, "/");
                strcat(first_node_path, children_list->data[0]);
                int zoo_data_len = ZDATALEN;
                char *first_node_data = malloc(ZDATALEN * sizeof(char));
                if (ZOK != zoo_get(zh, first_node_path, 0, first_node_data, &zoo_data_len, NULL)) {
                    fprintf(stderr, "Error getting data from znode %s!\n", first_node_path);
                }
                free(first_node_path);

                printf("Got data from first node\n");

                // Fill the table with the data from the first node
                fill_table(first_node_data, table_ptr2);

                printf("Filled table with data from first node\n");
            }

        }
    

    printf("Ended proccess of zookeeper initialization\n");

    free(children_list);

}

void zk_children_handler(zhandle_t *zh, int type, int state, const char *path, void *watcherCtx) {
    printf("Zookeeper watch triggered!\n");
    printf("This node path: %s\n", this_node);
    zoo_string* children_list =	(zoo_string *) malloc(sizeof(zoo_string));
	int zoo_data_len = ZDATALEN;
	if (state == ZOO_CONNECTED_STATE)	 {
		if (type == ZOO_CHILD_EVENT) {
	 	   /* Get the updated children and reset the watch */ 
 			if (ZOK != zoo_wget_children(zh, root_path, zk_children_handler, watcher_ctx, children_list)) {
 				fprintf(stderr, "Error setting watch at %s!\n", root_path);
 			}
        
            char* new_next_node = zk_closest_node(this_node, children_list->data, children_list->count);


            if (new_next_node != NULL) {
                printf("Next node: %s\n", new_next_node);

                


                // If next node changed, fill the table with the data from the next node

                if (next_node == NULL || strcmp(next_node, new_next_node) != 0) {
                    next_node = new_next_node;

                    printf("Next node changed\n");

                    // Get data from next node
                    char *next_node_path = (char *) malloc(strlen(root_path) + strlen("/") + strlen(new_next_node) + 1);
                    strcpy(next_node_path, root_path);
                    strcat(next_node_path, "/");
                    strcat(next_node_path, new_next_node);
                
                    if (ZOK != zoo_get(zh, next_node_path, 0, next_node_data, &zoo_data_len, NULL)) {
                        fprintf(stderr, "Error getting data from znode %s!\n", next_node_path);
                    }
                    free(next_node_path);

                    rtable = rtable_connect(next_node_data);
                    

                    printf("Connected to next node\n");
                }
            }

		 } 
	 }
	 free(children_list);
}

void zk_connection_watcher(zhandle_t *zzh, int type, int state, const char *path, void *watcherCtx) {
    if (type == ZOO_SESSION_EVENT) {
		if (state == ZOO_CONNECTED_STATE) {
			is_connected = 1; 
		} else {
			is_connected = 0; 
		}
	}
}


/* Helper function to extract the ID from a node name */
int get_node_id(const char *node_name) {
    int id;
    if (sscanf(node_name, "node%d", &id) == 1) {
        return id;
    } else {
        return -1; // Return -1 if the format is incorrect
    }
}

/* Given a list of nodes, returns the node with the next id of the given node_name */
char *zk_closest_node(char *node_name, char **node_list, int node_list_size) {
    int node_id = get_node_id(node_name);
    if (node_id == -1) {
        return NULL; // Node name format incorrect or not found
    }

    int closest_id = -1;
    char *closest_node = NULL;

    for (int i = 0; i < node_list_size; ++i) {
        int current_id = get_node_id(node_list[i]);

        if (current_id > node_id && (closest_id == -1 || current_id < closest_id)) {
            closest_id = current_id;
            closest_node = node_list[i];
        }
    }

    return closest_node;
}

void fill_table(char *ip_port, struct table_t *table_pointer) {
    // Initiate a connection to the server
    struct rtable_t *rtable = rtable_connect(ip_port);

    if (rtable == NULL) {
        printf("Error connecting to server, to retrieve table\n");
        return;
    }

    // Get the table from the server
    struct entry_t **entries = rtable_get_table(rtable);

    if (entries == NULL) {
        printf("Error retrieving table from server\n");
        return;
    }

    // Fill the table
    for (int i = 0; entries[i] != NULL; ++i) {
        table_put(table_pointer, entries[i]->key, entries[i]->value);
    }

    // Free the entries array
    for (int i = 0; entries[i] != NULL; ++i) {
        entry_destroy(entries[i]);
    }

    free(entries);

    // Close the connection to the server
    rtable_disconnect(rtable);



}




