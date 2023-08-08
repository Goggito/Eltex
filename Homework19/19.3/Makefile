CC := gcc
CFLAGS := -Wall -MMD -I. -lrt -g
RM := rm -rf *.o *.d *.a *.txt 

SRC_DIR := src
OBJ_DIR := obj
LIB_DIR := lib
BIN_DIR := bin

SERVER := server
CLIENT := client
TEST := stress_test

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))


all: $(SERVER) $(CLIENT) $(TEST)

#MAIN
$(SERVER): $(OBJS) 
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$(SERVER) $(OBJ_DIR)/$(SERVER).o 

$(CLIENT): $(OBJS) 
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$(CLIENT) $(OBJ_DIR)/$(CLIENT).o 

$(TEST): $(OBJS) 
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$(TEST) $(OBJ_DIR)/$(TEST).o 

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ 

clean:
	rm -rf $(OBJ_DIR)/*.o bin/* $(LIB_DIR)/*.so
	$(RM) $(SERVER) $(CLIENT) $(TEST)

