#include <stdio.h>
#include <string.h>
#include "../unittest/UnitTest.h"
#include <time.h>
#include "../cstl/ring.h"
#include "../cstl/map.h"
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


/* map */
#ifdef CSTLGEN
#include "IntIntMapA.h"
#include "IntIntMMapA.h"
#else

CSTL_MAP_INTERFACE(IntIntMapA, int, int)
CSTL_MAP_DEBUG_INTERFACE(IntIntMapA)

CSTL_MULTIMAP_INTERFACE(IntIntMMapA, int, int)
CSTL_MAP_DEBUG_INTERFACE(IntIntMMapA)

/* int */
CSTL_MAP_IMPLEMENT(IntIntMapA, int, int, CSTL_LESS)
CSTL_MAP_DEBUG_IMPLEMENT(IntIntMapA, int, int, CSTL_LESS, %d, %d, VISUAL)

CSTL_MULTIMAP_IMPLEMENT(IntIntMMapA, int, int, CSTL_LESS)
CSTL_MAP_DEBUG_IMPLEMENT(IntIntMMapA, int, int, CSTL_LESS, %d, %d, VISUAL)
#endif
static IntIntMapA *ia;
static IntIntMMapA *ima;



#define SIZE	32
static int hoge_int[SIZE];
static double hoge_double[SIZE];
static int *hoge_ptr[SIZE];
static char *hoge_str[SIZE];
static char str[SIZE][16];

void map_init_hoge(void)
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

