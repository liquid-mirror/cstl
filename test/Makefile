CC=gcc
# MEMORY=-DHEAP_DEBUG -DMY_MALLOC
CFLAGS=-Wall -g $(MEMORY)

all: test
clean:
	rm -f *.exe *.o

heap.o: heap.c heap.h
	$(CC) $(CFLAGS) -c heap.c

vector_test.o: ../cstl/vector.h vector_test.c heap.o
	$(CC) $(CFLAGS) -c vector_test.c

deque_test.o: ../cstl/deque.h deque_test.c heap.o
	$(CC) $(CFLAGS) -c deque_test.c

list_test.o: ../cstl/list.h list_test.c heap.o
	$(CC) $(CFLAGS) -c list_test.c

set_test.o: ../cstl/set.h ../cstl/rbtree.h set_test.c heap.o rbtree_debug.h
	$(CC) $(CFLAGS) -c set_test.c

map_test.o: ../cstl/map.h ../cstl/rbtree.h map_test.c heap.o rbtree_debug.h
	$(CC) $(CFLAGS) -c map_test.c

string_test.o: ../cstl/string.h ../cstl/vector.h string_test.cpp heap.o
	g++ $(CFLAGS) -c string_test.cpp

test: test.h main.c vector_test.o deque_test.o list_test.o set_test.o map_test.o string_test.o
	$(CC) $(CFLAGS) -o $@.exe main.c heap.o vector_test.o deque_test.o list_test.o set_test.o map_test.o
	./$@.exe
	g++ -o string_test.exe heap.o string_test.o
	./string_test.exe

