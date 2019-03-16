CC = gcc
CFLAGS = -Wall -g

all: idispatch

idispatch:
	$(CC) $(CFLAGS) -o idispatcher.o idispatcher.c


clean:
	rm -rf *.o