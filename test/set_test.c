#include <stdio.h>
#include <string.h>
#include <assert.h>
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
	IntSetA *x;
	int b[] = {100, 101, 102, 103, 104, 105, 106, 107, 108, 109};
	printf("***** test_1_1 *****\n");
	ia = IntSetA_new();
	/* 初期状態 */
	assert(IntSetA_empty(ia));
	assert(IntSetA_size(ia) == 0);
	assert(IntSetA_begin(ia) == IntSetA_end(ia));
	assert(IntSetA_rbegin(ia) == IntSetA_rend(ia));
	/* insert */
	for (i = 0; i < SIZE; i++) {
		pos[i] = IntSetA_insert(ia, hoge_int[i], &success[i]);
		assert(IntSetA_verify(ia));
		assert(pos[i]);
		if (i < SIZE/2) {
			assert(success[i]);
			count++;
		} else {
			assert(!success[i]);
		}
	}
/*    IntSetA_print(ia);*/
	assert(!IntSetA_empty(ia));
	assert(IntSetA_size(ia) == count);
	assert(count == SIZE/2);
	/* count, find, lower_bound, upper_bound */
	for (i = 0; i < SIZE/2; i++) {
		assert(IntSetA_count(ia, hoge_int[i]) == 1);
		assert(pos[i] == IntSetA_find(ia, hoge_int[i]));
		assert(pos[i] == IntSetA_lower_bound(ia, hoge_int[i]));
		assert(pos[i] == IntSetA_upper_bound(ia, hoge_int[i]-1));
		assert(IntSetA_lower_bound(ia, hoge_int[i]+1) == IntSetA_upper_bound(ia, hoge_int[i]));
	}
	assert(IntSetA_find(ia, *IntSetAIterator_elem(IntSetA_begin(ia)) -1) == IntSetA_end(ia));
	assert(IntSetA_lower_bound(ia, *IntSetAIterator_elem(IntSetA_rbegin(ia)) +1) == IntSetA_end(ia));
	assert(IntSetA_upper_bound(ia, *IntSetAIterator_elem(IntSetA_rbegin(ia))) == IntSetA_end(ia));
	/* begin, end, next, key */
	for (p = IntSetA_begin(ia), i = 0; p != IntSetA_end(ia); p = IntSetAIterator_next(p), i++) {
		assert(*IntSetAIterator_elem(p) == i);
	}
	assert(i == SIZE/2);
	assert(IntSetAIterator_next(IntSetA_rbegin(ia)) == IntSetA_end(ia));
	/* rbegin, rend, prev, key */
	for (p = IntSetA_rbegin(ia), i = SIZE/2 -1; p != IntSetA_rend(ia); p = IntSetAIterator_prev(p), i--) {
		assert(*IntSetAIterator_elem(p) == i);
	}
	assert(i == -1);
	assert(IntSetAIterator_prev(IntSetA_begin(ia)) == IntSetA_rend(ia));
	/* erase */
	for (i = 0; i < SIZE; i++) {
		if (pos[i] && success[i]) {
			IntSetAIterator itr = IntSetAIterator_next(pos[i]);
			assert(itr == IntSetA_erase(ia, pos[i]));
			assert(IntSetA_verify(ia));
			count--;
		}
	}
	assert(IntSetA_empty(ia));
	assert(IntSetA_size(ia) == 0);
	assert(count == 0);
	/* erase_range */
	for (i = 0; i < SIZE/2; i++) {
		pos[i] = IntSetA_insert(ia, hoge_int[i], NULL);
		assert(pos[i]);
	}
	assert(IntSetA_size(ia) == SIZE/2);
	assert(IntSetA_find(ia, SIZE/2 -2) == IntSetA_erase_range(ia, IntSetA_find(ia, 2), IntSetA_find(ia, SIZE/2 -2)));
	assert(IntSetA_verify(ia));
	assert(IntSetA_size(ia) == 4);
	assert(IntSetA_end(ia) == IntSetA_erase_range(ia, IntSetA_begin(ia), IntSetA_end(ia)));
	assert(IntSetA_size(ia) == 0);
	assert(IntSetA_insert(ia, hoge_int[0], NULL));
	assert(IntSetA_size(ia) == 1);
	assert(IntSetAIterator_next(IntSetA_begin(ia)) == IntSetA_erase_range(ia, IntSetA_begin(ia), IntSetAIterator_next(IntSetA_begin(ia))));
	assert(IntSetA_size(ia) == 0);
	assert(IntSetA_insert(ia, 100, NULL));
	assert(IntSetA_insert(ia, 110, NULL));
	assert(IntSetA_size(ia) == 2);
	assert(IntSetA_upper_bound(ia, 110) == IntSetA_erase_range(ia, IntSetA_lower_bound(ia, 100), IntSetA_upper_bound(ia, 110)));
	assert(IntSetA_size(ia) == 0);
	/* erase_key */
	for (i = 0; i < SIZE/2; i++) {
		pos[i] = IntSetA_insert(ia, hoge_int[i], NULL);
		assert(pos[i]);
	}
	assert(IntSetA_size(ia) == SIZE/2);
	for (i = 0; i < SIZE/2; i++) {
		assert(IntSetA_erase_key(ia, hoge_int[i]) == 1);
		assert(IntSetA_verify(ia));
	}
	assert(IntSetA_size(ia) == 0);
	/* 大量にinsert */
	count = 0;
	while (count < 1000000 && IntSetA_insert(ia, count, NULL)) {
		count++;
	}
	assert(IntSetA_size(ia) == count);
	printf("count: %d\n", count);
	printf("size: %d\n", IntSetA_size(ia));
	/* clear */
	IntSetA_clear(ia);
	printf("size: %d\n", IntSetA_size(ia));
	assert(IntSetA_size(ia) == 0);
	assert(IntSetA_insert(ia, 100, NULL));
	assert(IntSetA_size(ia) == 1);
	IntSetA_clear(ia);
	assert(IntSetA_size(ia) == 0);
	IntSetA_clear(ia);
	assert(IntSetA_size(ia) == 0);
	/* swap */
	for (i = 0; i < SIZE/2; i++) {
		pos[i] = IntSetA_insert(ia, hoge_int[i], &success[i]);
		assert(pos[i]);
		assert(success[i]);
	}
	assert(IntSetA_size(ia) == SIZE/2);
	x = IntSetA_new();
	for (i = 0; i < sizeof b / sizeof b[0]; i++) {
		pos[i] = IntSetA_insert(x, b[i], &success[i]);
		assert(pos[i]);
		assert(success[i]);
	}
	assert(IntSetA_size(x) == sizeof b / sizeof b[0]);
