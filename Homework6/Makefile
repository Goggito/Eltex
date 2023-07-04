CC = gcc
CFLAGS = -Wall -Wextra -g

TARGET = main

all: $(TARGET)

$(TARGET): main.c liboperations.a
	$(CC) $(CFLAGS) -o $(TARGET) main.c -L. -loperations

liboperations.a: add.o sub.o mul.o div.o
	ar cr liboperations.a add.o sub.o mul.o div.o

add.o: add.c
	$(CC) $(CFLAGS) add.c -c

sub.o: sub.c
	$(CC) $(CFLAGS) sub.c -c

mul.o: mul.c
	$(CC) $(CFLAGS) mul.c -c

div.o: div.c
	$(CC) $(CFLAGS) div.c -c

debug: $(TARGET)
	gdb $(TARGET)

clean:
	rm -f $(TARGET)