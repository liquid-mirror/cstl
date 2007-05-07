#ifndef TEST_H_INCLUDED
#define TEST_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "../cstl/vector.h"
#include "../cstl/ring.h"
#include "../cstl/deque.h"
#include "../cstl/list.h"
#include "../cstl/set.h"
#include "../cstl/map.h"
#include "rbtree_debug.h"
#include "heap.h"
#ifdef MY_MALLOC
extern Heap heap;
#define malloc(s)		Heap_alloc(&heap, s)
#define realloc(p, s)	Heap_realloc(&heap, p, s)
#define free(p)			Heap_free(&heap, p)
#endif

/* vector */
VECTOR_INTERFACE(UCharVector, unsigned char)
VECTOR_INTERFACE(IntVector, int)


/* ring */
RING_INTERFACE(UCharRing, unsigned char)
RING_INTERFACE(IntRing, int)


/* deque */
DEQUE_INTERFACE(UCharDeque, unsigned char)
DEQUE_INTERFACE(IntDeque, int)


/* list */
typedef struct Hoge_t {
	char *key;
	int value;
} Hoge;

LIST_INTERFACE(UCharList, unsigned char)
LIST_INTERFACE(IntList, int)
LIST_INTERFACE(StrList, char *)
LIST_INTERFACE(HogeList, Hoge)


/* set */
SET_INTERFACE(IntSetA, int)
SET_DEBUG_INTERFACE(IntSetA)

SET_INTERFACE(IntSetD, int)
SET_DEBUG_INTERFACE(IntSetD)

MULTISET_INTERFACE(IntMSetA, int)
SET_DEBUG_INTERFACE(IntMSetA)

SET_INTERFACE(DoubleSetA, double)
SET_DEBUG_INTERFACE(DoubleSetA)

SET_INTERFACE(PtrSetA, int*)
SET_DEBUG_INTERFACE(PtrSetA)

SET_INTERFACE(StrSetA, char*)
SET_DEBUG_INTERFACE(StrSetA)

SET_INTERFACE(UIntSetA, unsigned int)
SET_DEBUG_INTERFACE(UIntSetA)

/* map */
MAP_INTERFACE(IntIntMapA, int, int)
MAP_DEBUG_INTERFACE(IntIntMapA)

MULTIMAP_INTERFACE(IntIntMMapA, int, int)
MAP_DEBUG_INTERFACE(IntIntMMapA)


void VectorTest_run(void);
void RingTest_run(void);
void DequeTest_run(void);
void ListTest_run(void);
void SetTest_run(void);
void MapTest_run(void);

#endif /* TEST_H_INCLUDED */
