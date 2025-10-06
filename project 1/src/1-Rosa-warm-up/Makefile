CC = gcc
CFLAGS = -O3 -Wall

all: hello_worldc

hello_worldc: hello_world.c
	$(CC) $(CFLAGS) hello_world.c -o hello_worldc

clean:
	rm -f hello_worldc

.PHONY: all clean
