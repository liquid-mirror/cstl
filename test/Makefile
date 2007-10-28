CC=gcc
# MEMORY=-DHEAP_DEBUG -DMY_MALLOC
CFLAGS=-Wall -g $(MEMORY)

all: test
clean:
	rm -f *.exe *.o

heap.o: heap.c heap.h
	$(CC) $(CFLAGS) -c heap.c

vector: ../cstl/vector.h vector_test.c heap.o
	$(CC) $(CFLAGS) vector_test.c heap.o -o $@.exe
	./$@.exe

ring: ../cstl/ring.h ring_test.c heap.o
	$(CC) $(CFLAGS) ring_test.c heap.o -o $@.exe
	./$@.exe

deque: ../cstl/deque.h ../cstl/vector.h ../cstl/ring.h deque_test.c heap.o
	$(CC) $(CFLAGS) deque_test.c heap.o -o $@.exe
	./$@.exe

list: ../cstl/list.h list_test.c heap.o
	$(CC) $(CFLAGS) list_test.c heap.o -o $@.exe
	./$@.exe

set: ../cstl/set.h ../cstl/rbtree.h set_test.c heap.o rbtree_debug.h
	$(CC) $(CFLAGS) set_test.c heap.o -o $@.exe
	./$@.exe

map: ../cstl/map.h ../cstl/rbtree.h map_test.c heap.o rbtree_debug.h
	$(CC) $(CFLAGS) map_test.c heap.o -o $@.exe
	./$@.exe

string: ../cstl/string.h ../cstl/vector.h string_test.cpp heap.o
	g++ $(CFLAGS) string_test.cpp heap.o -o $@.exe
	./$@.exe

test: vector ring deque list set map string
