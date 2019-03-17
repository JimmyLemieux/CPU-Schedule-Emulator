CC = gcc
CFLAGS = -Wall -g -std=c11

all: idispatch

idispatch:
	$(CC) $(CFLAGS) -o idispatcher.o idispatcher.c


clean:
	rm -rf *.o