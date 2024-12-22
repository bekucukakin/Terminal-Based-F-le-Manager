CC = gcc
CFLAGS = -Wall -g
OBJ = src/main.o src/file_operations.o src/directory_ops.o src/permissions.o src/logger.o
BIN_DIR = bin

all: $(BIN_DIR)/file_manager

$(BIN_DIR)/file_manager: $(OBJ)
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(BIN_DIR) src/*.o logs/*.log
