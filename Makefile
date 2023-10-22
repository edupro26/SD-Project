BIN_DIR = binary
INC_DIR = include
OBJ_DIR = objects
SRC_DIR = source
LIB_DIR = lib
MAC_INC_DIR = -I /opt/homebrew/Cellar/protobuf-c/1.4.1_8/include
MAC_LINK_FLAGS = -L/opt/homebrew/Cellar/protobuf-c/1.4.1_8/lib -lprotobuf-c

CC = gcc
CFLAGS = -g -lrt -Wall -Wextra -I $(INC_DIR) $(MAC_INC_DIR)
LDFLAGS = $(MAC_LINK_FLAGS)

EXECS = libtable $(BIN_DIR)/table_server $(BIN_DIR)/table_client

LIB_MAC_OBJ = $(OBJ_DIR)/table-mac.o $(OBJ_DIR)/data-mac.o $(OBJ_DIR)/entry-mac.o $(OBJ_DIR)/list-mac.o
TABLE_CLIENT = $(OBJ_DIR)/sdmessage.pb-c.o $(OBJ_DIR)/table_client.o $(OBJ_DIR)/client_stub.o $(OBJ_DIR)/network_client.o
TABLE_SERVER = $(OBJ_DIR)/sdmessage.pb-c.o $(OBJ_DIR)/table_server.o $(OBJ_DIR)/network_server.o $(OBJ_DIR)/table_skel.o

OBJ_TO_DEL = $(OBJ_DIR)/network_client.o $(OBJ_DIR)/client_stub.o $(OBJ_DIR)/client_stub-private.o $(OBJ_DIR)/network_server.o $(OBJ_DIR)/table_skel.o $(OBJ_DIR)/table_skel-private.o $(OBJ_DIR)/table_client.o $(OBJ_DIR)/table_server.o $(OBJ_DIR)/sdmessage.pb-c.o

all: $(EXECS)

libtable: 
	ar rcs $(LIB_DIR)/libtable.a $(OBJ_DIR)/table.o $(OBJ_DIR)/data.o $(OBJ_DIR)/entry.o $(OBJ_DIR)/list.o

table-client: $(BIN_DIR)/table_client

table-server: $(BIN_DIR)/table_server

# MAC OS COMPILATION

libtable-mac:
	ar rcs $(LIB_DIR)/libtable.a $(LIB_MAC_OBJ)

mac-table-client: $(TABLE_CLIENT)
	$(CC) $^ $(LDFLAGS) -L$(LIB_DIR) -ltable -o $(BIN_DIR)/table_client

mac-table-server: $(TABLE_SERVER)
	$(CC) $^ $(LDFLAGS) -L$(LIB_DIR) -ltable -o $(BIN_DIR)/table_server

mac: libtable-mac mac-table-client mac-table-server

$(BIN_DIR)/table_client: $(TABLE_CLIENT)
	$(CC) $^ -lprotobuf-c -L$(LIB_DIR) -ltable -o $@

$(BIN_DIR)/table_server: $(TABLE_SERVER)
	$(CC) $^ -lprotobuf-c -L$(LIB_DIR) -ltable -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -f $(EXECS) $(OBJ_TO_DEL) $(LIB_DIR)/libtable.a

.PHONY: all clean