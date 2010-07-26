#include <stdio.h>
#include <string.h>
#include "../unittest/UnitTest.h"
#include <time.h>
#include "../cstl/ring.h"
#include "../cstl/set.h"
#include "rbtree_debug.h"
#include "Pool.h"
#ifdef MY_MALLOC
double buf[1024*1024/sizeof(double)];
Pool pool;
#define malloc(s)		Pool_malloc(&pool, s)
#define realloc(p, s)	Pool_realloc(&pool, p, s)
#define free(p)			Pool_free(&pool, p)
#endif


/* ring */
CSTL_RING_INTERFACE(IntRing, int)
CSTL_RING_IMPLEMENT(IntRing, int)



/* set */
#ifdef CSTLGEN
#include "IntSetA.h"
#include "IntSetD.h"
#include "IntMSetA.h"
#include "DoubleSetA.h"
#include "PtrSetA.h"
#include "StrSetA.h"
#include "UIntSetA.h"
#else

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

/* int */
CSTL_SET_IMPLEMENT(IntSetA, int, CSTL_LESS)
CSTL_SET_DEBUG_IMPLEMENT(IntSetA, int, CSTL_LESS, %d, VISUAL)

CSTL_SET_IMPLEMENT(IntSetD, int, CSTL_GREATER)
CSTL_SET_DEBUG_IMPLEMENT(IntSetD, int, CSTL_GREATER, %d, VISUAL)

CSTL_MULTISET_IMPLEMENT(IntMSetA, int, CSTL_LESS)
CSTL_SET_DEBUG_IMPLEMENT(IntMSetA, int, CSTL_LESS, %d, VISUAL)


/* double */
CSTL_SET_IMPLEMENT(DoubleSetA, double, CSTL_LESS)
CSTL_SET_DEBUG_IMPLEMENT(DoubleSetA, double, CSTL_LESS, %g, VISUAL)


/* ptr */
CSTL_SET_IMPLEMENT(PtrSetA, int*, CSTL_LESS)
CSTL_SET_DEBUG_IMPLEMENT(PtrSetA, int*, CSTL_LESS, %p, VISUAL)


/* str */
CSTL_SET_IMPLEMENT(StrSetA, char*, strcmp)
CSTL_SET_DEBUG_IMPLEMENT(StrSetA, char*, strcmp, %s, VISUAL)

/* unsigned int */
CSTL_SET_IMPLEMENT(UIntSetA, unsigned int, CSTL_LESS)
CSTL_SET_DEBUG_IMPLEMENT(UIntSetA, unsigned int, CSTL_LESS, %d, VISUAL)
#endif
static IntSetA *ia;
static IntSetD *id;
static IntMSetA *ima;
static DoubleSetA *da;
static PtrSetA *pa;
static StrSetA *sa;
static UIntSetA *uia;



#define SIZE	32
static int hoge_int[SIZE];
static double hoge_double[SIZE];
static int *hoge_ptr[SIZE];
static char *hoge_str[SIZE];
static char str[SIZE][16];

void set_init_hoge(void)
{
	int i;
	int tmp;
	IntRing *q = IntRing_new(SIZE/2);
	srand(time(0));
/*    printf("hoge data\n");*/
	for (i = 0; i < SIZE/2; i++) {
		IntRing_push_back(q, i);
	}
	for (i = 0; i < SIZE; i++) {
		if (!IntRing_empty(q)) {
			int rnd = rand() % IntRing_size(q);
			tmp = *IntRing_at(q, rnd);
			IntRing_erase(q, rnd, 1);
		} else {
			tmp = i/2;
		}
		hoge_int[i] = tmp;
		hoge_double[i] = hoge_int[i] * 0.5;
		hoge_ptr[i] = &hoge_int[tmp];
		sprintf(str[i], "%05d", i < SIZE/2 ? i : tmp);
		hoge_str[i] = str[i];
/*        printf("%4d: int[%3d], double[%5g], ptr[%p], str[%s]\n",*/
/*                i, hoge_int[i], hoge_double[i], hoge_ptr[i], hoge_str[i]);*/
	}
	POOL_DUMP_OVERFLOW(&pool);
	IntRing_delete(q);
}

