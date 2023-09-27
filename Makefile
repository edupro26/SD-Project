BIN_DIR = binary
INC_DIR = include
OBJ_DIR = objects
SRC_DIR = source

CC = gcc
CFLAGS = -g -lrt -Wall -Wextra -I $(INC_DIR)

EXECS = $(BIN_DIR)/test_data $(BIN_DIR)/test_entry

all: $(EXECS)

$(BIN_DIR)/test_data: $(OBJ_DIR)/test_data.o $(OBJ_DIR)/data.o
	$(CC) $^ -o $@
$(BIN_DIR)/test_entry: $(OBJ_DIR)/test_entry.o $(OBJ_DIR)/entry.o $(OBJ_DIR)/data.o
	$(CC) $^ -o $@
	

#(BIN_DIR)/test_entry: TODO
#(BIN_DIR)/test_list: TODO
#(BIN_DIR)/test_table: TODO
#(BIN_DIR)/test_serialization: TODO

	@echo Build complete.

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

	

clean:
	rm -rf $(OBJ_DIR)/* $(BIN_DIR)/*

.PHONY: all clean