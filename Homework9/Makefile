CC := gcc
CFLAGS := -Wall -fPIC
LDFLAGS := 
SRC_DIR := src
OBJ_DIR := obj
LIB_DIR := lib

# Список динамических библеотек
LIBS := 

# Список исходных файлов
SOURCES := $(wildcard $(SRC_DIR)/*.c)

# Формирование списка объектных файлов из исходных файлов
OBJ := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))

# Имя исполняемого файла 
TARGET := main

all: $(TARGET) $(patsubst %,$(LIB_DIR)/lib%.so,$(LIBS))

# Правило сборки исполняемого файла
$(TARGET): $(OBJ)
	$(CC) $(OBJ_DIR)/$(TARGET).o -o bin/$(TARGET) -lncurses

# Правило сборки объектных файлов
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Правило сборки динамических библиотек
$(LIB_DIR)/lib%.so: $(OBJ_DIR)/%.o
	$(CC) $(LDFLAGS) $< -o $@

# Удаления созданных файлов
clean:
	rm -rf $(OBJ_DIR)/*.o bin/$(TARGET) $(LIB_DIR)/*.so