void SetTest_test_1_1(void)
{
	int i;
	size_t count = 0;
	int success[SIZE];
	IntSetAIterator pos[SIZE];
	IntSetAIterator p;
	IntSetAReverseIterator rp;
	IntSetA *x;
	int b[] = {100, 101, 102, 103, 104, 105, 106, 107, 108, 109};
	ia = IntSetA_new();
	/* 初期状態 */
	ASSERT(cstl_empty(ia));
	ASSERT(cstl_size(ia) == 0);
/*    ASSERT(cstl_begin(ia) == cstl_end(ia));*/
	ASSERT(cstl_iter_eq(cstl_begin(ia), cstl_end(ia)));
/*    ASSERT(cstl_rbegin(ia) == cstl_rend(ia));*/
	ASSERT(cstl_iter_eq(cstl_rbegin(ia), cstl_rend(ia)));
	/* insert */
	for (i = 0; i < SIZE; i++) {
/*        pos[i] = cstl_set_insert(ia, hoge_int[i], &success[i]);*/
		ASSERT(cstl_set_insert(ia, hoge_int[i], &pos[i], &success[i]));
		ASSERT(IntSetA_verify(ia));
		if (i < SIZE/2) {
			ASSERT(success[i]);
			count++;
		} else {
			ASSERT(!success[i]);
		}
	}
/*    IntSetA_print(ia);*/
	ASSERT(!cstl_empty(ia));
	ASSERT(cstl_size(ia) == count);
	ASSERT(count == SIZE/2);
	/* count, find, lower_bound, upper_bound */
	for (i = 0; i < SIZE/2; i++) {
		ASSERT(cstl_count(ia, hoge_int[i]) == 1);
		ASSERT(cstl_iter_eq(pos[i], cstl_find(ia, hoge_int[i])));
		ASSERT(cstl_iter_eq(pos[i], cstl_lower_bound(ia, hoge_int[i])));
		ASSERT(cstl_iter_eq(pos[i], cstl_upper_bound(ia, hoge_int[i]-1)));
		ASSERT(cstl_iter_eq(cstl_lower_bound(ia, hoge_int[i]+1), cstl_upper_bound(ia, hoge_int[i])));
	}
	ASSERT(cstl_iter_eq(cstl_find(ia, *cstl_iter_data(cstl_begin(ia)) -1), cstl_end(ia)));
	ASSERT(cstl_iter_eq(cstl_lower_bound(ia, *cstl_iter_data(cstl_rbegin(ia)) +1), cstl_end(ia)));
	ASSERT(cstl_iter_eq(cstl_upper_bound(ia, *cstl_iter_data(cstl_rbegin(ia))), cstl_end(ia)));
	/* begin, end, next, data */
	for (p = cstl_begin(ia), i = 0; cstl_iter_ne(p, cstl_end(ia)); cstl_iter_inc(&p), i++) {
		ASSERT(*cstl_iter_data(p) == i);
	}
	ASSERT(i == SIZE/2);
/*    ASSERT(cstl_iter_eq(cstl_iter_next(cstl_rbegin(ia)), cstl_end(ia)));*/
	ASSERT(*cstl_iter_data(cstl_rbegin(ia)) == *cstl_iter_data(cstl_iter_prev(cstl_end(ia))));
	/* rbegin, rend, prev, data */
	for (rp = cstl_rbegin(ia), i = SIZE/2 -1; cstl_iter_ne(rp, cstl_rend(ia)); cstl_iter_inc(&rp), i--) {
		ASSERT(*cstl_iter_data(rp) == i);
	}
	ASSERT(i == -1);
/*    ASSERT(cstl_iter_eq(cstl_iter_prev(cstl_begin(ia)), cstl_rend(ia)));*/
	ASSERT(*cstl_iter_data(cstl_begin(ia)) == *cstl_iter_data(cstl_iter_prev(cstl_rend(ia))));
	/* erase */
	for (i = 0; i < SIZE; i++) {
/*        if (pos[i] && success[i]) {*/
		if (success[i]) {
			IntSetAIterator itr = cstl_iter_next(pos[i]);
/*            ASSERT(itr == cstl_erase(ia, pos[i]));*/
			ASSERT(cstl_iter_eq(itr, cstl_erase(ia, pos[i])));
			ASSERT(IntSetA_verify(ia));
			count--;
		}
	}
	ASSERT(cstl_empty(ia));
	ASSERT(cstl_size(ia) == 0);
	ASSERT(count == 0);
	/* erase_range */
	for (i = 0; i < SIZE/2; i++) {
/*        pos[i] = cstl_set_insert(ia, hoge_int[i], NULL);*/
		ASSERT(cstl_set_insert(ia, hoge_int[i], &pos[i], NULL));
	}
	ASSERT(cstl_size(ia) == SIZE/2);
	ASSERT(cstl_iter_eq(cstl_find(ia, SIZE/2 -2), cstl_erase_range(ia, cstl_find(ia, 2), cstl_find(ia, SIZE/2 -2))));
	ASSERT(IntSetA_verify(ia));
	ASSERT(cstl_size(ia) == 4);
/*    ASSERT(cstl_end(ia) == cstl_erase_range(ia, cstl_begin(ia), cstl_end(ia)));*/
	ASSERT(cstl_iter_eq(cstl_end(ia), cstl_erase_range(ia, cstl_begin(ia), cstl_end(ia))));
	ASSERT(cstl_size(ia) == 0);
	ASSERT(cstl_set_insert(ia, hoge_int[0], NULL, NULL));
	ASSERT(cstl_size(ia) == 1);
	p = cstl_iter_next(cstl_begin(ia));
	ASSERT(cstl_iter_eq(p, cstl_erase_range(ia, cstl_begin(ia), cstl_iter_next(cstl_begin(ia)))));
	ASSERT(cstl_size(ia) == 0);
	ASSERT(cstl_set_insert(ia, 100, NULL, NULL));
	ASSERT(cstl_set_insert(ia, 110, NULL, NULL));
	ASSERT(cstl_size(ia) == 2);
	ASSERT(cstl_iter_eq(cstl_upper_bound(ia, 110), cstl_erase_range(ia, cstl_lower_bound(ia, 100), cstl_upper_bound(ia, 110))));
	ASSERT(cstl_size(ia) == 0);
	/* erase_key */
	for (i = 0; i < SIZE/2; i++) {
/*        pos[i] = cstl_set_insert(ia, hoge_int[i], NULL);*/
/*        ASSERT(pos[i]);*/
		ASSERT(cstl_set_insert(ia, hoge_int[i], &pos[i], NULL));
	}
	ASSERT(cstl_size(ia) == SIZE/2);
	for (i = 0; i < SIZE/2; i++) {
		ASSERT(cstl_erase_key(ia, hoge_int[i]) == 1);
		ASSERT(IntSetA_verify(ia));
	}
	ASSERT(cstl_size(ia) == 0);
	/* 大量にinsert */
	count = 0;
	while (count < 1000000 && cstl_set_insert(ia, count, NULL, NULL)) {
		count++;
	}
	ASSERT(cstl_size(ia) == count);
	printf("count: %d\n", count);
	printf("size: %d\n", cstl_size(ia));
	/* clear */
	cstl_clear(ia);
	printf("size: %d\n", cstl_size(ia));
	ASSERT(cstl_size(ia) == 0);
	ASSERT(cstl_set_insert(ia, 100, NULL, NULL));
	ASSERT(cstl_size(ia) == 1);
	cstl_clear(ia);
	ASSERT(cstl_size(ia) == 0);
	cstl_clear(ia);
	ASSERT(cstl_size(ia) == 0);
	/* swap */
	for (i = 0; i < SIZE/2; i++) {
/*        pos[i] = cstl_set_insert(ia, hoge_int[i], &success[i]);*/
/*        ASSERT(pos[i]);*/
		ASSERT(cstl_set_insert(ia, hoge_int[i], &pos[i], &success[i]));
		ASSERT(success[i]);
	}
	ASSERT(cstl_size(ia) == SIZE/2);
	x = IntSetA_new();
	for (i = 0; i < sizeof b / sizeof b[0]; i++) {
/*        pos[i] = cstl_set_insert(x, b[i], &success[i]);*/
/*        ASSERT(pos[i]);*/
		ASSERT(cstl_set_insert(x, b[i], &pos[i], &success[i]));
		ASSERT(success[i]);
	}
	ASSERT(cstl_size(x) == sizeof b / sizeof b[0]);
/*    IntSetA_print(ia);*/
/*    IntSetA_print(x);*/

	cstl_swap(ia, x);

	ASSERT(IntSetA_verify(ia));
	ASSERT(IntSetA_verify(x));
/*    IntSetA_print(ia);*/
/*    IntSetA_print(x);*/
	ASSERT(cstl_size(x) == SIZE/2);
	ASSERT(cstl_size(ia) == sizeof b / sizeof b[0]);
	/* insert_range */
	count = cstl_size(x);
	cstl_set_insert(x, b[0], 0, 0);
	cstl_set_insert(ia, hoge_int[0], 0, 0);
/*    IntSetA_print(ia);*/
/*    IntSetA_print(x);*/
	ASSERT(cstl_assoc_insert_range(x, cstl_begin(ia), cstl_end(ia)));

/*    IntSetA_print(ia);*/
/*    IntSetA_print(x);*/
	ASSERT(cstl_size(ia) == sizeof b / sizeof b[0] + 1);
	ASSERT(cstl_size(x)  == count + sizeof b / sizeof b[0]);
	ASSERT(IntSetA_verify(ia));
	ASSERT(IntSetA_verify(x));

	ASSERT(cstl_assoc_insert_range(x, cstl_begin(ia), cstl_end(ia)));
	ASSERT(cstl_size(ia) == sizeof b / sizeof b[0] + 1);
	ASSERT(cstl_size(x)  == count + sizeof b / sizeof b[0]);
	ASSERT(IntSetA_verify(ia));
	ASSERT(IntSetA_verify(x));

	POOL_DUMP_OVERFLOW(&pool);
	cstl_delete(ia);
	cstl_delete(x);
}

