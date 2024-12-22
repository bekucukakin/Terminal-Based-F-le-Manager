# Compiler ve bayraklar
CC = gcc
CFLAGS = -Wall -g
INCLUDE_DIR = include
OBJ_DIR = obj
BIN_DIR = bin

# Kaynak dosyaları
SRC = src/directory_ops.c src/file_operations.c src/logger.c src/main.c src/permissions.c

# Nesne dosyaları
OBJ = $(SRC:src/%.c=$(OBJ_DIR)/%.o)

# Çıktı dosyası
TARGET = $(BIN_DIR)/file_manager

# Başlık dosyaları için include yolu
INC = -I$(INCLUDE_DIR)

# Varsayılan hedef (programı oluştur)
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

# Nesne dosyalarını derle
$(OBJ_DIR)/%.o: src/%.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

# Temizleme işlemi
clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET)