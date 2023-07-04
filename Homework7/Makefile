CC = gcc
CFLAGS = -Wall -Wextra -g

TARGET = main

all: $(TARGET)

$(TARGET): main.c liboperations.so
	$(CC) $(CFLAGS) -o $(TARGET) main.c -L. -loperations

liboperations.so: add.o sub.o mul.o div.o
	$(CC) $(CFLAGS) -shared -o liboperations.so add.o sub.o mul.o div.o 

add.o: add.c
	$(CC) $(CFLAGS) add.c -fPIC -c

sub.o: sub.c
	$(CC) $(CFLAGS) sub.c -fPIC -c

mul.o: mul.c
	$(CC) $(CFLAGS) mul.c -fPIC -c

div.o: div.c
	$(CC) $(CFLAGS) div.c -fPIC -c

debug: $(TARGET)
	gdb $(TARGET)

clean:
	rm -f $(TARGET) 