void SetTest_test_1_2(void)
{
	int i;
	int success[SIZE];
	size_t count = 0;
	IntSetDIterator pos[SIZE];
	IntSetDIterator p;
	id = IntSetD_new();
	/* 初期状態 */
	ASSERT(cstl_empty(id));
	ASSERT(cstl_size(id) == 0);
	ASSERT(cstl_iter_eq(cstl_begin(id), cstl_end(id)));
	ASSERT(cstl_iter_eq(cstl_rbegin(id), cstl_rend(id)));
	/* insert */
	for (i = 0; i < SIZE; i++) {
/*        pos[i] = cstl_set_insert(id, hoge_int[i], &success[i]);*/
		ASSERT(cstl_set_insert(id, hoge_int[i], &pos[i], &success[i]));
		if (i < SIZE/2) {
			ASSERT(success[i]);
			count++;
		} else {
			ASSERT(!success[i]);
		}
	}
/*    IntSetD_print(id);*/
	ASSERT(!cstl_empty(id));
	ASSERT(cstl_size(id) == count);
	ASSERT(count == SIZE/2);
	/* count, find, lower_bound, upper_bound */
	for (i = 0; i < SIZE/2; i++) {
		ASSERT(cstl_count(id, hoge_int[i]) == 1);
		ASSERT(cstl_iter_eq(pos[i], cstl_find(id, hoge_int[i])));
		ASSERT(cstl_iter_eq(pos[i], cstl_lower_bound(id, hoge_int[i])));
		ASSERT(cstl_iter_eq(pos[i], cstl_upper_bound(id, hoge_int[i]+1)));
		ASSERT(cstl_iter_eq(cstl_lower_bound(id, hoge_int[i]-1), cstl_upper_bound(id, hoge_int[i])));
	}
	ASSERT(cstl_iter_eq(cstl_find(id, *cstl_iter_data(cstl_begin(id)) +1), cstl_end(id)));
	ASSERT(cstl_iter_eq(cstl_lower_bound(id, *cstl_iter_data(cstl_rbegin(id)) -1), cstl_end(id)));
	ASSERT(cstl_iter_eq(cstl_upper_bound(id, *cstl_iter_data(cstl_rbegin(id))), cstl_end(id)));
	/* begin, end, next, data */
	for (p = cstl_begin(id), i = SIZE/2 -1; cstl_iter_ne(p, cstl_end(id)); cstl_iter_inc(&p), i--) {
		ASSERT(*cstl_iter_data(p) == i);
	}
	ASSERT(i == -1);
/*    ASSERT(cstl_iter_next(cstl_rbegin(id)) == cstl_end(id));*/
	ASSERT(*cstl_iter_data(cstl_rbegin(id)) == *cstl_iter_data(cstl_iter_prev(cstl_end(id))));
	/* rbegin, rend, prev, data */
	for (p = cstl_rbegin(id), i = 0; cstl_iter_ne(p, cstl_rend(id)); cstl_iter_inc(&p), i++) {
		ASSERT(*cstl_iter_data(p) == i);
	}
	ASSERT(i == SIZE/2);
/*    ASSERT(cstl_iter_prev(cstl_begin(id)) == cstl_rend(id));*/
	ASSERT(*cstl_iter_data(cstl_begin(id)) == *cstl_iter_data(cstl_iter_prev(cstl_rend(id))));
	/* erase */
	for (i = 0; i < SIZE; i++) {
		if (success[i]) {
			IntSetDIterator itr = cstl_iter_next(pos[i]);
			ASSERT(cstl_iter_eq(itr, cstl_erase(id, pos[i])));
			count--;
		}
	}
	ASSERT(cstl_empty(id));
	ASSERT(cstl_size(id) == 0);
	ASSERT(count == 0);
	/* erase_range */
	for (i = 0; i < SIZE/2; i++) {
		ASSERT(cstl_set_insert(id, hoge_int[i], &pos[i], NULL));
/*        pos[i] = cstl_set_insert(id, hoge_int[i], NULL);*/
/*        ASSERT(pos[i]);*/
	}
	ASSERT(cstl_size(id) == SIZE/2);
	ASSERT(cstl_iter_eq(cstl_find(id, 1), cstl_erase_range(id, cstl_find(id, SIZE/2-1 -2), cstl_find(id, 1))));
	ASSERT(cstl_size(id) == 4);
	ASSERT(cstl_iter_eq(cstl_end(id), cstl_erase_range(id, cstl_begin(id), cstl_end(id))));
	ASSERT(cstl_size(id) == 0);
	ASSERT(cstl_set_insert(id, hoge_int[0], NULL, NULL));
	ASSERT(cstl_size(id) == 1);
	p = cstl_iter_next(cstl_begin(id));
	ASSERT(cstl_iter_eq(p, cstl_erase_range(id, cstl_begin(id), cstl_iter_next(cstl_begin(id)))));
/*    ASSERT(cstl_iter_next(cstl_begin(id)) == cstl_erase_range(id, cstl_begin(id), cstl_iter_next(cstl_begin(id))));*/
	ASSERT(cstl_size(id) == 0);
	ASSERT(cstl_set_insert(id, 100, NULL, NULL));
	ASSERT(cstl_set_insert(id, 110, NULL, NULL));
	ASSERT(cstl_size(id) == 2);
	ASSERT(cstl_iter_eq(cstl_upper_bound(id, 100), cstl_erase_range(id, cstl_lower_bound(id, 110), cstl_upper_bound(id, 100))));
	ASSERT(cstl_size(id) == 0);
	/* erase_key */
	for (i = 0; i < SIZE/2; i++) {
		ASSERT(cstl_set_insert(id, hoge_int[i], &pos[i], NULL));
/*        pos[i] = cstl_set_insert(id, hoge_int[i], NULL);*/
/*        ASSERT(pos[i]);*/
	}
	ASSERT(cstl_size(id) == SIZE/2);
	for (i = 0; i < SIZE/2; i++) {
		ASSERT(cstl_erase_key(id, hoge_int[i]) == 1);
	}
	ASSERT(cstl_size(id) == 0);
	/* 大量にinsert */
	count = 0;
	while (count < 1000000 && cstl_set_insert(id, count, NULL, NULL)) {
		count++;
	}
	ASSERT(cstl_size(id) == count);
	printf("count: %d\n", count);

	POOL_DUMP_OVERFLOW(&pool);
	cstl_delete(id);
}

