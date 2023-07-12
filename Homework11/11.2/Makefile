CC := gcc
CFLAGS := -Wall 
LDFLAGS := 

#Директории
SRC_DIR :=
OBJ_DIR :=
LIB_DIR := 
BIN_DIR :=

# Список динамических библеотек
LIBS := 

# Список исходных файлов
SOURCES := $(wildcard $(SRC_DIR)*.c)

# Формирование списка объектных файлов из исходных файлов
OBJ := $(patsubst $(SRC_DIR)%.c, $(OBJ_DIR)%.o, $(SOURCES))

# Имя исполняемого файла 
TARGET := main


# Цель общей сборки
all: $(TARGET) $(patsubst %,$(LIB_DIR)lib%.so,$(LIBS))

# Правило сборки исполняемого файла
$(TARGET): $(OBJ)
	$(CC) -o $(BIN_DIR)$(TARGET) $(OBJ_DIR)$(TARGET).o  

# Правило сборки объектных файлов
$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Правило сборки динамических библиотек
$(LIB_DIR)lib%.so: $(OBJ_DIR)%.o
	$(CC) $(LDFLAGS) $< -o $@

# Удаление созданных файлов
clean:
	rm -rf $(OBJ_DIR)*.o $(BIN_DIR)$(TARGET) $(LIB_DIR)*.so