/*    IntSetA_print(ia);*/
/*    IntSetA_print(x);*/

	IntSetA_swap(ia, x);

	assert(IntSetA_verify(ia));
	assert(IntSetA_verify(x));
/*    IntSetA_print(ia);*/
/*    IntSetA_print(x);*/
	assert(IntSetA_size(x) == SIZE/2);
	assert(IntSetA_size(ia) == sizeof b / sizeof b[0]);
	/* insert_range */
	count = IntSetA_size(x);
	IntSetA_insert(x, b[0], 0);
	IntSetA_insert(ia, hoge_int[0], 0);
/*    IntSetA_print(ia);*/
/*    IntSetA_print(x);*/
	assert(IntSetA_insert_range(x, IntSetA_begin(ia), IntSetA_end(ia)));

/*    IntSetA_print(ia);*/
/*    IntSetA_print(x);*/
	assert(IntSetA_size(ia) == sizeof b / sizeof b[0] + 1);
	assert(IntSetA_size(x)  == count + sizeof b / sizeof b[0]);
	assert(IntSetA_verify(ia));
	assert(IntSetA_verify(x));

	assert(IntSetA_insert_range(x, IntSetA_begin(ia), IntSetA_end(ia)));
	assert(IntSetA_size(ia) == sizeof b / sizeof b[0] + 1);
	assert(IntSetA_size(x)  == count + sizeof b / sizeof b[0]);
	assert(IntSetA_verify(ia));
	assert(IntSetA_verify(x));

	POOL_DUMP_OVERFLOW(&pool);
	IntSetA_delete(ia);
	IntSetA_delete(x);
}

