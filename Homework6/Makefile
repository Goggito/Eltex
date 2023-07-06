CC = gcc
CFLAGS = -Wall -Wextra -g

SRC = add.c sub.c mul.c div.c
OBJ = add.o sub.o mul.o div.o

TARGET = main

all: $(TARGET)

$(TARGET): main.c liboperations.a
	$(CC) $(CFLAGS) -o $(TARGET) main.c -L. -loperations

liboperations.a: $(OBJ)
	ar cr liboperations.a $(OBJ)

%.o : %.c
	$(CC) $(CFLAGS) -c

debug: $(TARGET)
	gdb $(TARGET)

clean:
	rm -f $(TARGET) %.o %.a