void SetTest_test_1_3(void)
{
	int i;
	size_t count = 0;
	IntMSetAIterator pos[SIZE];
	IntMSetAIterator p;
	IntMSetA *x;
	int b[] = {100, 109, 101, 108, 102, 103, 104, 104, 105, 106, 107, 100, 101, 101, 108, 109};
	int flag[SIZE/2] = {0};
	ima = IntMSetA_new();
	/* 初期状態 */
	ASSERT(cstl_empty(ima));
	ASSERT(cstl_size(ima) == 0);
	ASSERT(cstl_iter_eq(cstl_begin(ima), cstl_end(ima)));
	ASSERT(cstl_iter_eq(cstl_rbegin(ima), cstl_rend(ima)));
	/* insert */
	for (i = 0; i < SIZE; i++) {
/*        pos[i] = cstl_multiset_insert(ima, hoge_int[i]);*/
		ASSERT(cstl_multiset_insert(ima, hoge_int[i], &pos[i]));
/*        ASSERT(pos[i] && pos[i] != cstl_end(ima));*/
		ASSERT(cstl_iter_ne(pos[i], cstl_end(ima)));
		count++;
	}
/*    IntMSetA_print(ima);*/
	ASSERT(!cstl_empty(ima));
	ASSERT(cstl_size(ima) == count);
	ASSERT(count == SIZE);
	/* count */
	for (i = 0; i < SIZE; i++) {
		if (i < SIZE/2/2) {
			ASSERT(cstl_count(ima, i) == 1);
		} else if (i < SIZE/2) {
			ASSERT(cstl_count(ima, i) == 3);
		} else {
			ASSERT(cstl_count(ima, i) == 0);
		}
	}
	/* find, lower_bound, upper_bound */
	for (i = 0; i < SIZE; i++) {
		if (cstl_count(ima, hoge_int[i]) == 1) {
			ASSERT(cstl_iter_eq(pos[i], cstl_find(ima, hoge_int[i])));
			ASSERT(cstl_iter_eq(pos[i], cstl_lower_bound(ima, hoge_int[i])));
			ASSERT(cstl_iter_eq(pos[i], cstl_upper_bound(ima, hoge_int[i]-1)));
		} else if (cstl_count(ima, hoge_int[i]) == 3) {
			if (!flag[hoge_int[i]]) {
				flag[hoge_int[i]] = 1;
				ASSERT(cstl_iter_eq(pos[i], cstl_lower_bound(ima, hoge_int[i])));
				ASSERT(cstl_iter_eq(pos[i], cstl_upper_bound(ima, hoge_int[i]-1)));
			}
		} else {
			ASSERT(0);
		}
		ASSERT(cstl_iter_eq(cstl_lower_bound(ima, hoge_int[i]+1), cstl_upper_bound(ima, hoge_int[i])));
	}
	ASSERT(cstl_iter_eq(cstl_find(ima, *cstl_iter_data(cstl_begin(ima)) -1), cstl_end(ima)));
	ASSERT(cstl_iter_eq(cstl_lower_bound(ima, *cstl_iter_data(cstl_rbegin(ima)) +1), cstl_end(ima)));
	ASSERT(cstl_iter_eq(cstl_upper_bound(ima, *cstl_iter_data(cstl_rbegin(ima))), cstl_end(ima)));
	/* begin, end, next, data */
	for (p = cstl_begin(ima), i = 0; cstl_iter_ne(p, cstl_end(ima)); cstl_iter_inc(&p), i++) {
/*        printf("%d, %d\n", i, *cstl_iter_data(p));*/
	}
	ASSERT(i == SIZE);
/*    ASSERT(cstl_iter_eq(cstl_iter_next(cstl_rbegin(ia)), cstl_end(ia)));*/
	ASSERT(*cstl_iter_data(cstl_rbegin(ima)) == *cstl_iter_data(cstl_iter_prev(cstl_end(ima))));
	/* rbegin, rend, prev, data */
	for (p = cstl_rbegin(ima), i = SIZE -1; cstl_iter_ne(p, cstl_rend(ima)); cstl_iter_inc(&p), i--) {
/*        printf("%d, %d\n", i, *cstl_iter_data(p));*/
	}
	ASSERT(i == -1);
/*    ASSERT(cstl_iter_prev(cstl_begin(ima)) == cstl_rend(ima));*/
	ASSERT(*cstl_iter_data(cstl_begin(ima)) == *cstl_iter_data(cstl_iter_prev(cstl_rend(ima))));
	/* erase */
	for (i = 0; i < SIZE; i++) {
		IntMSetAIterator itr = cstl_iter_next(pos[i]);
		ASSERT(cstl_iter_eq(itr, cstl_erase(ima, pos[i])));
		count--;
	}
	ASSERT(cstl_empty(ima));
	ASSERT(cstl_size(ima) == 0);
	ASSERT(count == 0);
	/* erase_range */
	for (i = 0; i < SIZE; i++) {
/*        pos[i] = cstl_multiset_insert(ima, hoge_int[i]);*/
		ASSERT(cstl_multiset_insert(ima, hoge_int[i], &pos[i]));
/*        ASSERT(pos[i] && pos[i] != cstl_end(ima));*/
		ASSERT(cstl_iter_ne(pos[i], cstl_end(ima)));
	}
	ASSERT(cstl_size(ima) == SIZE);
/*    ASSERT(cstl_iter_eq(cstl_iter_next(cstl_find(ima, SIZE/2/2 -1)), cstl_erase_range(ima, cstl_find(ima, 0), cstl_iter_next(cstl_find(ima, SIZE/2/2 -1)))));*/
	p = cstl_iter_next(cstl_find(ima, SIZE/2/2 -1));
	ASSERT(cstl_iter_eq(p, cstl_erase_range(ima, cstl_find(ima, 0), cstl_iter_next(cstl_find(ima, SIZE/2/2 -1)))));
	ASSERT(cstl_size(ima) == SIZE - SIZE/2/2);
	ASSERT(cstl_iter_eq(cstl_end(ima), cstl_erase_range(ima, cstl_begin(ima), cstl_end(ima))));
	ASSERT(cstl_size(ima) == 0);
	ASSERT(cstl_multiset_insert(ima, hoge_int[0], NULL));
	ASSERT(cstl_size(ima) == 1);
	p = cstl_iter_next(cstl_begin(ima));
	ASSERT(cstl_iter_eq(p, cstl_erase_range(ima, cstl_begin(ima), cstl_iter_next(cstl_begin(ima)))));
/*    ASSERT(cstl_iter_next(cstl_begin(ima)) == cstl_erase_range(ima, cstl_begin(ima), cstl_iter_next(cstl_begin(ima))));*/
	ASSERT(cstl_size(ima) == 0);
	ASSERT(cstl_multiset_insert(ima, 100, NULL));
	ASSERT(cstl_multiset_insert(ima, 100, NULL));
	ASSERT(cstl_multiset_insert(ima, 100, NULL));
	ASSERT(cstl_multiset_insert(ima, 110, NULL));
	ASSERT(cstl_multiset_insert(ima, 110, NULL));
	ASSERT(cstl_size(ima) == 5);
	ASSERT(cstl_iter_eq(cstl_upper_bound(ima, 110), cstl_erase_range(ima, cstl_lower_bound(ima, 100), cstl_upper_bound(ima, 110))));
	ASSERT(cstl_size(ima) == 0);
	/* erase_key */
	for (i = 0; i < SIZE; i++) {
/*        pos[i] = cstl_multiset_insert(ima, hoge_int[i]);*/
		ASSERT(cstl_multiset_insert(ima, hoge_int[i], &pos[i]));
/*        ASSERT(pos[i] && pos[i] != cstl_end(ima));*/
		ASSERT(cstl_iter_ne(pos[i], cstl_end(ima)));
	}
	ASSERT(cstl_size(ima) == SIZE);
	for (i = 0; i < SIZE/2/2; i++) {
		ASSERT(cstl_erase_key(ima, i) == 1);
	}
	ASSERT(cstl_size(ima) == SIZE - SIZE/2/2);
	for (i = SIZE/2/2; i < SIZE/2; i++) {
		ASSERT(cstl_erase_key(ima, i) == 3);
	}
	ASSERT(cstl_size(ima) == 0);
	/* 大量にinsert */
	count = 0;
	while (count < 1000000 && cstl_multiset_insert(ima, 0, NULL)) {
		count++;
	}
	ASSERT(cstl_size(ima) == count);
	printf("count: %d\n", count);

	cstl_clear(ima);

	/* insert_range */
	for (i = 0; i < SIZE/2; i++) {
		ASSERT(cstl_multiset_insert(ima, hoge_int[i], &pos[i]));
/*        pos[i] = cstl_multiset_insert(ima, hoge_int[i]);*/
/*        ASSERT(pos[i]);*/
	}
	ASSERT(cstl_size(ima) == SIZE/2);
	x = IntMSetA_new();
	for (i = 0; i < sizeof b / sizeof b[0]; i++) {
		ASSERT(cstl_multiset_insert(x, b[i], &pos[i]));
/*        pos[i] = cstl_multiset_insert(x, b[i]);*/
/*        ASSERT(pos[i]);*/
	}
	for (i = 0; i < SIZE/2; i++) {
		ASSERT(cstl_multiset_insert(x, hoge_int[i], &pos[i]));
/*        pos[i] = cstl_multiset_insert(x, hoge_int[i]);*/
/*        ASSERT(pos[i]);*/
	}
	ASSERT(cstl_size(x) == sizeof b / sizeof b[0] + SIZE/2);
/*    IntMSetA_print(ima);*/
/*    IntMSetA_print(x);*/
	ASSERT(cstl_assoc_insert_range(x, cstl_begin(ima), cstl_end(ima)));
	ASSERT(IntMSetA_verify(ima));
	ASSERT(IntMSetA_verify(x));
/*    IntMSetA_print(ima);*/
/*    IntMSetA_print(x);*/
	ASSERT(cstl_size(x) == sizeof b / sizeof b[0] + SIZE);


	POOL_DUMP_OVERFLOW(&pool);
	cstl_delete(ima);
	cstl_delete(x);
}