void SetTest_test_1_2(void)
{
	int i;
	int success[SIZE];
	size_t count = 0;
	IntSetDIterator pos[SIZE];
	IntSetDIterator p;
	printf("***** test_1_2 *****\n");
	id = IntSetD_new();
	/* 初期状態 */
	assert(IntSetD_empty(id));
	assert(IntSetD_size(id) == 0);
	assert(IntSetD_begin(id) == IntSetD_end(id));
	assert(IntSetD_rbegin(id) == IntSetD_rend(id));
	/* insert */
	for (i = 0; i < SIZE; i++) {
		pos[i] = IntSetD_insert(id, hoge_int[i], &success[i]);
		if (i < SIZE/2) {
			assert(success[i]);
			count++;
		} else {
			assert(!success[i]);
		}
		assert(pos[i]);
	}
/*    IntSetD_print(id);*/
	assert(!IntSetD_empty(id));
	assert(IntSetD_size(id) == count);
	assert(count == SIZE/2);
	/* count, find, lower_bound, upper_bound */
	for (i = 0; i < SIZE/2; i++) {
		assert(IntSetD_count(id, hoge_int[i]) == 1);
		assert(pos[i] == IntSetD_find(id, hoge_int[i]));
		assert(pos[i] == IntSetD_lower_bound(id, hoge_int[i]));
		assert(pos[i] == IntSetD_upper_bound(id, hoge_int[i]+1));
		assert(IntSetD_lower_bound(id, hoge_int[i]-1) == IntSetD_upper_bound(id, hoge_int[i]));
	}
	assert(IntSetD_find(id, *IntSetDIterator_elem(IntSetD_begin(id)) +1) == IntSetD_end(id));
	assert(IntSetD_lower_bound(id, *IntSetDIterator_elem(IntSetD_rbegin(id)) -1) == IntSetD_end(id));
	assert(IntSetD_upper_bound(id, *IntSetDIterator_elem(IntSetD_rbegin(id))) == IntSetD_end(id));
	/* begin, end, next, key */
	for (p = IntSetD_begin(id), i = SIZE/2 -1; p != IntSetD_end(id); p = IntSetDIterator_next(p), i--) {
		assert(*IntSetDIterator_elem(p) == i);
	}
	assert(i == -1);
	assert(IntSetDIterator_next(IntSetD_rbegin(id)) == IntSetD_end(id));
	/* rbegin, rend, prev, key */
	for (p = IntSetD_rbegin(id), i = 0; p != IntSetD_rend(id); p = IntSetDIterator_prev(p), i++) {
		assert(*IntSetDIterator_elem(p) == i);
	}
	assert(i == SIZE/2);
	assert(IntSetDIterator_prev(IntSetD_begin(id)) == IntSetD_rend(id));
	/* erase */
	for (i = 0; i < SIZE; i++) {
		if (pos[i] && success[i]) {
			IntSetDIterator itr = IntSetDIterator_next(pos[i]);
			assert(itr == IntSetD_erase(id, pos[i]));
			count--;
		}
	}
	assert(IntSetD_empty(id));
	assert(IntSetD_size(id) == 0);
	assert(count == 0);
	/* erase_range */
	for (i = 0; i < SIZE/2; i++) {
		pos[i] = IntSetD_insert(id, hoge_int[i], NULL);
		assert(pos[i]);
	}
	assert(IntSetD_size(id) == SIZE/2);
	assert(IntSetD_find(id, 1) == IntSetD_erase_range(id, IntSetD_find(id, SIZE/2-1 -2), IntSetD_find(id, 1)));
	assert(IntSetD_size(id) == 4);
	assert(IntSetD_end(id) == IntSetD_erase_range(id, IntSetD_begin(id), IntSetD_end(id)));
	assert(IntSetD_size(id) == 0);
	assert(IntSetD_insert(id, hoge_int[0], NULL));
	assert(IntSetD_size(id) == 1);
	assert(IntSetDIterator_next(IntSetD_begin(id)) == IntSetD_erase_range(id, IntSetD_begin(id), IntSetDIterator_next(IntSetD_begin(id))));
	assert(IntSetD_size(id) == 0);
	assert(IntSetD_insert(id, 100, NULL));
	assert(IntSetD_insert(id, 110, NULL));
	assert(IntSetD_size(id) == 2);
	assert(IntSetD_upper_bound(id, 100) == IntSetD_erase_range(id, IntSetD_lower_bound(id, 110), IntSetD_upper_bound(id, 100)));
	assert(IntSetD_size(id) == 0);
	/* erase_key */
	for (i = 0; i < SIZE/2; i++) {
		pos[i] = IntSetD_insert(id, hoge_int[i], NULL);
		assert(pos[i]);
	}
	assert(IntSetD_size(id) == SIZE/2);
	for (i = 0; i < SIZE/2; i++) {
		assert(IntSetD_erase_key(id, hoge_int[i]) == 1);
	}
	assert(IntSetD_size(id) == 0);
	/* 大量にinsert */
	count = 0;
	while (count < 1000000 && IntSetD_insert(id, count, NULL)) {
		count++;
	}
	assert(IntSetD_size(id) == count);
	printf("count: %d\n", count);

	POOL_DUMP_OVERFLOW(&pool);
	IntSetD_delete(id);
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
	printf("***** test_1_3 *****\n");
	ima = IntMSetA_new();
	/* 初期状態 */
	assert(IntMSetA_empty(ima));
	assert(IntMSetA_size(ima) == 0);
	assert(IntMSetA_begin(ima) == IntMSetA_end(ima));
	assert(IntMSetA_rbegin(ima) == IntMSetA_rend(ima));
	/* insert */
	for (i = 0; i < SIZE; i++) {
		pos[i] = IntMSetA_insert(ima, hoge_int[i]);
		assert(pos[i] && pos[i] != IntMSetA_end(ima));
		count++;
	}
/*    IntMSetA_print(ima);*/
	assert(!IntMSetA_empty(ima));
	assert(IntMSetA_size(ima) == count);
	assert(count == SIZE);
	/* count */
	for (i = 0; i < SIZE; i++) {
		if (i < SIZE/2/2) {
			assert(IntMSetA_count(ima, i) == 1);
		} else if (i < SIZE/2) {
			assert(IntMSetA_count(ima, i) == 3);
		} else {
			assert(IntMSetA_count(ima, i) == 0);
		}
	}
	/* find, lower_bound, upper_bound */
	for (i = 0; i < SIZE; i++) {
		if (IntMSetA_count(ima, hoge_int[i]) == 1) {
			assert(pos[i] == IntMSetA_find(ima, hoge_int[i]));
			assert(pos[i] == IntMSetA_lower_bound(ima, hoge_int[i]));
			assert(pos[i] == IntMSetA_upper_bound(ima, hoge_int[i]-1));
		} else if (IntMSetA_count(ima, hoge_int[i]) == 3) {
			if (!flag[hoge_int[i]]) {
				flag[hoge_int[i]] = 1;
				assert(pos[i] == IntMSetA_lower_bound(ima, hoge_int[i]));
				assert(pos[i] == IntMSetA_upper_bound(ima, hoge_int[i]-1));
			}
		} else {
			assert(0);
		}
		assert(IntMSetA_lower_bound(ima, hoge_int[i]+1) == IntMSetA_upper_bound(ima, hoge_int[i]));
	}
	assert(IntMSetA_find(ima, *IntMSetAIterator_elem(IntMSetA_begin(ima)) -1) == IntMSetA_end(ima));
	assert(IntMSetA_lower_bound(ima, *IntMSetAIterator_elem(IntMSetA_rbegin(ima)) +1) == IntMSetA_end(ima));
	assert(IntMSetA_upper_bound(ima, *IntMSetAIterator_elem(IntMSetA_rbegin(ima))) == IntMSetA_end(ima));
	/* begin, end, next, key */
	for (p = IntMSetA_begin(ima), i = 0; p != IntMSetA_end(ima); p = IntMSetAIterator_next(p), i++) {
/*        printf("%d, %d\n", i, *IntMSetAIterator_elem(p));*/
	}
	assert(i == SIZE);
	assert(IntMSetAIterator_next(IntMSetA_rbegin(ima)) == IntMSetA_end(ima));
	/* rbegin, rend, prev, key */
	for (p = IntMSetA_rbegin(ima), i = SIZE -1; p != IntMSetA_rend(ima); p = IntMSetAIterator_prev(p), i--) {
/*        printf("%d, %d\n", i, *IntMSetAIterator_elem(p));*/
	}
	assert(i == -1);
	assert(IntMSetAIterator_prev(IntMSetA_begin(ima)) == IntMSetA_rend(ima));
	/* erase */
	for (i = 0; i < SIZE; i++) {
		IntMSetAIterator itr = IntMSetAIterator_next(pos[i]);
		assert(itr == IntMSetA_erase(ima, pos[i]));
		count--;
	}
	assert(IntMSetA_empty(ima));
	assert(IntMSetA_size(ima) == 0);
	assert(count == 0);
	/* erase_range */
	for (i = 0; i < SIZE; i++) {
		pos[i] = IntMSetA_insert(ima, hoge_int[i]);
		assert(pos[i] && pos[i] != IntMSetA_end(ima));
	}
	assert(IntMSetA_size(ima) == SIZE);
	assert(IntMSetAIterator_next(IntMSetA_find(ima, SIZE/2/2 -1)) == IntMSetA_erase_range(ima, IntMSetA_find(ima, 0), IntMSetAIterator_next(IntMSetA_find(ima, SIZE/2/2 -1))));
	assert(IntMSetA_size(ima) == SIZE - SIZE/2/2);
	assert(IntMSetA_end(ima) == IntMSetA_erase_range(ima, IntMSetA_begin(ima), IntMSetA_end(ima)));
	assert(IntMSetA_size(ima) == 0);
	assert(IntMSetA_insert(ima, hoge_int[0]));
	assert(IntMSetA_size(ima) == 1);
	assert(IntMSetAIterator_next(IntMSetA_begin(ima)) == IntMSetA_erase_range(ima, IntMSetA_begin(ima), IntMSetAIterator_next(IntMSetA_begin(ima))));
	assert(IntMSetA_size(ima) == 0);
	assert(IntMSetA_insert(ima, 100));
	assert(IntMSetA_insert(ima, 100));
	assert(IntMSetA_insert(ima, 100));
	assert(IntMSetA_insert(ima, 110));
	assert(IntMSetA_insert(ima, 110));
	assert(IntMSetA_size(ima) == 5);
	assert(IntMSetA_upper_bound(ima, 110) == IntMSetA_erase_range(ima, IntMSetA_lower_bound(ima, 100), IntMSetA_upper_bound(ima, 110)));
	assert(IntMSetA_size(ima) == 0);
	/* erase_key */
	for (i = 0; i < SIZE; i++) {
		pos[i] = IntMSetA_insert(ima, hoge_int[i]);
		assert(pos[i] && pos[i] != IntMSetA_end(ima));
	}
	assert(IntMSetA_size(ima) == SIZE);
	for (i = 0; i < SIZE/2/2; i++) {
		assert(IntMSetA_erase_key(ima, i) == 1);
	}
	assert(IntMSetA_size(ima) == SIZE - SIZE/2/2);
	for (i = SIZE/2/2; i < SIZE/2; i++) {
		assert(IntMSetA_erase_key(ima, i) == 3);
	}
	assert(IntMSetA_size(ima) == 0);
	/* 大量にinsert */
	count = 0;
	while (count < 1000000 && IntMSetA_insert(ima, 0)) {
		count++;
	}
	assert(IntMSetA_size(ima) == count);
	printf("count: %d\n", count);

	IntMSetA_clear(ima);

	/* insert_range */
	for (i = 0; i < SIZE/2; i++) {
		pos[i] = IntMSetA_insert(ima, hoge_int[i]);
		assert(pos[i]);
	}
	assert(IntMSetA_size(ima) == SIZE/2);
	x = IntMSetA_new();
	for (i = 0; i < sizeof b / sizeof b[0]; i++) {
		pos[i] = IntMSetA_insert(x, b[i]);
		assert(pos[i]);
	}
	for (i = 0; i < SIZE/2; i++) {
		pos[i] = IntMSetA_insert(x, hoge_int[i]);
		assert(pos[i]);
	}
	assert(IntMSetA_size(x) == sizeof b / sizeof b[0] + SIZE/2);
/*    IntMSetA_print(ima);*/
/*    IntMSetA_print(x);*/
	assert(IntMSetA_insert_range(x, IntMSetA_begin(ima), IntMSetA_end(ima)));
	assert(IntMSetA_verify(ima));
	assert(IntMSetA_verify(x));
/*    IntMSetA_print(ima);*/
/*    IntMSetA_print(x);*/
	assert(IntMSetA_size(x) == sizeof b / sizeof b[0] + SIZE);


	POOL_DUMP_OVERFLOW(&pool);
	IntMSetA_delete(ima);
	IntMSetA_delete(x);
}

