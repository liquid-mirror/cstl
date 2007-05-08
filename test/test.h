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
CSTL_VECTOR_INTERFACE(UCharVector, unsigned char)
CSTL_VECTOR_INTERFACE(IntVector, int)


/* ring */
CSTL_RING_INTERFACE(UCharRing, unsigned char)
CSTL_RING_INTERFACE(IntRing, int)


/* deque */
CSTL_DEQUE_INTERFACE(UCharDeque, unsigned char)
CSTL_DEQUE_INTERFACE(IntDeque, int)


/* list */
typedef struct Hoge_t {
	char *key;
	int value;
} Hoge;

CSTL_LIST_INTERFACE(UCharList, unsigned char)
CSTL_LIST_INTERFACE(IntList, int)
CSTL_LIST_INTERFACE(StrList, char *)
CSTL_LIST_INTERFACE(HogeList, Hoge)


/* set */
CSTL_SET_INTERFACE(IntSetA, int)
CSTL_SET_DEBUG_INTERFACE(IntSetA)

CSTL_SET_INTERFACE(IntSetD, int)
CSTL_SET_DEBUG_INTERFACE(IntSetD)

CSTL_MULTISET_INTERFACE(IntMSetA, int)
CSTL_SET_DEBUG_INTERFACE(IntMSetA)

CSTL_SET_INTERFACE(DoubleSetA, double)
CSTL_SET_DEBUG_INTERFACE(DoubleSetA)

CSTL_SET_INTERFACE(PtrSetA, int*)
CSTL_SET_DEBUG_INTERFACE(PtrSetA)

CSTL_SET_INTERFACE(StrSetA, char*)
CSTL_SET_DEBUG_INTERFACE(StrSetA)

CSTL_SET_INTERFACE(UIntSetA, unsigned int)
CSTL_SET_DEBUG_INTERFACE(UIntSetA)

/* map */
CSTL_MAP_INTERFACE(IntIntMapA, int, int)
CSTL_MAP_DEBUG_INTERFACE(IntIntMapA)

CSTL_MULTIMAP_INTERFACE(IntIntMMapA, int, int)
CSTL_MAP_DEBUG_INTERFACE(IntIntMMapA)


void VectorTest_run(void);
void RingTest_run(void);
void DequeTest_run(void);
void ListTest_run(void);
void SetTest_run(void);
void MapTest_run(void);

#endif /* TEST_H_INCLUDED */