void MapTest_test_1_1(void)
{
	int i;
	int success[SIZE];
	size_t count = 0;
	IntIntMapAIterator pos[SIZE];
	IntIntMapAIterator p;
	IntIntMapA *x;
	ia = IntIntMapA_new();
	/* 初期状態 */
	ASSERT(cstl_empty(ia));
	ASSERT(cstl_size(ia) == 0);
	ASSERT(cstl_iter_eq(cstl_begin(ia), cstl_end(ia)));
	ASSERT(cstl_iter_eq(cstl_rbegin(ia), cstl_rend(ia)));
	/* insert */
	for (i = 0; i < SIZE; i++) {
/*        pos[i] = cstl_map_insert(ia, hoge_int[i], hoge_int[i], &success[i]);*/
		ASSERT(cstl_map_insert(ia, hoge_int[i], hoge_int[i], &pos[i], &success[i]));
		if (i < SIZE/2) {
			ASSERT(success[i]);
			count++;
		} else {
			ASSERT(!success[i]);
		}
	}
/*    IntIntMapA_print(ia);*/
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
	ASSERT(cstl_iter_eq(cstl_find(ia, *cstl_iter_key(cstl_begin(ia)) -1), cstl_end(ia)));
	ASSERT(cstl_iter_eq(cstl_lower_bound(ia, *cstl_iter_key(cstl_rbegin(ia)) +1), cstl_end(ia)));
	ASSERT(cstl_iter_eq(cstl_upper_bound(ia, *cstl_iter_key(cstl_rbegin(ia))), cstl_end(ia)));
	/* begin, end, next, key, value, at */
	for (p = cstl_begin(ia), i = 0; cstl_iter_ne(p, cstl_end(ia)); cstl_iter_inc(&p), i++) {
		ASSERT(*cstl_iter_key(p) == i);
		ASSERT(*cstl_iter_val(p) == i);
		ASSERT(*cstl_at(ia, *cstl_iter_key(p)) == i);
		*cstl_iter_val(p) = ~i;
		ASSERT(*cstl_at(ia, *cstl_iter_key(p)) == ~i);
		*cstl_at(ia, *cstl_iter_key(p)) = i;
	}
	ASSERT(i == SIZE/2);
/*    ASSERT(cstl_iter_next(cstl_rbegin(ia)) == cstl_end(ia));*/
	ASSERT(*cstl_iter_key(cstl_rbegin(ia)) == *cstl_iter_key(cstl_iter_prev(cstl_end(ia))));
	ASSERT(*cstl_iter_val(cstl_rbegin(ia)) == *cstl_iter_val(cstl_iter_prev(cstl_end(ia))));
	/* rbegin, rend, prev, key, value, at */
	for (p = cstl_rbegin(ia), i = SIZE/2 -1; cstl_iter_ne(p, cstl_rend(ia)); cstl_iter_inc(&p), i--) {
		ASSERT(*cstl_iter_key(p) == i);
		ASSERT(*cstl_iter_val(p) == i);
		ASSERT(*cstl_at(ia, *cstl_iter_key(p)) == i);
		*cstl_iter_val(p) = ~i;
		ASSERT(*cstl_at(ia, *cstl_iter_key(p)) == ~i);
		*cstl_at(ia, *cstl_iter_key(p)) = i;
	}
	ASSERT(i == -1);
/*    ASSERT(cstl_iter_prev(cstl_begin(ia)) == cstl_rend(ia));*/
	ASSERT(*cstl_iter_key(cstl_begin(ia)) == *cstl_iter_key(cstl_iter_prev(cstl_rend(ia))));
	ASSERT(*cstl_iter_val(cstl_begin(ia)) == *cstl_iter_val(cstl_iter_prev(cstl_rend(ia))));
	/* erase */
	for (i = 0; i < SIZE; i++) {
		if (success[i]) {
			IntIntMapAIterator itr = cstl_iter_next(pos[i]);
			ASSERT(cstl_iter_eq(itr, cstl_erase(ia, pos[i])));
			count--;
		}
	}
	ASSERT(cstl_empty(ia));
	ASSERT(cstl_size(ia) == 0);
	ASSERT(count == 0);
	/* erase_range */
	for (i = 0; i < SIZE/2; i++) {
		ASSERT(cstl_map_insert(ia, hoge_int[i], hoge_int[i], &pos[i], NULL));
/*        pos[i] = cstl_map_insert(ia, hoge_int[i], hoge_int[i], NULL);*/
		ASSERT(cstl_iter_ne(pos[i], cstl_end(ia)));
	}
	ASSERT(cstl_size(ia) == SIZE/2);
	ASSERT(cstl_iter_eq(cstl_find(ia, SIZE/2 -2), cstl_erase_range(ia, cstl_find(ia, 2), cstl_find(ia, SIZE/2 -2))));
	ASSERT(cstl_size(ia) == 4);
	ASSERT(cstl_iter_eq(cstl_end(ia), cstl_erase_range(ia, cstl_begin(ia), cstl_end(ia))));
	ASSERT(cstl_size(ia) == 0);
	ASSERT(cstl_map_insert(ia, hoge_int[0], hoge_int[0], NULL, NULL));
	ASSERT(cstl_size(ia) == 1);
	p = cstl_iter_next(cstl_begin(ia));
	ASSERT(cstl_iter_eq(p, cstl_erase_range(ia, cstl_begin(ia), cstl_iter_next(cstl_begin(ia)))));
/*    ASSERT(cstl_iter_next(cstl_begin(ia)) == cstl_erase_range(ia, cstl_begin(ia), cstl_iter_next(cstl_begin(ia))));*/
	ASSERT(cstl_size(ia) == 0);
	ASSERT(cstl_map_insert(ia, 100, 101, NULL, NULL));
	ASSERT(cstl_map_insert(ia, 110, 111, NULL, NULL));
	ASSERT(cstl_size(ia) == 2);
	ASSERT(cstl_iter_eq(cstl_upper_bound(ia, 110), cstl_erase_range(ia, cstl_lower_bound(ia, 100), cstl_upper_bound(ia, 110))));
	ASSERT(cstl_size(ia) == 0);
	/* erase_key */
	for (i = 0; i < SIZE/2; i++) {
		ASSERT(cstl_map_insert(ia, hoge_int[i], hoge_int[i], &pos[i], NULL));
/*        pos[i] = cstl_map_insert(ia, hoge_int[i], hoge_int[i], NULL);*/
		ASSERT(cstl_iter_ne(pos[i], cstl_end(ia)));
	}
	ASSERT(cstl_size(ia) == SIZE/2);
	for (i = 0; i < SIZE/2; i++) {
		ASSERT(cstl_erase_key(ia, hoge_int[i]) == 1);
	}
	ASSERT(cstl_size(ia) == 0);
	/* 大量にinsert */
	count = 0;
	while (count < 1000000 && cstl_map_insert(ia, count, 9999, NULL, NULL)) {
		count++;
	}
	ASSERT(cstl_size(ia) == count);
	printf("count: %d\n", count);
	printf("size: %d\n", cstl_size(ia));
	/* clear */
	cstl_clear(ia);
	printf("size: %d\n", cstl_size(ia));
	ASSERT(cstl_size(ia) == 0);
	ASSERT(cstl_map_insert(ia, 100, 123, NULL, NULL));
	ASSERT(cstl_size(ia) == 1);
	cstl_clear(ia);
	ASSERT(cstl_size(ia) == 0);
	cstl_clear(ia);
	ASSERT(cstl_size(ia) == 0);

	/* at */
	/* 挿入 */
	*cstl_at(ia, 1) = 482;
	*cstl_at(ia, 2) = 4;
	*cstl_at(ia, 3) = 554;
	*cstl_at(ia, 4) = 44;
	*cstl_at(ia, 5) = 89;
/*    IntIntMapA_print(ia);*/
	/* 書換え */
	*cstl_at(ia, 1) = 82;
/*    IntIntMapA_print(ia);*/

	/* swap */
	x = IntIntMapA_new();
	*cstl_at(x, 10) = 2;
	*cstl_at(x, 20) = 442;
	*cstl_at(x, 30) = 694;
/*    IntIntMapA_print(ia);*/
/*    IntIntMapA_print(x);*/
	IntIntMapA_swap(ia, x);
/*    IntIntMapA_print(ia);*/
/*    IntIntMapA_print(x);*/
	ASSERT(cstl_size(ia) == 3);
	ASSERT(cstl_size(x) == 5);
	/* insert_range */
/*    IntIntMapA_print(ia);*/
/*    IntIntMapA_print(x);*/
	ASSERT(cstl_assoc_insert_range(x, cstl_begin(ia), cstl_end(ia)));
/*    IntIntMapA_print(ia);*/
/*    IntIntMapA_print(x);*/
	ASSERT(cstl_size(x) == 8);
	ASSERT(cstl_assoc_insert_range(x, cstl_begin(ia), cstl_end(ia)));
	ASSERT(cstl_size(x) == 8);
	IntIntMapA_print(ia);
	IntIntMapA_print(x);



	POOL_DUMP_OVERFLOW(&pool);
	cstl_delete(ia);
	cstl_delete(x);
}