void SetTest_test_2_1(void)
{
	int i;
	int success[SIZE];
	size_t count = 0;
	DoubleSetAIterator pos[SIZE];
	DoubleSetAIterator p;
	printf("***** test_2_1 *****\n");
	da = DoubleSetA_new();
	/* 初期状態 */
	assert(DoubleSetA_empty(da));
	assert(DoubleSetA_size(da) == 0);
	assert(DoubleSetA_begin(da) == DoubleSetA_end(da));
	assert(DoubleSetA_rbegin(da) == DoubleSetA_rend(da));
	/* insert */
	for (i = 0; i < SIZE; i++) {
		pos[i] = DoubleSetA_insert(da, hoge_double[i], &success[i]);
		if (i < SIZE/2) {
			assert(success[i]);
			count++;
		} else {
			assert(!success[i]);
		}
		assert(pos[i]);
	}
	assert(!DoubleSetA_empty(da));
	assert(DoubleSetA_size(da) == count);
	assert(count == SIZE/2);
	/* count, find, lower_bound, upper_bound */
	for (i = 0; i < SIZE/2; i++) {
		assert(DoubleSetA_count(da, hoge_double[i]) == 1);
		assert(pos[i] == DoubleSetA_find(da, hoge_double[i]));
		assert(pos[i] == DoubleSetA_lower_bound(da, hoge_double[i]));
		assert(pos[i] == DoubleSetA_upper_bound(da, hoge_double[i]-0.5));
		assert(DoubleSetA_lower_bound(da, hoge_double[i]+0.5) == DoubleSetA_upper_bound(da, hoge_double[i]));
	}
	assert(DoubleSetA_find(da, *DoubleSetAIterator_elem(DoubleSetA_begin(da)) -1) == DoubleSetA_end(da));
	assert(DoubleSetA_lower_bound(da, *DoubleSetAIterator_elem(DoubleSetA_rbegin(da)) +1) == DoubleSetA_end(da));
	assert(DoubleSetA_upper_bound(da, *DoubleSetAIterator_elem(DoubleSetA_rbegin(da))) == DoubleSetA_end(da));
	/* begin, end, next, key */
	for (p = DoubleSetA_begin(da), i = 0; p != DoubleSetA_end(da); p = DoubleSetAIterator_next(p), i++) {
		assert(*DoubleSetAIterator_elem(p) == i*0.5);
	}
	assert(i == SIZE/2);
	assert(DoubleSetAIterator_next(DoubleSetA_rbegin(da)) == DoubleSetA_end(da));
	/* rbegin, rend, prev, key */
	for (p = DoubleSetA_rbegin(da), i = SIZE/2 -1; p != DoubleSetA_rend(da); p = DoubleSetAIterator_prev(p), i--) {
		assert(*DoubleSetAIterator_elem(p) == i*0.5);
	}
	assert(i == -1);
	assert(DoubleSetAIterator_prev(DoubleSetA_begin(da)) == DoubleSetA_rend(da));
	/* erase */
	for (i = 0; i < SIZE; i++) {
		if (pos[i] && success[i]) {
			DoubleSetAIterator itr = DoubleSetAIterator_next(pos[i]);
			assert(itr == DoubleSetA_erase(da, pos[i]));
			count--;
		}
	}
	assert(DoubleSetA_empty(da));
	assert(DoubleSetA_size(da) == 0);
	assert(count == 0);
	/* erase_range */
	for (i = 0; i < SIZE/2; i++) {
		pos[i] = DoubleSetA_insert(da, hoge_double[i], NULL);
		assert(pos[i]);
	}
	assert(DoubleSetA_size(da) == SIZE/2);
	assert(DoubleSetA_find(da, SIZE/2 -2) == DoubleSetA_erase_range(da, DoubleSetA_find(da, 2), DoubleSetA_find(da, SIZE/2 -2)));
	assert(DoubleSetA_size(da) == 4);
	assert(DoubleSetA_end(da) == DoubleSetA_erase_range(da, DoubleSetA_begin(da), DoubleSetA_end(da)));
	assert(DoubleSetA_size(da) == 0);
	assert(DoubleSetA_insert(da, hoge_double[0], NULL));
	assert(DoubleSetA_size(da) == 1);
	assert(DoubleSetAIterator_next(DoubleSetA_begin(da)) == DoubleSetA_erase_range(da, DoubleSetA_begin(da), DoubleSetAIterator_next(DoubleSetA_begin(da))));
	assert(DoubleSetA_size(da) == 0);
	assert(DoubleSetA_insert(da, 100.1, NULL));
	assert(DoubleSetA_insert(da, 110.1, NULL));
	assert(DoubleSetA_size(da) == 2);
	assert(DoubleSetA_upper_bound(da, 110.1) == DoubleSetA_erase_range(da, DoubleSetA_lower_bound(da, 100.1), DoubleSetA_upper_bound(da, 110.1)));
	assert(DoubleSetA_size(da) == 0);
	/* erase_key */
	for (i = 0; i < SIZE/2; i++) {
		pos[i] = DoubleSetA_insert(da, hoge_double[i], NULL);
		assert(pos[i]);
	}
	assert(DoubleSetA_size(da) == SIZE/2);
	for (i = 0; i < SIZE/2; i++) {
		assert(DoubleSetA_erase_key(da, hoge_double[i]) == 1);
	}
	assert(DoubleSetA_size(da) == 0);
	/* 大量にinsert */
	count = 0;
	while (count < 1000000 && DoubleSetA_insert(da, count, NULL)) {
		count++;
	}
	assert(DoubleSetA_size(da) == count);
	printf("count: %d\n", count);
	printf("size: %d\n", DoubleSetA_size(da));
	/* clear */
	DoubleSetA_clear(da);
	printf("size: %d\n", DoubleSetA_size(da));
	assert(DoubleSetA_size(da) == 0);
	assert(DoubleSetA_insert(da, 100, NULL));
	assert(DoubleSetA_size(da) == 1);
	DoubleSetA_clear(da);
	assert(DoubleSetA_size(da) == 0);
	DoubleSetA_clear(da);
	assert(DoubleSetA_size(da) == 0);

	POOL_DUMP_OVERFLOW(&pool);
	DoubleSetA_delete(da);
}

