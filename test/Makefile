CC=gcc
# MEMORY=-DHEAP_DEBUG -DMY_MALLOC
CFLAGS=-Wall -g $(MEMORY)
ifneq ($(MEMORY),)
	HEAP="heap"
endif

all: test
clean:
	rm -f *.exe *.o

heap.o: heap.c heap.h
	$(CC) $(CFLAGS) -c heap.c

vector: ../cstl/vector.h vector_test.c heap.o
	sh codegen.sh vector UCharVector "unsigned char" true false . $(HEAP)
	sh codegen.sh vector IntVector "int" true false . $(HEAP)
	$(CC) $(CFLAGS) -o $@.exe vector_test.c heap.o UCharVector.c IntVector.c
	./$@.exe

ring: ../cstl/ring.h ring_test.c heap.o
	sh codegen.sh ring UCharRing "unsigned char" true false . $(HEAP)
	sh codegen.sh ring IntRing "int" true false . $(HEAP)
	$(CC) $(CFLAGS) -o $@.exe ring_test.c heap.o UCharRing.c IntRing.c
	./$@.exe

deque: ../cstl/deque.h ../cstl/vector.h ../cstl/ring.h deque_test.c heap.o
	sh codegen.sh deque UCharDeque "unsigned char" true false . $(HEAP)
	sh codegen.sh deque IntDeque "int" true false . $(HEAP)
	$(CC) $(CFLAGS) -o $@.exe deque_test.c heap.o UCharDeque.c IntDeque.c
	./$@.exe

list: ../cstl/list.h list_test.c heap.o
	sh codegen.sh list UCharList "unsigned char" false false . $(HEAP)
	sh codegen.sh list IntList "int" false false . $(HEAP)
	sh codegen.sh list StrList "char*" false false . $(HEAP)
	sh codegen.sh list HogeList "Hoge" false hoge.h . $(HEAP)
	$(CC) $(CFLAGS) -o $@.exe list_test.c heap.o UCharList.c IntList.c StrList.c HogeList.c
	./$@.exe

set: ../cstl/set.h ../cstl/rbtree.h set_test.c heap.o rbtree_debug.h
	sh codegen.sh set IntSetA "int" CSTL_LESS false '%d' . $(HEAP)
	sh codegen.sh set IntSetD "int" CSTL_GREATER false '%d' . $(HEAP)
	sh codegen.sh multiset IntMSetA "int" CSTL_LESS false '%d' . $(HEAP)
	sh codegen.sh set DoubleSetA "double" CSTL_LESS false '%g' . $(HEAP)
	sh codegen.sh set PtrSetA "int*" CSTL_LESS false '%p' . $(HEAP)
	sh codegen.sh set StrSetA "char*" strcmp false '%s' . $(HEAP)
	sh codegen.sh set UIntSetA "unsigned int" CSTL_LESS false '%d' . $(HEAP)
	$(CC) $(CFLAGS) -o $@.exe set_test.c heap.o IntSetA.c IntSetD.c IntMSetA.c DoubleSetA.c PtrSetA.c StrSetA.c UIntSetA.c
	./$@.exe

map: ../cstl/map.h ../cstl/rbtree.h map_test.c heap.o rbtree_debug.h
	sh codegen.sh map IntIntMapA "int" "int" CSTL_LESS false '%d' '%d' . $(HEAP)
	sh codegen.sh multimap IntIntMMapA "int" "int" CSTL_LESS false '%d' '%d' . $(HEAP)
	$(CC) $(CFLAGS) -o $@.exe map_test.c heap.o IntIntMapA.c IntIntMMapA.c
	./$@.exe

string: ../cstl/string.h ../cstl/vector.h string_test.cpp heap.o
	sh codegen.sh string String "char" true false . $(HEAP)
	g++ $(CFLAGS) -o $@.exe string_test.cpp heap.o String.c
	./$@.exe

test: vector ring deque list set map string
