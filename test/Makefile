CC=gcc
# MEMORY=-DHEAP_DEBUG -DMY_MALLOC
# CSTLGEN=-DCSTLGEN
CFLAGS=-Wall -g $(MEMORY)
ifneq ($(MEMORY),)
	HEAP="heap"
endif

all: test
clean:
	rm -f *.exe *.o UChar* Int* UInt* Double* Ptr* Str* Hoge*

heap.o: heap.c heap.h
	$(CC) $(CFLAGS) -c heap.c

vector: ../cstl/vector.h vector_test.c heap.o
ifneq ($(CSTLGEN),)
	sh cstlgen.sh vector UCharVector "unsigned char" true false false . $(HEAP)
	sh cstlgen.sh vector IntVector "int" true false false . $(HEAP)
	$(CC) $(CFLAGS) -o $@.exe vector_test.c heap.o UCharVector.c IntVector.c $(CSTLGEN)
else
	$(CC) $(CFLAGS) -o $@.exe vector_test.c heap.o
endif
	./$@.exe

ring: ../cstl/ring.h ring_test.c heap.o
ifneq ($(CSTLGEN),)
	sh cstlgen.sh ring UCharRing "unsigned char" true false false . $(HEAP)
	sh cstlgen.sh ring IntRing "int" true false false . $(HEAP)
	$(CC) $(CFLAGS) -o $@.exe ring_test.c heap.o UCharRing.c IntRing.c $(CSTLGEN)
else
	$(CC) $(CFLAGS) -o $@.exe ring_test.c heap.o
endif
	./$@.exe

deque: ../cstl/deque.h ../cstl/vector.h ../cstl/ring.h deque_test.c heap.o deque_debug.h
ifneq ($(CSTLGEN),)
	sh cstlgen.sh deque UCharDeque "unsigned char" true false '%d' . $(HEAP)
	sh cstlgen.sh deque IntDeque "int" true false '%d' . $(HEAP)
	$(CC) $(CFLAGS) -o $@.exe deque_test.c heap.o UCharDeque.c IntDeque.c $(CSTLGEN)
else
	$(CC) $(CFLAGS) -o $@.exe deque_test.c heap.o
endif
	./$@.exe

list: ../cstl/list.h list_test.c heap.o list_debug.h
ifneq ($(CSTLGEN),)
	sh cstlgen.sh list IntList "int" false false '%d' . $(HEAP)
	sh cstlgen.sh list HogeList "Hoge" false hoge.h '%d' . $(HEAP)
	$(CC) $(CFLAGS) -o $@.exe list_test.c heap.o IntList.c HogeList.c $(CSTLGEN)
else
	$(CC) $(CFLAGS) -o $@.exe list_test.c heap.o
endif
	./$@.exe

set: ../cstl/set.h ../cstl/rbtree.h set_test.c heap.o rbtree_debug.h
ifneq ($(CSTLGEN),)
	sh cstlgen.sh set IntSetA "int" CSTL_LESS false '%d' . $(HEAP)
	sh cstlgen.sh set IntSetD "int" CSTL_GREATER false '%d' . $(HEAP)
	sh cstlgen.sh multiset IntMSetA "int" CSTL_LESS false '%d' . $(HEAP)
	sh cstlgen.sh set DoubleSetA "double" CSTL_LESS false '%g' . $(HEAP)
	sh cstlgen.sh set PtrSetA "int*" CSTL_LESS false '%p' . $(HEAP)
	sh cstlgen.sh set StrSetA "char*" strcmp false '%s' . $(HEAP)
	sh cstlgen.sh set UIntSetA "unsigned int" CSTL_LESS false '%d' . $(HEAP)
	$(CC) $(CFLAGS) -o $@.exe set_test.c heap.o IntSetA.c IntSetD.c IntMSetA.c DoubleSetA.c PtrSetA.c StrSetA.c UIntSetA.c $(CSTLGEN)
else
	$(CC) $(CFLAGS) -o $@.exe set_test.c heap.o
endif
	./$@.exe

map: ../cstl/map.h ../cstl/rbtree.h map_test.c heap.o rbtree_debug.h
ifneq ($(CSTLGEN),)
	sh cstlgen.sh map IntIntMapA "int" "int" CSTL_LESS false '%d' '%d' . $(HEAP)
	sh cstlgen.sh multimap IntIntMMapA "int" "int" CSTL_LESS false '%d' '%d' . $(HEAP)
	$(CC) $(CFLAGS) -o $@.exe map_test.c heap.o IntIntMapA.c IntIntMMapA.c $(CSTLGEN)
else
	$(CC) $(CFLAGS) -o $@.exe map_test.c heap.o
endif
	./$@.exe

string: ../cstl/string.h ../cstl/vector.h string_test.cpp heap.o
ifneq ($(CSTLGEN),)
	sh cstlgen.sh string String "char" true false false . $(HEAP)
	g++ $(CFLAGS) -o $@.exe string_test.cpp heap.o String.c $(CSTLGEN)
else
	g++ $(CFLAGS) -o $@.exe string_test.cpp heap.o
endif
	./$@.exe

algo: ../cstl/algorithm.h ../cstl/vector.h ../cstl/deque.h ../cstl/string.h algo_test.cpp heap.o
ifneq ($(CSTLGEN),)
	sh cstlgen.sh vector IntVector "int" true false false . $(HEAP)
	sh cstlgen.sh vector HogeVector "Hoge" true hoge.h false . $(HEAP)
	g++ $(CFLAGS) -o $@.exe algo_test.cpp heap.o IntVector.c HogeVector.c $(CSTLGEN)
else
	g++ $(CFLAGS) -o $@.exe algo_test.cpp heap.o
endif
	./$@.exe


test: vector ring deque list set map string algo