void SetTest_test_3_1(void)
{
	int i;
	int success[SIZE];
	size_t count = 0;
	int *intp = NULL;
	PtrSetAIterator pos[SIZE];
	PtrSetAIterator p;
	printf("***** test_3_1 *****\n");
	pa = PtrSetA_new();
	/* 初期状態 */
	assert(PtrSetA_empty(pa));
	assert(PtrSetA_size(pa) == 0);
	assert(PtrSetA_begin(pa) == PtrSetA_end(pa));
	assert(PtrSetA_rbegin(pa) == PtrSetA_rend(pa));
	/* insert */
	for (i = 0; i < SIZE; i++) {
		pos[i] = PtrSetA_insert(pa, hoge_ptr[i], &success[i]);
		if (i < SIZE/2) {
			assert(success[i]);
			count++;
		} else {
			assert(!success[i]);
		}
		assert(pos[i]);
	}
	assert(!PtrSetA_empty(pa));
	assert(PtrSetA_size(pa) == count);
	assert(count == SIZE/2);
	/* count, find, lower_bound, upper_bound */
	for (i = 0; i < SIZE/2; i++) {
		assert(PtrSetA_count(pa, hoge_ptr[i]) == 1);
		assert(pos[i] == PtrSetA_find(pa, hoge_ptr[i]));
		assert(pos[i] == PtrSetA_lower_bound(pa, hoge_ptr[i]));
		assert(pos[i] == PtrSetA_upper_bound(pa, hoge_ptr[i]-1));
		assert(PtrSetA_lower_bound(pa, hoge_ptr[i]+1) == PtrSetA_upper_bound(pa, hoge_ptr[i]));
	}
	assert(PtrSetA_find(pa, *PtrSetAIterator_elem(PtrSetA_begin(pa)) -1) == PtrSetA_end(pa));
	assert(PtrSetA_lower_bound(pa, *PtrSetAIterator_elem(PtrSetA_rbegin(pa)) +1) == PtrSetA_end(pa));
	assert(PtrSetA_upper_bound(pa, *PtrSetAIterator_elem(PtrSetA_rbegin(pa))) == PtrSetA_end(pa));
	/* begin, end, next, key */
	for (p = PtrSetA_begin(pa), i = 0; p != PtrSetA_end(pa); p = PtrSetAIterator_next(p), i++) {
		assert(*PtrSetAIterator_elem(p) == &hoge_int[i]);
	}
	assert(i == SIZE/2);
	assert(PtrSetAIterator_next(PtrSetA_rbegin(pa)) == PtrSetA_end(pa));
	/* rbegin, rend, prev, key */
	for (p = PtrSetA_rbegin(pa), i = SIZE/2 -1; p != PtrSetA_rend(pa); p = PtrSetAIterator_prev(p), i--) {
		assert(*PtrSetAIterator_elem(p) == &hoge_int[i]);
	}
	assert(i == -1);
	assert(PtrSetAIterator_prev(PtrSetA_begin(pa)) == PtrSetA_rend(pa));
	/* erase */
	for (i = 0; i < SIZE; i++) {
		if (pos[i] && success[i]) {
			PtrSetAIterator itr = PtrSetAIterator_next(pos[i]);
			assert(itr == PtrSetA_erase(pa, pos[i]));
			count--;
		}
	}
	assert(PtrSetA_empty(pa));
	assert(PtrSetA_size(pa) == 0);
	assert(count == 0);
	/* erase_range */
	for (i = 0; i < SIZE/2; i++) {
		pos[i] = PtrSetA_insert(pa, hoge_ptr[i], NULL);
		assert(pos[i]);
	}
	assert(PtrSetA_size(pa) == SIZE/2);
	assert(PtrSetA_find(pa, &hoge_int[SIZE/2 -2]) == PtrSetA_erase_range(pa, PtrSetA_find(pa, &hoge_int[2]), PtrSetA_find(pa, &hoge_int[SIZE/2 -2])));
	assert(PtrSetA_size(pa) == 4);
	assert(PtrSetA_end(pa) == PtrSetA_erase_range(pa, PtrSetA_begin(pa), PtrSetA_end(pa)));
	assert(PtrSetA_size(pa) == 0);
	assert(PtrSetA_insert(pa, hoge_ptr[0], NULL));
	assert(PtrSetA_size(pa) == 1);
	assert(PtrSetAIterator_next(PtrSetA_begin(pa)) == PtrSetA_erase_range(pa, PtrSetA_begin(pa), PtrSetAIterator_next(PtrSetA_begin(pa))));
	assert(PtrSetA_size(pa) == 0);
	assert(PtrSetA_insert(pa, (int*)100, NULL));
	assert(PtrSetA_insert(pa, (int*)100 + 1, NULL));
	assert(PtrSetA_size(pa) == 2);
	assert(PtrSetA_upper_bound(pa, (int*)100 + 1) == PtrSetA_erase_range(pa, PtrSetA_lower_bound(pa, (int*)100), PtrSetA_upper_bound(pa, (int*)100 + 1)));
	assert(PtrSetA_size(pa) == 0);
	/* erase_key */
	for (i = 0; i < SIZE/2; i++) {
		pos[i] = PtrSetA_insert(pa, hoge_ptr[i], NULL);
		assert(pos[i]);
	}
	assert(PtrSetA_size(pa) == SIZE/2);
	for (i = 0; i < SIZE/2; i++) {
		assert(PtrSetA_erase_key(pa, hoge_ptr[i]) == 1);
	}
	assert(PtrSetA_size(pa) == 0);
	/* 大量にinsert */
	count = 0;
	while (count < 1000000 && PtrSetA_insert(pa, intp, NULL)) {
		count++;
		intp++;
	}
	assert(PtrSetA_size(pa) == count);
	printf("count: %d\n", count);
	printf("size: %d\n", PtrSetA_size(pa));
	/* clear */
	PtrSetA_clear(pa);
	printf("size: %d\n", PtrSetA_size(pa));
	assert(PtrSetA_size(pa) == 0);
	assert(PtrSetA_insert(pa, intp, NULL));
	assert(PtrSetA_size(pa) == 1);
	PtrSetA_clear(pa);
	assert(PtrSetA_size(pa) == 0);
	PtrSetA_clear(pa);
	assert(PtrSetA_size(pa) == 0);

	POOL_DUMP_OVERFLOW(&pool);
	PtrSetA_delete(pa);
}

