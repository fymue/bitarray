CFLAGS=-march=native -O3 -Wall -Wextra
CC=gcc
DEBUGFLAGS=-g -Wall -Wextra

default: libbitarray.c libbitarray.h
	$(CC) $(CFLAGS) -c libbitarray.c -o bitarray.o

shared: libbitarray.c libbitarray.h
	$(CC) $(CFLAGS) -fPIC -shared -o libbitarray.so libbitarray.c

debug: libbitarray.c libbitarray.h
	$(CC) $(DEBUGFLAGS) -c libbitarray.c -o bitarray.o

debug_shared: libbitarray.c libbitarray.h
	$(CC) $(DEBUGFLAGS) -fPIC -shared -o libbitarray.so libbitarray.c

test: bitarray_test.c
	$(CC) $(DEBUGFLAGS) -o test bitarray_test.c

clean:
	rm -f *.o *.so test