CFLAGS=-DNDEBUG -march=native -O3 -Wall -Wextra
CC=gcc

all: default

default: libbitarray.c libbitarray.h
	$(CC) $(CFLAGS) -c libbitarray.c -o bitarray.o

shared: libbitarray.c libbitarray.h
	$(CC) $(CFLAGS) -fPIC -shared -o libbitarray.so libbitarray.c

clean:
	rm -f *.o *.so