void SetTest_test_4_1(void)
{
	int i;
	int success[SIZE];
	size_t count = 0;
	StrSetAIterator pos[SIZE];
	StrSetAIterator p;
	printf("***** test_4_1 *****\n");
	sa = StrSetA_new();
	/* 初期状態 */
	assert(StrSetA_empty(sa));
	assert(StrSetA_size(sa) == 0);
	assert(StrSetA_begin(sa) == StrSetA_end(sa));
	assert(StrSetA_rbegin(sa) == StrSetA_rend(sa));
	/* insert */
	for (i = 0; i < SIZE; i++) {
		pos[i] = StrSetA_insert(sa, hoge_str[i], &success[i]);
		if (i < SIZE/2) {
			assert(success[i]);
			count++;
		} else {
			assert(!success[i]);
		}
		assert(pos[i]);
	}
/*    StrSetA_print(sa);*/
	assert(!StrSetA_empty(sa));
	assert(StrSetA_size(sa) == count);
	assert(count == SIZE/2);
	/* count, find, lower_bound, upper_bound */
	for (i = 0; i < SIZE/2; i++) {
		assert(StrSetA_count(sa, hoge_str[i]) == 1);
		assert(pos[i] == StrSetA_find(sa, hoge_str[i]));
		assert(pos[i] == StrSetA_lower_bound(sa, hoge_str[i]));
		if (i != SIZE/2-1) {
			assert(pos[i+1] == StrSetA_upper_bound(sa, hoge_str[i]));
			assert(StrSetA_lower_bound(sa, hoge_str[i+1]) == StrSetA_upper_bound(sa, hoge_str[i]));
		}
	}
	assert(StrSetA_find(sa, "hoge") == StrSetA_end(sa));
	assert(StrSetA_lower_bound(sa, "hoge") == StrSetA_end(sa));
	assert(StrSetA_upper_bound(sa, *StrSetAIterator_elem(StrSetA_rbegin(sa))) == StrSetA_end(sa));
	/* begin, end, next, key */
	for (p = StrSetA_begin(sa), i = 0; p != StrSetA_end(sa); p = StrSetAIterator_next(p), i++) {
		assert(*StrSetAIterator_elem(p) == hoge_str[i]);
	}
	assert(i == SIZE/2);
	assert(StrSetAIterator_next(StrSetA_rbegin(sa)) == StrSetA_end(sa));
	/* rbegin, rend, prev, key */
	for (p = StrSetA_rbegin(sa), i = SIZE/2 -1; p != StrSetA_rend(sa); p = StrSetAIterator_prev(p), i--) {
		assert(*StrSetAIterator_elem(p) == hoge_str[i]);
	}
	assert(i == -1);
	assert(StrSetAIterator_prev(StrSetA_begin(sa)) == StrSetA_rend(sa));
	/* erase */
	for (i = 0; i < SIZE; i++) {
		if (pos[i] && success[i]) {
			StrSetAIterator itr = StrSetAIterator_next(pos[i]);
			assert(itr == StrSetA_erase(sa, pos[i]));
			count--;
		}
	}
	assert(StrSetA_empty(sa));
	assert(StrSetA_size(sa) == 0);
	assert(count == 0);
	/* erase_range */
	for (i = 0; i < SIZE/2; i++) {
		pos[i] = StrSetA_insert(sa, hoge_str[i], NULL);
		assert(pos[i]);
	}
	assert(StrSetA_size(sa) == SIZE/2);
	assert(StrSetA_find(sa, hoge_str[SIZE/2 -2]) == StrSetA_erase_range(sa, StrSetA_find(sa, hoge_str[2]), StrSetA_find(sa, hoge_str[SIZE/2 -2])));
	assert(StrSetA_size(sa) == 4);
	assert(StrSetA_end(sa) == StrSetA_erase_range(sa, StrSetA_begin(sa), StrSetA_end(sa)));
	assert(StrSetA_size(sa) == 0);
	assert(StrSetA_insert(sa, hoge_str[0], NULL));
	assert(StrSetA_size(sa) == 1);
	assert(StrSetAIterator_next(StrSetA_begin(sa)) == StrSetA_erase_range(sa, StrSetA_begin(sa), StrSetAIterator_next(StrSetA_begin(sa))));
	assert(StrSetA_size(sa) == 0);
	assert(StrSetA_insert(sa, "100", NULL));
	assert(StrSetA_insert(sa, "110", NULL));
	assert(StrSetA_size(sa) == 2);
	assert(StrSetA_upper_bound(sa, "110") == StrSetA_erase_range(sa, StrSetA_lower_bound(sa, "100"), StrSetA_upper_bound(sa, "110")));
	assert(StrSetA_size(sa) == 0);
	/* erase_key */
	for (i = 0; i < SIZE/2; i++) {
		pos[i] = StrSetA_insert(sa, hoge_str[i], NULL);
		assert(pos[i]);
	}
	assert(StrSetA_size(sa) == SIZE/2);
	for (i = 0; i < SIZE/2; i++) {
		assert(StrSetA_erase_key(sa, hoge_str[i]) == 1);
	}
	assert(StrSetA_size(sa) == 0);
	/* clear */
	assert(StrSetA_insert(sa, "103", NULL));
	assert(StrSetA_insert(sa, "101", NULL));
	assert(StrSetA_insert(sa, "100", NULL));
	assert(StrSetA_insert(sa, "104", NULL));
	assert(StrSetA_insert(sa, "106", NULL));
	assert(StrSetA_insert(sa, "105", NULL));
	assert(StrSetA_insert(sa, "102", NULL));
	printf("size: %d\n", StrSetA_size(sa));
	StrSetA_clear(sa);
	printf("size: %d\n", StrSetA_size(sa));
	assert(StrSetA_size(sa) == 0);
	assert(StrSetA_insert(sa, "100", NULL));
	assert(StrSetA_size(sa) == 1);
	StrSetA_clear(sa);
	assert(StrSetA_size(sa) == 0);
	StrSetA_clear(sa);
	assert(StrSetA_size(sa) == 0);

	POOL_DUMP_OVERFLOW(&pool);
	StrSetA_delete(sa);
}