void SetTest_test_2_1(void)
{
	int i;
	int success[SIZE];
	size_t count = 0;
	DoubleSetAIterator pos[SIZE];
	DoubleSetAIterator p;
	da = DoubleSetA_new();
	/* 初期状態 */
	ASSERT(cstl_empty(da));
	ASSERT(cstl_size(da) == 0);
	ASSERT(cstl_iter_eq(cstl_begin(da), cstl_end(da)));
	ASSERT(cstl_iter_eq(cstl_rbegin(da), cstl_rend(da)));
	/* insert */
	for (i = 0; i < SIZE; i++) {
/*        pos[i] = cstl_set_insert(da, hoge_double[i], &success[i]);*/
		ASSERT(cstl_set_insert(da, hoge_double[i], &pos[i], &success[i]));
		if (i < SIZE/2) {
			ASSERT(success[i]);
			count++;
		} else {
			ASSERT(!success[i]);
		}
	}
	ASSERT(!cstl_empty(da));
	ASSERT(cstl_size(da) == count);
	ASSERT(count == SIZE/2);
	/* count, find, lower_bound, upper_bound */
	for (i = 0; i < SIZE/2; i++) {
		ASSERT(cstl_count(da, hoge_double[i]) == 1);
		ASSERT(cstl_iter_eq(pos[i], cstl_find(da, hoge_double[i])));
		ASSERT(cstl_iter_eq(pos[i], cstl_lower_bound(da, hoge_double[i])));
		ASSERT(cstl_iter_eq(pos[i], cstl_upper_bound(da, hoge_double[i]-0.5)));
		ASSERT(cstl_iter_eq(cstl_lower_bound(da, hoge_double[i]+0.5), cstl_upper_bound(da, hoge_double[i])));
	}
	ASSERT(cstl_iter_eq(cstl_find(da, *cstl_iter_data(cstl_begin(da)) -1), cstl_end(da)));
	ASSERT(cstl_iter_eq(cstl_lower_bound(da, *cstl_iter_data(cstl_rbegin(da)) +1), cstl_end(da)));
	ASSERT(cstl_iter_eq(cstl_upper_bound(da, *cstl_iter_data(cstl_rbegin(da))), cstl_end(da)));
	/* begin, end, next, data */
	for (p = cstl_begin(da), i = 0; cstl_iter_ne(p, cstl_end(da)); cstl_iter_inc(&p), i++) {
		ASSERT(*cstl_iter_data(p) == i*0.5);
	}
	ASSERT(i == SIZE/2);
/*    ASSERT(cstl_iter_next(cstl_rbegin(da)) == cstl_end(da));*/
	ASSERT(*cstl_iter_data(cstl_rbegin(da)) == *cstl_iter_data(cstl_iter_prev(cstl_end(da))));
	/* rbegin, rend, prev, data */
	for (p = cstl_rbegin(da), i = SIZE/2 -1; cstl_iter_ne(p, cstl_rend(da)); cstl_iter_inc(&p), i--) {
		ASSERT(*cstl_iter_data(p) == i*0.5);
	}
	ASSERT(i == -1);
/*    ASSERT(cstl_iter_prev(cstl_begin(da)) == cstl_rend(da));*/
	ASSERT(*cstl_iter_data(cstl_begin(da)) == *cstl_iter_data(cstl_iter_prev(cstl_rend(da))));
	/* erase */
	for (i = 0; i < SIZE; i++) {
		if (success[i]) {
			DoubleSetAIterator itr = cstl_iter_next(pos[i]);
			ASSERT(cstl_iter_eq(itr, cstl_erase(da, pos[i])));
			count--;
		}
	}
	ASSERT(cstl_empty(da));
	ASSERT(cstl_size(da) == 0);
	ASSERT(count == 0);
	/* erase_range */
	for (i = 0; i < SIZE/2; i++) {
		ASSERT(cstl_set_insert(da, hoge_double[i], &pos[i], NULL));
/*        pos[i] = cstl_set_insert(da, hoge_double[i], NULL);*/
/*        ASSERT(pos[i]);*/
	}
	ASSERT(cstl_size(da) == SIZE/2);
	ASSERT(cstl_iter_eq(cstl_find(da, SIZE/2 -2), cstl_erase_range(da, cstl_find(da, 2), cstl_find(da, SIZE/2 -2))));
	ASSERT(cstl_size(da) == 4);
	ASSERT(cstl_iter_eq(cstl_end(da), cstl_erase_range(da, cstl_begin(da), cstl_end(da))));
	ASSERT(cstl_size(da) == 0);
	ASSERT(cstl_set_insert(da, hoge_double[0], NULL, NULL));
	ASSERT(cstl_size(da) == 1);
	p = cstl_iter_next(cstl_begin(da));
	ASSERT(cstl_iter_eq(p, cstl_erase_range(da, cstl_begin(da), cstl_iter_next(cstl_begin(da)))));
/*    ASSERT(cstl_iter_next(cstl_begin(da)) == cstl_erase_range(da, cstl_begin(da), cstl_iter_next(cstl_begin(da))));*/
	ASSERT(cstl_size(da) == 0);
	ASSERT(cstl_set_insert(da, 100.1, NULL, NULL));
	ASSERT(cstl_set_insert(da, 110.1, NULL, NULL));
	ASSERT(cstl_size(da) == 2);
	ASSERT(cstl_iter_eq(cstl_upper_bound(da, 110.1), cstl_erase_range(da, cstl_lower_bound(da, 100.1), cstl_upper_bound(da, 110.1))));
	ASSERT(cstl_size(da) == 0);
	/* erase_key */
	for (i = 0; i < SIZE/2; i++) {
		ASSERT(cstl_set_insert(da, hoge_double[i], &pos[i], NULL));
/*        pos[i] = cstl_set_insert(da, hoge_double[i], NULL);*/
/*        ASSERT(pos[i]);*/
	}
	ASSERT(cstl_size(da) == SIZE/2);
	for (i = 0; i < SIZE/2; i++) {
		ASSERT(cstl_erase_key(da, hoge_double[i]) == 1);
	}
	ASSERT(cstl_size(da) == 0);
	/* 大量にinsert */
	count = 0;
	while (count < 1000000 && cstl_set_insert(da, count, NULL, NULL)) {
		count++;
	}
	ASSERT(cstl_size(da) == count);
	printf("count: %d\n", count);
	printf("size: %d\n", cstl_size(da));
	/* clear */
	cstl_clear(da);
	printf("size: %d\n", cstl_size(da));
	ASSERT(cstl_size(da) == 0);
	ASSERT(cstl_set_insert(da, 100, NULL, NULL));
	ASSERT(cstl_size(da) == 1);
	cstl_clear(da);
	ASSERT(cstl_size(da) == 0);
	cstl_clear(da);
	ASSERT(cstl_size(da) == 0);

	POOL_DUMP_OVERFLOW(&pool);
	cstl_delete(da);
}

