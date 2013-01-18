CC = gcc
CFLAGS = -save-temps=obj -O2 -g
all: main

main: queue.o

queue.o: queue.h common.h
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm *.o *.s *.i
