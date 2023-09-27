BIN_DIR = binary
INC_DIR = include
OBJ_DIR = objects
SRC_DIR = source

CC = gcc
CFLAGS = -g -lrt -Wall -Wextra -I $(INC_DIR)

EXECS = $(BIN_DIR)/test_data $(BIN_DIR)/test_entry $(BIN_DIR)/test_list #$(BIN_DIR)/test_table $(BIN_DIR)/test_serialization

TEST_DATA_OBJ = $(OBJ_DIR)/test_data.o $(OBJ_DIR)/data.o
TEST_ENTRY_OBJ = $(OBJ_DIR)/test_entry.o $(OBJ_DIR)/entry.o $(OBJ_DIR)/data.o
TEST_LIST_OBJ = $(OBJ_DIR)/test_list.o $(OBJ_DIR)/list.o $(OBJ_DIR)/data.o $(OBJ_DIR)/entry.o
#TEST_TABLE_OBJ = TODO
#TEST_SERIALIZATION_OBJ = TODO

all: $(EXECS)

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