void SetTest_test_3_1(void)
{
	int i;
	int success[SIZE];
	size_t count = 0;
	int *intp = NULL;
	PtrSetAIterator pos[SIZE];
	PtrSetAIterator p;
	pa = PtrSetA_new();
	/* 初期状態 */
	ASSERT(cstl_empty(pa));
	ASSERT(cstl_size(pa) == 0);
	ASSERT(cstl_iter_eq(cstl_begin(pa), cstl_end(pa)));
	ASSERT(cstl_iter_eq(cstl_rbegin(pa), cstl_rend(pa)));
	/* insert */
	for (i = 0; i < SIZE; i++) {
/*        pos[i] = cstl_set_insert(pa, hoge_ptr[i], &success[i]);*/
		ASSERT(cstl_set_insert(pa, hoge_ptr[i], &pos[i], &success[i]));
		if (i < SIZE/2) {
			ASSERT(success[i]);
			count++;
		} else {
			ASSERT(!success[i]);
		}
	}
	ASSERT(!cstl_empty(pa));
	ASSERT(cstl_size(pa) == count);
	ASSERT(count == SIZE/2);
	/* count, find, lower_bound, upper_bound */
	for (i = 0; i < SIZE/2; i++) {
		ASSERT(cstl_count(pa, hoge_ptr[i]) == 1);
		ASSERT(cstl_iter_eq(pos[i], cstl_find(pa, hoge_ptr[i])));
		ASSERT(cstl_iter_eq(pos[i], cstl_lower_bound(pa, hoge_ptr[i])));
		ASSERT(cstl_iter_eq(pos[i], cstl_upper_bound(pa, hoge_ptr[i]-1)));
		ASSERT(cstl_iter_eq(cstl_lower_bound(pa, hoge_ptr[i]+1), cstl_upper_bound(pa, hoge_ptr[i])));
	}
	ASSERT(cstl_iter_eq(cstl_find(pa, *cstl_iter_data(cstl_begin(pa)) -1), cstl_end(pa)));
	ASSERT(cstl_iter_eq(cstl_lower_bound(pa, *cstl_iter_data(cstl_rbegin(pa)) +1), cstl_end(pa)));
	ASSERT(cstl_iter_eq(cstl_upper_bound(pa, *cstl_iter_data(cstl_rbegin(pa))), cstl_end(pa)));
	/* begin, end, next, data */
	for (p = cstl_begin(pa), i = 0; cstl_iter_ne(p, cstl_end(pa)); cstl_iter_inc(&p), i++) {
		ASSERT(*cstl_iter_data(p) == &hoge_int[i]);
	}
	ASSERT(i == SIZE/2);
/*    ASSERT(cstl_iter_next(cstl_rbegin(pa)) == cstl_end(pa));*/
	ASSERT(*cstl_iter_data(cstl_rbegin(pa)) == *cstl_iter_data(cstl_iter_prev(cstl_end(pa))));
	/* rbegin, rend, prev, data */
	for (p = cstl_rbegin(pa), i = SIZE/2 -1; cstl_iter_ne(p, cstl_rend(pa)); cstl_iter_inc(&p), i--) {
		ASSERT(*cstl_iter_data(p) == &hoge_int[i]);
	}
	ASSERT(i == -1);
/*    ASSERT(cstl_iter_prev(cstl_begin(pa)) == cstl_rend(pa));*/
	ASSERT(*cstl_iter_data(cstl_begin(pa)) == *cstl_iter_data(cstl_iter_prev(cstl_rend(pa))));
	/* erase */
	for (i = 0; i < SIZE; i++) {
		if (success[i]) {
			PtrSetAIterator itr = cstl_iter_next(pos[i]);
			ASSERT(cstl_iter_eq(itr, cstl_erase(pa, pos[i])));
			count--;
		}
	}
	ASSERT(cstl_empty(pa));
	ASSERT(cstl_size(pa) == 0);
	ASSERT(count == 0);
	/* erase_range */
	for (i = 0; i < SIZE/2; i++) {
		ASSERT(cstl_set_insert(pa, hoge_ptr[i], &pos[i], &success[i]));
/*        pos[i] = cstl_set_insert(pa, hoge_ptr[i], NULL);*/
/*        ASSERT(pos[i]);*/
	}
	ASSERT(cstl_size(pa) == SIZE/2);
	ASSERT(cstl_iter_eq(cstl_find(pa, &hoge_int[SIZE/2 -2]), cstl_erase_range(pa, cstl_find(pa, &hoge_int[2]), cstl_find(pa, &hoge_int[SIZE/2 -2]))));
	ASSERT(cstl_size(pa) == 4);
	ASSERT(cstl_iter_eq(cstl_end(pa), cstl_erase_range(pa, cstl_begin(pa), cstl_end(pa))));
	ASSERT(cstl_size(pa) == 0);
	ASSERT(cstl_set_insert(pa, hoge_ptr[0], NULL, NULL));
	ASSERT(cstl_size(pa) == 1);
	p = cstl_iter_next(cstl_begin(pa));
	ASSERT(cstl_iter_eq(p, cstl_erase_range(pa, cstl_begin(pa), cstl_iter_next(cstl_begin(pa)))));
/*    ASSERT(cstl_iter_next(cstl_begin(pa)) == cstl_erase_range(pa, cstl_begin(pa), cstl_iter_next(cstl_begin(pa))));*/
	ASSERT(cstl_size(pa) == 0);
	ASSERT(cstl_set_insert(pa, (int*)100, NULL, NULL));
	ASSERT(cstl_set_insert(pa, (int*)100 + 1, NULL, NULL));
	ASSERT(cstl_size(pa) == 2);
	ASSERT(cstl_iter_eq(cstl_upper_bound(pa, (int*)100 + 1), cstl_erase_range(pa, cstl_lower_bound(pa, (int*)100), cstl_upper_bound(pa, (int*)100 + 1))));
	ASSERT(cstl_size(pa) == 0);
	/* erase_key */
	for (i = 0; i < SIZE/2; i++) {
		ASSERT(cstl_set_insert(pa, hoge_ptr[i], &pos[i], NULL));
/*        pos[i] = cstl_set_insert(pa, hoge_ptr[i], NULL);*/
/*        ASSERT(pos[i]);*/
	}
	ASSERT(cstl_size(pa) == SIZE/2);
	for (i = 0; i < SIZE/2; i++) {
		ASSERT(cstl_erase_key(pa, hoge_ptr[i]) == 1);
	}
	ASSERT(cstl_size(pa) == 0);
	/* 大量にinsert */
	count = 0;
	while (count < 1000000 && cstl_set_insert(pa, intp, NULL, NULL)) {
		count++;
		intp++;
	}
	ASSERT(cstl_size(pa) == count);
	printf("count: %d\n", count);
	printf("size: %d\n", cstl_size(pa));
	/* clear */
	cstl_clear(pa);
	printf("size: %d\n", cstl_size(pa));
	ASSERT(cstl_size(pa) == 0);
	ASSERT(cstl_set_insert(pa, intp, NULL, NULL));
	ASSERT(cstl_size(pa) == 1);
	cstl_clear(pa);
	ASSERT(cstl_size(pa) == 0);
	cstl_clear(pa);
	ASSERT(cstl_size(pa) == 0);

	POOL_DUMP_OVERFLOW(&pool);
	cstl_delete(pa);
}