void MapTest_test_1_2(void)
{
	int i;
	size_t count = 0;
	IntIntMMapAIterator pos[SIZE];
	IntIntMMapAIterator p;
	int flag[SIZE/2] = {0};
	ima = IntIntMMapA_new();
	/* 初期状態 */
	ASSERT(cstl_empty(ima));
	ASSERT(cstl_size(ima) == 0);
	ASSERT(cstl_iter_eq(cstl_begin(ima), cstl_end(ima)));
	ASSERT(cstl_iter_eq(cstl_rbegin(ima), cstl_rend(ima)));
	/* insert */
	for (i = 0; i < SIZE; i++) {
/*        pos[i] = cstl_multimap_insert(ima, hoge_int[i], hoge_int[i]);*/
/*        ASSERT(pos[i] && cstl_iter_ne(pos[i], cstl_end(ima)));*/
		ASSERT(cstl_multimap_insert(ima, hoge_int[i], hoge_int[i], &pos[i]));
		ASSERT(cstl_iter_ne(pos[i], cstl_end(ima)));
		count++;
	}
/*    IntIntMMapA_print(ima);*/
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
	ASSERT(cstl_iter_eq(cstl_find(ima, *cstl_iter_key(cstl_begin(ima)) -1), cstl_end(ima)));
	ASSERT(cstl_iter_eq(cstl_lower_bound(ima, *cstl_iter_key(cstl_rbegin(ima)) +1), cstl_end(ima)));
	ASSERT(cstl_iter_eq(cstl_upper_bound(ima, *cstl_iter_key(cstl_rbegin(ima))), cstl_end(ima)));
	/* begin, end, next, key, value */
	for (p = cstl_begin(ima), i = 0; cstl_iter_ne(p, cstl_end(ima)); cstl_iter_inc(&p), i++) {
/*        printf("%d, %d, %d\n", i, *cstl_iter_key(p), *cstl_iter_val(p));*/
	}
	ASSERT(i == SIZE);
/*    ASSERT(cstl_iter_next(cstl_rbegin(ima)) == cstl_end(ima));*/
	ASSERT(*cstl_iter_key(cstl_rbegin(ima)) == *cstl_iter_key(cstl_iter_prev(cstl_end(ima))));
	ASSERT(*cstl_iter_val(cstl_rbegin(ima)) == *cstl_iter_val(cstl_iter_prev(cstl_end(ima))));
	/* rbegin, rend, prev, key, value */
	for (p = cstl_rbegin(ima), i = SIZE -1; cstl_iter_ne(p, cstl_rend(ima)); cstl_iter_inc(&p), i--) {
/*        printf("%d, %d, %d\n", i, *cstl_iter_key(p), *cstl_iter_val(ima, p));*/
	}
	ASSERT(i == -1);
/*    ASSERT(cstl_iter_prev(cstl_begin(ima)) == cstl_rend(ima));*/
	ASSERT(*cstl_iter_key(cstl_begin(ima)) == *cstl_iter_key(cstl_iter_prev(cstl_rend(ima))));
	ASSERT(*cstl_iter_val(cstl_begin(ima)) == *cstl_iter_val(cstl_iter_prev(cstl_rend(ima))));
	/* erase */
	for (i = 0; i < SIZE; i++) {
		IntIntMMapAIterator itr = cstl_iter_next(pos[i]);
		ASSERT(cstl_iter_eq(itr, cstl_erase(ima, pos[i])));
		count--;
	}
	ASSERT(cstl_empty(ima));
	ASSERT(cstl_size(ima) == 0);
	ASSERT(count == 0);
	/* erase_range */
	for (i = 0; i < SIZE; i++) {
		ASSERT(cstl_multimap_insert(ima, hoge_int[i], hoge_int[i], &pos[i]));
/*        pos[i] = cstl_multimap_insert(ima, hoge_int[i], hoge_int[i]);*/
		ASSERT(cstl_iter_ne(pos[i], cstl_end(ima)));
	}
	ASSERT(cstl_size(ima) == SIZE);
/*    ASSERT(cstl_iter_next(cstl_find(ima, SIZE/2/2 -1)) == cstl_erase_range(ima, cstl_find(ima, 0), cstl_iter_next(cstl_find(ima, SIZE/2/2 -1))));*/
	p = cstl_iter_next(cstl_find(ima, SIZE/2/2 -1));
	ASSERT(cstl_iter_eq(p, cstl_erase_range(ima, cstl_find(ima, 0), cstl_iter_next(cstl_find(ima, SIZE/2/2 -1)))));
	ASSERT(cstl_size(ima) == SIZE - SIZE/2/2);
	ASSERT(cstl_iter_eq(cstl_end(ima), cstl_erase_range(ima, cstl_begin(ima), cstl_end(ima))));
	ASSERT(cstl_size(ima) == 0);
	ASSERT(cstl_multimap_insert(ima, hoge_int[0], hoge_int[0], NULL));
	ASSERT(cstl_size(ima) == 1);
/*    ASSERT(cstl_iter_next(cstl_begin(ima)) == cstl_erase_range(ima, cstl_begin(ima), cstl_iter_next(cstl_begin(ima))));*/
	p = cstl_iter_next(cstl_begin(ima));
	ASSERT(cstl_iter_eq(p, cstl_erase_range(ima, cstl_begin(ima), cstl_iter_next(cstl_begin(ima)))));
	ASSERT(cstl_size(ima) == 0);
	ASSERT(cstl_multimap_insert(ima, 100, 100, NULL));
	ASSERT(cstl_multimap_insert(ima, 100, 100, NULL));
	ASSERT(cstl_multimap_insert(ima, 100, 100, NULL));
	ASSERT(cstl_multimap_insert(ima, 110, 110, NULL));
	ASSERT(cstl_multimap_insert(ima, 110, 110, NULL));
	ASSERT(cstl_size(ima) == 5);
	ASSERT(cstl_iter_eq(cstl_upper_bound(ima, 110), cstl_erase_range(ima, cstl_lower_bound(ima, 100), cstl_upper_bound(ima, 110))));
	ASSERT(cstl_size(ima) == 0);
	/* erase_key */
	for (i = 0; i < SIZE; i++) {
		ASSERT(cstl_multimap_insert(ima, hoge_int[i], hoge_int[i], &pos[i]));
/*        pos[i] = cstl_multimap_insert(ima, hoge_int[i], hoge_int[i]);*/
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
	while (count < 1000000 && cstl_multimap_insert(ima, 0, count, NULL)) {
		count++;
	}
	ASSERT(cstl_size(ima) == count);
	printf("count: %d\n", count);

	POOL_DUMP_OVERFLOW(&pool);
	cstl_delete(ima);
}



static TestCase map_tests[] = {
	{ "MapTest_test_1_1", MapTest_test_1_1 },
	{ "MapTest_test_1_2", MapTest_test_1_2 },
	TEST_CASE_NULL,
};

static int setup(void)
{
	map_init_hoge();
	return 0;
}

static TestSuite suites[] = {
	{ "map test", setup, 0, map_tests },
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
