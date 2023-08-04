CC = gcc
CFLAGS = -Wall -MMD -I. -g
RM = rm -rf *.o *.d *.a *.txt 

SERVER = server
CLIENT = client

SRCS = $(wildcard *.c)
OBJS = $(patsubst %.c, %.o, $(SRCS))


all: $(SERVER) $(CLIENT)

#MAIN
$(SERVER): $(OBJS) 
	$(CC) $(CFLAGS) -o $(SERVER) $(SERVER).o

$(CLIENT): $(OBJS) 
	$(CC) $(CFLAGS) -o $(CLIENT) $(CLIENT).o 

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
	$(RM) $(SERVER) $(CLIENT)

