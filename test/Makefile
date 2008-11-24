CC=gcc
# MEMORY=-DPOOL_DEBUG -DMY_MALLOC
# CSTLGEN=-DCSTLGEN
CFLAGS=-Wall -g $(MEMORY) #-fprofile-arcs -ftest-coverage
ifneq ($(MEMORY),)
	POOL="pool"
endif

all: test
clean:
	rm -f *.exe *.o UChar* Int* UInt* Double* Ptr* Str* Hoge* WString* #*.gc*

Pool.o: Pool.c Pool.h
	$(CC) $(CFLAGS) -c Pool.c

vector: ../cstl/vector.h vector_test.c Pool.o
ifneq ($(CSTLGEN),)
	sh cstlgen.sh vector UCharVector "unsigned char" true false false . $(POOL)
	sh cstlgen.sh vector IntVector "int" true false false . $(POOL)
	sh cstlgen.sh vector HogeVector "Hoge" true hoge.h false . $(POOL)
	sh cstlgen.sh vector HogepVector "Hoge*" true hoge.h false . $(POOL)
	$(CC) $(CFLAGS) -o $@.exe vector_test.c Pool.o UCharVector.c IntVector.c $(CSTLGEN)
else
	$(CC) $(CFLAGS) -o $@.exe vector_test.c Pool.o
endif
	./$@.exe

ring: ../cstl/ring.h ring_test.c Pool.o
ifneq ($(CSTLGEN),)
	sh cstlgen.sh ring UCharRing "unsigned char" true false false . $(POOL)
	sh cstlgen.sh ring IntRing "int" true false false . $(POOL)
	sh cstlgen.sh ring HogeRing "Hoge" true hoge.h false . $(POOL)
	sh cstlgen.sh ring HogepRing "Hoge*" true hoge.h false . $(POOL)
	$(CC) $(CFLAGS) -o $@.exe ring_test.c Pool.o UCharRing.c IntRing.c $(CSTLGEN)
else
	$(CC) $(CFLAGS) -o $@.exe ring_test.c Pool.o
endif
	./$@.exe

deque: ../cstl/deque.h ../cstl/vector.h ../cstl/ring.h deque_test.c Pool.o deque_debug.h
ifneq ($(CSTLGEN),)
	sh cstlgen.sh deque UCharDeque "unsigned char" true false '%d' . $(POOL)
	sh cstlgen.sh deque IntDeque "int" true false '%d' . $(POOL)
	sh cstlgen.sh deque HogeDeque "Hoge" true hoge.h '%d' . $(POOL)
	sh cstlgen.sh deque HogepDeque "Hoge*" true hoge.h '%p' . $(POOL)
	$(CC) $(CFLAGS) -o $@.exe deque_test.c Pool.o UCharDeque.c IntDeque.c $(CSTLGEN)
else
	$(CC) $(CFLAGS) -o $@.exe deque_test.c Pool.o
endif
	./$@.exe

list: ../cstl/list.h list_test.c Pool.o list_debug.h
ifneq ($(CSTLGEN),)
	sh cstlgen.sh list IntList "int" false false '%d' . $(POOL)
	sh cstlgen.sh list HogeList "Hoge" false hoge.h '%d' . $(POOL)
	sh cstlgen.sh list HogepList "Hoge*" false hoge.h '%p' . $(POOL)
	$(CC) $(CFLAGS) -o $@.exe list_test.c Pool.o IntList.c HogeList.c $(CSTLGEN)
else
	$(CC) $(CFLAGS) -o $@.exe list_test.c Pool.o
endif
	./$@.exe

set: ../cstl/set.h ../cstl/rbtree.h set_test.c Pool.o rbtree_debug.h
ifneq ($(CSTLGEN),)
	sh cstlgen.sh set IntSetA "int" CSTL_LESS false '%d' . $(POOL)
	sh cstlgen.sh set IntSetD "int" CSTL_GREATER false '%d' . $(POOL)
	sh cstlgen.sh multiset IntMSetA "int" CSTL_LESS false '%d' . $(POOL)
	sh cstlgen.sh set DoubleSetA "double" CSTL_LESS false '%g' . $(POOL)
	sh cstlgen.sh set PtrSetA "int*" CSTL_LESS false '%p' . $(POOL)
	sh cstlgen.sh set StrSetA "char*" strcmp false '%s' . $(POOL)
	sh cstlgen.sh set UIntSetA "unsigned int" CSTL_LESS false '%d' . $(POOL)
	sh cstlgen.sh set HogeSetA "Hoge" HOGE_COMP hoge.h '%d' . $(POOL)
	sh cstlgen.sh set HogepSetA "Hoge*" HOGEP_COMP hoge.h '%p' . $(POOL)
	$(CC) $(CFLAGS) -o $@.exe set_test.c Pool.o IntSetA.c IntSetD.c IntMSetA.c DoubleSetA.c PtrSetA.c StrSetA.c UIntSetA.c $(CSTLGEN)
else
	$(CC) $(CFLAGS) -o $@.exe set_test.c Pool.o
endif
	./$@.exe

map: ../cstl/map.h ../cstl/rbtree.h map_test.c Pool.o rbtree_debug.h
ifneq ($(CSTLGEN),)
	sh cstlgen.sh map IntIntMapA "int" "int" CSTL_LESS false '%d' '%d' . $(POOL)
	sh cstlgen.sh map HogeIntMapA "Hoge" "int" HOGE_COMP hoge.h '%d' '%d' . $(POOL)
	sh cstlgen.sh map HogepIntMapA "Hoge*" "int" HOGEP_COMP hoge.h '%p' '%d' . $(POOL)
	sh cstlgen.sh multimap IntIntMMapA "int" "int" CSTL_LESS false '%d' '%d' . $(POOL)
	$(CC) $(CFLAGS) -o $@.exe map_test.c Pool.o IntIntMapA.c IntIntMMapA.c $(CSTLGEN)
else
	$(CC) $(CFLAGS) -o $@.exe map_test.c Pool.o
endif
	./$@.exe

string: ../cstl/string.h ../cstl/vector.h string_test.cpp Pool.o
ifneq ($(CSTLGEN),)
	sh cstlgen.sh string String "char" true false false . $(POOL)
	sh cstlgen.sh string WString "wchar_t" true false false . $(POOL)
	g++ $(CFLAGS) -o $@.exe string_test.cpp Pool.o String.c $(CSTLGEN)
else
	g++ $(CFLAGS) -o $@.exe string_test.cpp Pool.o
endif
	./$@.exe

algo: ../cstl/algorithm.h ../cstl/vector.h ../cstl/deque.h ../cstl/string.h algo_test.cpp Pool.o
ifneq ($(CSTLGEN),)
	sh cstlgen.sh vector IntVector "int" true false false . $(POOL)
	sh cstlgen.sh vector HogeVector "Hoge" true hoge.h false . $(POOL)
	g++ $(CFLAGS) -o $@.exe algo_test.cpp Pool.o IntVector.c HogeVector.c $(CSTLGEN)
else
	g++ $(CFLAGS) -o $@.exe algo_test.cpp Pool.o
endif
	./$@.exe


test: vector ring deque list set map string algo