void SetTest_test_5_1(void)
{
	int i;
	size_t count = 0;
	int success[SIZE];
	UIntSetAIterator pos[SIZE];
	UIntSetAIterator p;
	printf("***** test_5_1 *****\n");
	uia = UIntSetA_new();
	/* 初期状態 */
	assert(UIntSetA_empty(uia));
	assert(UIntSetA_size(uia) == 0);
	assert(UIntSetA_begin(uia) == UIntSetA_end(uia));
	assert(UIntSetA_rbegin(uia) == UIntSetA_rend(uia));
	/* insert */
	for (i = 0; i < SIZE; i++) {
		pos[i] = UIntSetA_insert(uia, hoge_int[i], &success[i]);
/*        UIntSetA_print(uia);*/
		assert(pos[i]);
		if (i < SIZE/2) {
			assert(success[i]);
			count++;
		} else {
			assert(!success[i]);
		}
	}
/*    UIntSetA_print(uia);*/
	assert(!UIntSetA_empty(uia));
	assert(UIntSetA_size(uia) == count);
	assert(count == SIZE/2);
	/* count, find, lower_bound, upper_bound */
	for (i = 0; i < SIZE/2; i++) {
		assert(UIntSetA_count(uia, hoge_int[i]) == 1);
		assert(pos[i] == UIntSetA_find(uia, hoge_int[i]));
		assert(pos[i] == UIntSetA_lower_bound(uia, hoge_int[i]));
		if (hoge_int[i] == 0) {
			assert(UIntSetA_end(uia) == UIntSetA_upper_bound(uia, hoge_int[i]-1));
		} else {
			assert(pos[i] == UIntSetA_upper_bound(uia, hoge_int[i]-1));
		}
		assert(UIntSetA_lower_bound(uia, hoge_int[i]+1) == UIntSetA_upper_bound(uia, hoge_int[i]));
	}
	assert(UIntSetA_find(uia, *UIntSetAIterator_elem(UIntSetA_begin(uia)) -1) == UIntSetA_end(uia));
	assert(UIntSetA_lower_bound(uia, *UIntSetAIterator_elem(UIntSetA_rbegin(uia)) +1) == UIntSetA_end(uia));
	assert(UIntSetA_upper_bound(uia, *UIntSetAIterator_elem(UIntSetA_rbegin(uia))) == UIntSetA_end(uia));
	/* begin, end, next, key */
	for (p = UIntSetA_begin(uia), i = 0; p != UIntSetA_end(uia); p = UIntSetAIterator_next(p), i++) {
		assert(*UIntSetAIterator_elem(p) == i);
	}
	assert(i == SIZE/2);
	assert(UIntSetAIterator_next(UIntSetA_rbegin(uia)) == UIntSetA_end(uia));
	/* rbegin, rend, prev, key */
	for (p = UIntSetA_rbegin(uia), i = SIZE/2 -1; p != UIntSetA_rend(uia); p = UIntSetAIterator_prev(p), i--) {
		assert(*UIntSetAIterator_elem(p) == i);
	}
	assert(i == -1);
	assert(UIntSetAIterator_prev(UIntSetA_begin(uia)) == UIntSetA_rend(uia));
	/* erase */
	for (i = 0; i < SIZE; i++) {
		if (pos[i] && success[i]) {
			UIntSetAIterator itr = UIntSetAIterator_next(pos[i]);
			assert(itr == UIntSetA_erase(uia, pos[i]));
			count--;
		}
	}
	assert(UIntSetA_empty(uia));
	assert(UIntSetA_size(uia) == 0);
	assert(count == 0);
	/* erase_range */
	for (i = 0; i < SIZE/2; i++) {
		pos[i] = UIntSetA_insert(uia, hoge_int[i], NULL);
		assert(pos[i]);
	}
	assert(UIntSetA_size(uia) == SIZE/2);
	assert(UIntSetA_find(uia, SIZE/2 -2) == UIntSetA_erase_range(uia, UIntSetA_find(uia, 2), UIntSetA_find(uia, SIZE/2 -2)));
	assert(UIntSetA_size(uia) == 4);
	assert(UIntSetA_end(uia) == UIntSetA_erase_range(uia, UIntSetA_begin(uia), UIntSetA_end(uia)));
	assert(UIntSetA_size(uia) == 0);
	assert(UIntSetA_insert(uia, hoge_int[0], NULL));
	assert(UIntSetA_size(uia) == 1);
	assert(UIntSetAIterator_next(UIntSetA_begin(uia)) == UIntSetA_erase_range(uia, UIntSetA_begin(uia), UIntSetAIterator_next(UIntSetA_begin(uia))));
	assert(UIntSetA_size(uia) == 0);
	assert(UIntSetA_insert(uia, 100, NULL));
	assert(UIntSetA_insert(uia, 110, NULL));
	assert(UIntSetA_size(uia) == 2);
	assert(UIntSetA_upper_bound(uia, 110) == UIntSetA_erase_range(uia, UIntSetA_lower_bound(uia, 100), UIntSetA_upper_bound(uia, 110)));
	assert(UIntSetA_size(uia) == 0);
	/* erase_key */
	for (i = 0; i < SIZE/2; i++) {
		pos[i] = UIntSetA_insert(uia, hoge_int[i], NULL);
		assert(pos[i]);
	}
	assert(UIntSetA_size(uia) == SIZE/2);
	for (i = 0; i < SIZE/2; i++) {
		assert(UIntSetA_erase_key(uia, hoge_int[i]) == 1);
	}
	assert(UIntSetA_size(uia) == 0);
	/* 大量にinsert */
	count = 0;
	while (count < 1000000 && UIntSetA_insert(uia, count, NULL)) {
		count++;
	}
	assert(UIntSetA_size(uia) == count);
	printf("count: %d\n", count);
	printf("size: %d\n", UIntSetA_size(uia));
	/* clear */
	UIntSetA_clear(uia);
	printf("size: %d\n", UIntSetA_size(uia));
	assert(UIntSetA_size(uia) == 0);
	assert(UIntSetA_insert(uia, 100, NULL));
	assert(UIntSetA_size(uia) == 1);
	UIntSetA_clear(uia);
	assert(UIntSetA_size(uia) == 0);
	UIntSetA_clear(uia);
	assert(UIntSetA_size(uia) == 0);

	POOL_DUMP_OVERFLOW(&pool);
	UIntSetA_delete(uia);
}




void SetTest_run(void)
{
	printf("\n===== set test =====\n");
	set_init_hoge();

	SetTest_test_1_1();
	SetTest_test_1_2();
	SetTest_test_1_3();
	SetTest_test_2_1();
	SetTest_test_3_1();
	SetTest_test_4_1();
	SetTest_test_5_1();
}


int main(void)
{
#ifdef MY_MALLOC
	Pool_init(&pool, buf, sizeof buf, sizeof buf[0]);
#endif
	SetTest_run();
#ifdef MY_MALLOC
	POOL_DUMP_LEAK(&pool, 0);
#endif
	return 0;
}
