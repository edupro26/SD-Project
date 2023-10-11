BIN_DIR = binary
INC_DIR = include
OBJ_DIR = objects
SRC_DIR = source
LIB_DIR = lib

CC = gcc
CFLAGS = -g -lrt -Wall -Wextra -I $(INC_DIR)

EXECS = $(BIN_DIR)/table_server $(BIN_DIR)/table_client

TABLE_CLIENT = $(OBJ_DIR)/network_client.h $(OBJ_DIR)/client_stub.h $(OBJ_DIR)/client_stub-private.h $(LIB_DIR)/libtable.a

all: $(EXECS)

libtable: $(OBJ_DIR)/table.o $(OBJ_DIR)/data.o $(OBJ_DIR)/entry.o $(OBJ_DIR)/list.o
	ar rcs $(LIB_DIR)/libtable.a $^

table_client: 

table_server:



$(BIN_DIR)/test_data: $(TEST_DATA_OBJ)
	$(CC) $^ -o $@

$(BIN_DIR)/test_entry: $(TEST_ENTRY_OBJ)
	$(CC) $^ -o $@

$(BIN_DIR)/test_list: $(TEST_LIST_OBJ)
	$(CC) $^ -o $@

$(BIN_DIR)/test_table: $(TEST_TABLE_OBJ)
	$(CC) $^ -o $@

$(BIN_DIR)/test_serialization: $(TEST_SERIALIZATION_OBJ)
	$(CC) $^ -o $@

	@echo Build complete.

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -rf $(OBJ_DIR)/* $(BIN_DIR)/*

.PHONY: all clean