void SetTest_test_4_1(void)
{
	int i;
	int success[SIZE];
	size_t count = 0;
	StrSetAIterator pos[SIZE];
	StrSetAIterator p;
	sa = StrSetA_new();
	/* 初期状態 */
	ASSERT(cstl_empty(sa));
	ASSERT(cstl_size(sa) == 0);
	ASSERT(cstl_iter_eq(cstl_begin(sa), cstl_end(sa)));
	ASSERT(cstl_iter_eq(cstl_rbegin(sa), cstl_rend(sa)));
	/* insert */
	for (i = 0; i < SIZE; i++) {
		ASSERT(cstl_set_insert(sa, hoge_str[i], &pos[i], &success[i]));
/*        pos[i] = cstl_set_insert(sa, hoge_str[i], &success[i]);*/
		if (i < SIZE/2) {
			ASSERT(success[i]);
			count++;
		} else {
			ASSERT(!success[i]);
		}
	}
/*    StrSetA_print(sa);*/
	ASSERT(!cstl_empty(sa));
	ASSERT(cstl_size(sa) == count);
	ASSERT(count == SIZE/2);
	/* count, find, lower_bound, upper_bound */
	for (i = 0; i < SIZE/2; i++) {
		ASSERT(cstl_count(sa, hoge_str[i]) == 1);
		ASSERT(cstl_iter_eq(pos[i], cstl_find(sa, hoge_str[i])));
		ASSERT(cstl_iter_eq(pos[i], cstl_lower_bound(sa, hoge_str[i])));
		if (i != SIZE/2-1) {
			ASSERT(cstl_iter_eq(pos[i+1], cstl_upper_bound(sa, hoge_str[i])));
			ASSERT(cstl_iter_eq(cstl_lower_bound(sa, hoge_str[i+1]), cstl_upper_bound(sa, hoge_str[i])));
		}
	}
	ASSERT(cstl_iter_eq(cstl_find(sa, "hoge"), cstl_end(sa)));
	ASSERT(cstl_iter_eq(cstl_lower_bound(sa, "hoge"), cstl_end(sa)));
	ASSERT(cstl_iter_eq(cstl_upper_bound(sa, *cstl_iter_data(cstl_rbegin(sa))), cstl_end(sa)));
	/* begin, end, next, data */
	for (p = cstl_begin(sa), i = 0; cstl_iter_ne(p, cstl_end(sa)); cstl_iter_inc(&p), i++) {
		ASSERT(*cstl_iter_data(p) == hoge_str[i]);
	}
	ASSERT(i == SIZE/2);
/*    ASSERT(cstl_iter_next(cstl_rbegin(sa)) == cstl_end(sa));*/
	ASSERT(*cstl_iter_data(cstl_rbegin(sa)) == *cstl_iter_data(cstl_iter_prev(cstl_end(sa))));
	/* rbegin, rend, prev, data */
	for (p = cstl_rbegin(sa), i = SIZE/2 -1; cstl_iter_ne(p, cstl_rend(sa)); cstl_iter_inc(&p), i--) {
		ASSERT(*cstl_iter_data(p) == hoge_str[i]);
	}
	ASSERT(i == -1);
/*    ASSERT(cstl_iter_prev(cstl_begin(sa)) == cstl_rend(sa));*/
	ASSERT(*cstl_iter_data(cstl_begin(sa)) == *cstl_iter_data(cstl_iter_prev(cstl_rend(sa))));
	/* erase */
	for (i = 0; i < SIZE; i++) {
		if (success[i]) {
			StrSetAIterator itr = cstl_iter_next(pos[i]);
			ASSERT(cstl_iter_eq(itr, cstl_erase(sa, pos[i])));
			count--;
		}
	}
	ASSERT(cstl_empty(sa));
	ASSERT(cstl_size(sa) == 0);
	ASSERT(count == 0);
	/* erase_range */
	for (i = 0; i < SIZE/2; i++) {
		ASSERT(cstl_set_insert(sa, hoge_str[i], &pos[i], NULL));
/*        pos[i] = cstl_set_insert(sa, hoge_str[i], NULL);*/
/*        ASSERT(pos[i]);*/
	}
	ASSERT(cstl_size(sa) == SIZE/2);
	ASSERT(cstl_iter_eq(cstl_find(sa, hoge_str[SIZE/2 -2]), cstl_erase_range(sa, cstl_find(sa, hoge_str[2]), cstl_find(sa, hoge_str[SIZE/2 -2]))));
	ASSERT(cstl_size(sa) == 4);
	ASSERT(cstl_iter_eq(cstl_end(sa), cstl_erase_range(sa, cstl_begin(sa), cstl_end(sa))));
	ASSERT(cstl_size(sa) == 0);
	ASSERT(cstl_set_insert(sa, hoge_str[0], NULL, NULL));
	ASSERT(cstl_size(sa) == 1);
	p = cstl_iter_next(cstl_begin(sa));
	ASSERT(cstl_iter_eq(p, cstl_erase_range(sa, cstl_begin(sa), cstl_iter_next(cstl_begin(sa)))));
/*    ASSERT(cstl_iter_next(cstl_begin(sa)) == cstl_erase_range(sa, cstl_begin(sa), cstl_iter_next(cstl_begin(sa))));*/
	ASSERT(cstl_size(sa) == 0);
	ASSERT(cstl_set_insert(sa, "100", NULL, NULL));
	ASSERT(cstl_set_insert(sa, "110", NULL, NULL));
	ASSERT(cstl_size(sa) == 2);
	ASSERT(cstl_iter_eq(cstl_upper_bound(sa, "110"), cstl_erase_range(sa, cstl_lower_bound(sa, "100"), cstl_upper_bound(sa, "110"))));
	ASSERT(cstl_size(sa) == 0);
	/* erase_key */
	for (i = 0; i < SIZE/2; i++) {
		ASSERT(cstl_set_insert(sa, hoge_str[i], &pos[i], NULL));
/*        pos[i] = cstl_set_insert(sa, hoge_str[i], NULL);*/
/*        ASSERT(pos[i]);*/
	}
	ASSERT(cstl_size(sa) == SIZE/2);
	for (i = 0; i < SIZE/2; i++) {
		ASSERT(cstl_erase_key(sa, hoge_str[i]) == 1);
	}
	ASSERT(cstl_size(sa) == 0);
	/* clear */
	ASSERT(cstl_set_insert(sa, "103", NULL, NULL));
	ASSERT(cstl_set_insert(sa, "101", NULL, NULL));
	ASSERT(cstl_set_insert(sa, "100", NULL, NULL));
	ASSERT(cstl_set_insert(sa, "104", NULL, NULL));
	ASSERT(cstl_set_insert(sa, "106", NULL, NULL));
	ASSERT(cstl_set_insert(sa, "105", NULL, NULL));
	ASSERT(cstl_set_insert(sa, "102", NULL, NULL));
	printf("size: %d\n", cstl_size(sa));
	cstl_clear(sa);
	printf("size: %d\n", cstl_size(sa));
	ASSERT(cstl_size(sa) == 0);
	ASSERT(cstl_set_insert(sa, "100", NULL, NULL));
	ASSERT(cstl_size(sa) == 1);
	cstl_clear(sa);
	ASSERT(cstl_size(sa) == 0);
	cstl_clear(sa);
	ASSERT(cstl_size(sa) == 0);

	POOL_DUMP_OVERFLOW(&pool);
	cstl_delete(sa);
}

