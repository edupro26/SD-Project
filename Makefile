BIN_DIR = binary
INC_DIR = include
OBJ_DIR = objects
SRC_DIR = source
LIB_DIR = lib

CC = gcc
CFLAGS = -g -lrt -Wall -Wextra -I $(INC_DIR)

EXECS = libtable $(BIN_DIR)/table_server $(BIN_DIR)/table_client

TABLE_CLIENT = $(OBJ_DIR)/table_client.o $(OBJ_DIR)/client_stub.o
TABLE_SERVER = $(OBJ_DIR)/table_server.o $(OBJ_DIR)/network_server.o $(OBJ_DIR)/table_skel.o


OBJ_TO_DEL = $(OBJ_DIR)/network_client.o $(OBJ_DIR)/client_stub.o $(OBJ_DIR)/client_stub-private.o $(OBJ_DIR)/network_server.o $(OBJ_DIR)/table_skel.o $(OBJ_DIR)/table_skel-private.o

all: $(EXECS)

libtable: $(OBJ_DIR)/table.o $(OBJ_DIR)/data.o $(OBJ_DIR)/entry.o $(OBJ_DIR)/list.o
	ar rcs $(LIB_DIR)/libtable.a $^

table-client: $(BIN_DIR)/table_client

table-server: $(BIN_DIR)/table_server

$(BIN_DIR)/table_client: $(TABLE_CLIENT)
	$(CC) $^ -o $@

$(BIN_DIR)/table_server: $(TABLE_SERVER)
	$(CC) $^ -o $@


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -f $(EXECS) $(OBJ_TO_DEL) $(LIB_DIR)/libtable.a

.PHONY: all clean