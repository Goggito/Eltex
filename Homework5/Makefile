CC = gcc
CFLAGS = -Wall -Wextra -g

TARGET = main

all: $(TARGET)

$(TARGET): main.c
	$(CC) $(CFLAGS) -o $(TARGET) main.c

debug: $(TARGET)
	gdb $(TARGET)

clean:
	rm -f $(TARGET)