void SetTest_test_5_1(void)
{
	int i;
	size_t count = 0;
	int success[SIZE];
	UIntSetAIterator pos[SIZE];
	UIntSetAIterator p;
	uia = UIntSetA_new();
	/* 初期状態 */
	ASSERT(cstl_empty(uia));
	ASSERT(cstl_size(uia) == 0);
	ASSERT(cstl_iter_eq(cstl_begin(uia), cstl_end(uia)));
	ASSERT(cstl_iter_eq(cstl_rbegin(uia), cstl_rend(uia)));
	/* insert */
	for (i = 0; i < SIZE; i++) {
		ASSERT(cstl_set_insert(uia, hoge_int[i], &pos[i], &success[i]));
/*        pos[i] = cstl_set_insert(uia, hoge_int[i], &success[i]);*/
/*        UIntSetA_print(uia);*/
		if (i < SIZE/2) {
			ASSERT(success[i]);
			count++;
		} else {
			ASSERT(!success[i]);
		}
	}
/*    UIntSetA_print(uia);*/
	ASSERT(!cstl_empty(uia));
	ASSERT(cstl_size(uia) == count);
	ASSERT(count == SIZE/2);
	/* count, find, lower_bound, upper_bound */
	for (i = 0; i < SIZE/2; i++) {
		ASSERT(cstl_count(uia, hoge_int[i]) == 1);
		ASSERT(cstl_iter_eq(pos[i], cstl_find(uia, hoge_int[i])));
		ASSERT(cstl_iter_eq(pos[i], cstl_lower_bound(uia, hoge_int[i])));
		if (hoge_int[i] == 0) {
			ASSERT(cstl_iter_eq(cstl_end(uia), cstl_upper_bound(uia, hoge_int[i]-1)));
		} else {
			ASSERT(cstl_iter_eq(pos[i], cstl_upper_bound(uia, hoge_int[i]-1)));
		}
		ASSERT(cstl_iter_eq(cstl_lower_bound(uia, hoge_int[i]+1), cstl_upper_bound(uia, hoge_int[i])));
	}
	ASSERT(cstl_iter_eq(cstl_find(uia, *cstl_iter_data(cstl_begin(uia)) -1), cstl_end(uia)));
	ASSERT(cstl_iter_eq(cstl_lower_bound(uia, *cstl_iter_data(cstl_rbegin(uia)) +1), cstl_end(uia)));
	ASSERT(cstl_iter_eq(cstl_upper_bound(uia, *cstl_iter_data(cstl_rbegin(uia))), cstl_end(uia)));
	/* begin, end, next, data */
	for (p = cstl_begin(uia), i = 0; cstl_iter_ne(p, cstl_end(uia)); cstl_iter_inc(&p), i++) {
		ASSERT(*cstl_iter_data(p) == i);
	}
	ASSERT(i == SIZE/2);
/*    ASSERT(cstl_iter_next(cstl_rbegin(uia)) == cstl_end(uia));*/
	ASSERT(*cstl_iter_data(cstl_rbegin(uia)) == *cstl_iter_data(cstl_iter_prev(cstl_end(uia))));
	/* rbegin, rend, prev, data */
	for (p = cstl_rbegin(uia), i = SIZE/2 -1; cstl_iter_ne(p, cstl_rend(uia)); cstl_iter_inc(&p), i--) {
		ASSERT(*cstl_iter_data(p) == i);
	}
	ASSERT(i == -1);
/*    ASSERT(cstl_iter_prev(cstl_begin(uia)) == cstl_rend(uia));*/
	ASSERT(*cstl_iter_data(cstl_begin(uia)) == *cstl_iter_data(cstl_iter_prev(cstl_rend(uia))));
	/* erase */
	for (i = 0; i < SIZE; i++) {
		if (success[i]) {
			UIntSetAIterator itr = cstl_iter_next(pos[i]);
			ASSERT(cstl_iter_eq(itr, cstl_erase(uia, pos[i])));
			count--;
		}
	}
	ASSERT(cstl_empty(uia));
	ASSERT(cstl_size(uia) == 0);
	ASSERT(count == 0);
	/* erase_range */
	for (i = 0; i < SIZE/2; i++) {
		ASSERT(cstl_set_insert(uia, hoge_int[i], &pos[i], NULL));
/*        pos[i] = cstl_set_insert(uia, hoge_int[i], NULL);*/
/*        ASSERT(pos[i]);*/
	}
	ASSERT(cstl_size(uia) == SIZE/2);
	ASSERT(cstl_iter_eq(cstl_find(uia, SIZE/2 -2), cstl_erase_range(uia, cstl_find(uia, 2), cstl_find(uia, SIZE/2 -2))));
	ASSERT(cstl_size(uia) == 4);
	ASSERT(cstl_iter_eq(cstl_end(uia), cstl_erase_range(uia, cstl_begin(uia), cstl_end(uia))));
	ASSERT(cstl_size(uia) == 0);
	ASSERT(cstl_set_insert(uia, hoge_int[0], NULL, NULL));
	ASSERT(cstl_size(uia) == 1);
	p = cstl_iter_next(cstl_begin(uia));
	ASSERT(cstl_iter_eq(p, cstl_erase_range(uia, cstl_begin(uia), cstl_iter_next(cstl_begin(uia)))));
/*    ASSERT(cstl_iter_next(cstl_begin(uia)) == cstl_erase_range(uia, cstl_begin(uia), cstl_iter_next(cstl_begin(uia))));*/
	ASSERT(cstl_size(uia) == 0);
	ASSERT(cstl_set_insert(uia, 100, NULL, NULL));
	ASSERT(cstl_set_insert(uia, 110, NULL, NULL));
	ASSERT(cstl_size(uia) == 2);
	ASSERT(cstl_iter_eq(cstl_upper_bound(uia, 110), cstl_erase_range(uia, cstl_lower_bound(uia, 100), cstl_upper_bound(uia, 110))));
	ASSERT(cstl_size(uia) == 0);
	/* erase_key */
	for (i = 0; i < SIZE/2; i++) {
		ASSERT(cstl_set_insert(uia, hoge_int[i], &pos[i], NULL));
/*        pos[i] = cstl_set_insert(uia, hoge_int[i], NULL);*/
/*        ASSERT(pos[i]);*/
	}
	ASSERT(cstl_size(uia) == SIZE/2);
	for (i = 0; i < SIZE/2; i++) {
		ASSERT(cstl_erase_key(uia, hoge_int[i]) == 1);
	}
	ASSERT(cstl_size(uia) == 0);
	/* 大量にinsert */
	count = 0;
	while (count < 1000000 && cstl_set_insert(uia, count, NULL, NULL)) {
		count++;
	}
	ASSERT(cstl_size(uia) == count);
	printf("count: %d\n", count);
	printf("size: %d\n", cstl_size(uia));
	/* clear */
	cstl_clear(uia);
	printf("size: %d\n", cstl_size(uia));
	ASSERT(cstl_size(uia) == 0);
	ASSERT(cstl_set_insert(uia, 100, NULL, NULL));
	ASSERT(cstl_size(uia) == 1);
	cstl_clear(uia);
	ASSERT(cstl_size(uia) == 0);
	cstl_clear(uia);
	ASSERT(cstl_size(uia) == 0);

	POOL_DUMP_OVERFLOW(&pool);
	cstl_delete(uia);
}


static TestCase set_tests[] = {
	{ "SetTest_test_1_1", SetTest_test_1_1 },
	{ "SetTest_test_1_2", SetTest_test_1_2 },
	{ "SetTest_test_1_3", SetTest_test_1_3 },
	{ "SetTest_test_2_1", SetTest_test_2_1 },
	{ "SetTest_test_3_1", SetTest_test_3_1 },
	{ "SetTest_test_4_1", SetTest_test_4_1 },
	{ "SetTest_test_5_1", SetTest_test_5_1 },
	TEST_CASE_NULL,
};

static int setup(void)
{
	set_init_hoge();
	return 0;
}

static TestSuite suites[] = {
	{ "set test", setup, 0, set_tests },
	TEST_SUITE_NULL,
};


int main(int argc, char *argv[])
{
#ifdef MY_MALLOC
	Pool_init(&pool, buf, sizeof buf, sizeof buf[0]);
#endif
	if (argc < 2) {
		unittest_run_all(suites);
	} else {
		unittest_run_interactive(suites);
	}
#ifdef MY_MALLOC
	POOL_DUMP_LEAK(&pool, 0);
#endif
	return 0;
}
