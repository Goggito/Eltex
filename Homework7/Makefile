CC = gcc
CFLAGS = -Wall -Wextra -g

SRC = add.c sub.c mul.c div.c
OBJ = add.o sub.o mul.o div.o

TARGET = main

all: $(TARGET)

$(TARGET): main.c liboperations.so
	$(CC) $(CFLAGS) -o $(TARGET) main.c -L. -loperations

liboperations.so: $(OBJ)
	$(CC) $(CFLAGS) -shared -o liboperations.so $(OBJ)

%.o : %.c
	$(CC) $(CFLAGS) $< -fPIC -c

debug: $(TARGET)
	gdb $(TARGET)

clean:
	rm -f $(TARGET) $(OBJ) 