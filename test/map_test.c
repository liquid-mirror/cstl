#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include "../cstl/ring.h"
#include "../cstl/map.h"
#include "rbtree_debug.h"
#include "heap.h"
#ifdef MY_MALLOC
double buf[1024*1024/sizeof(double)];
Heap heap;
#define malloc(s)		Heap_alloc(&heap, s)
#define realloc(p, s)	Heap_realloc(&heap, p, s)
#define free(p)			Heap_free(&heap, p)
#endif

/* ring */
CSTL_RING_INTERFACE(IntRing, int)
CSTL_RING_IMPLEMENT(IntRing, int)


/* map */
#ifdef CODEGEN
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
	HEAP_DUMP_OVERFLOW(&heap);
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
	printf("***** test_1_1 *****\n");
	ia = IntIntMapA_new();
	/* 初期状態 */
	assert(IntIntMapA_empty(ia));
	assert(IntIntMapA_size(ia) == 0);
	assert(IntIntMapA_begin(ia) == IntIntMapA_end(ia));
	assert(IntIntMapA_rbegin(ia) == IntIntMapA_rend(ia));
	/* insert */
	for (i = 0; i < SIZE; i++) {
		pos[i] = IntIntMapA_insert(ia, hoge_int[i], hoge_int[i], &success[i]);
		if (i < SIZE/2) {
			assert(success[i]);
			count++;
		} else {
			assert(!success[i]);
		}
		assert(pos[i]);
	}
/*    IntIntMapA_print(ia);*/
	assert(!IntIntMapA_empty(ia));
	assert(IntIntMapA_size(ia) == count);
	assert(count == SIZE/2);
	/* count, find, lower_bound, upper_bound */
	for (i = 0; i < SIZE/2; i++) {
		assert(IntIntMapA_count(ia, hoge_int[i]) == 1);
		assert(pos[i] == IntIntMapA_find(ia, hoge_int[i]));
		assert(pos[i] == IntIntMapA_lower_bound(ia, hoge_int[i]));
		assert(pos[i] == IntIntMapA_upper_bound(ia, hoge_int[i]-1));
		assert(IntIntMapA_lower_bound(ia, hoge_int[i]+1) == IntIntMapA_upper_bound(ia, hoge_int[i]));
	}
	assert(IntIntMapA_find(ia, IntIntMapA_key(IntIntMapA_begin(ia)) -1) == IntIntMapA_end(ia));
	assert(IntIntMapA_lower_bound(ia, IntIntMapA_key(IntIntMapA_rbegin(ia)) +1) == IntIntMapA_end(ia));
	assert(IntIntMapA_upper_bound(ia, IntIntMapA_key(IntIntMapA_rbegin(ia))) == IntIntMapA_end(ia));
	/* begin, end, next, key, value, lookup */
	for (p = IntIntMapA_begin(ia), i = 0; p != IntIntMapA_end(ia); p = IntIntMapA_next(p), i++) {
		assert(IntIntMapA_key(p) == i);
		assert(*IntIntMapA_value(p) == i);
		assert(*IntIntMapA_lookup(ia, IntIntMapA_key(p)) == i);
		*IntIntMapA_value(p) = ~i;
		assert(*IntIntMapA_lookup(ia, IntIntMapA_key(p)) == ~i);
		*IntIntMapA_lookup(ia, IntIntMapA_key(p)) = i;
	}
	assert(i == SIZE/2);
	assert(IntIntMapA_next(IntIntMapA_rbegin(ia)) == IntIntMapA_end(ia));
	/* rbegin, rend, prev, key, value, lookup */
	for (p = IntIntMapA_rbegin(ia), i = SIZE/2 -1; p != IntIntMapA_rend(ia); p = IntIntMapA_prev(p), i--) {
		assert(IntIntMapA_key(p) == i);
		assert(*IntIntMapA_value(p) == i);
		assert(*IntIntMapA_lookup(ia, IntIntMapA_key(p)) == i);
		*IntIntMapA_value(p) = ~i;
		assert(*IntIntMapA_lookup(ia, IntIntMapA_key(p)) == ~i);
		*IntIntMapA_lookup(ia, IntIntMapA_key(p)) = i;
	}
	assert(i == -1);
	assert(IntIntMapA_prev(IntIntMapA_begin(ia)) == IntIntMapA_rend(ia));
	/* erase */
	for (i = 0; i < SIZE; i++) {
		if (pos[i] && success[i]) {
			IntIntMapAIterator itr = IntIntMapA_next(pos[i]);
			assert(itr == IntIntMapA_erase(ia, pos[i]));
			count--;
		}
	}
	assert(IntIntMapA_empty(ia));
	assert(IntIntMapA_size(ia) == 0);
	assert(count == 0);
	/* erase_range */
	for (i = 0; i < SIZE/2; i++) {
		pos[i] = IntIntMapA_insert(ia, hoge_int[i], hoge_int[i], NULL);
		assert(pos[i] && pos[i] != IntIntMapA_end(ia));
	}
	assert(IntIntMapA_size(ia) == SIZE/2);
	assert(IntIntMapA_find(ia, SIZE/2 -2) == IntIntMapA_erase_range(ia, IntIntMapA_find(ia, 2), IntIntMapA_find(ia, SIZE/2 -2)));
	assert(IntIntMapA_size(ia) == 4);
	assert(IntIntMapA_end(ia) == IntIntMapA_erase_range(ia, IntIntMapA_begin(ia), IntIntMapA_end(ia)));
	assert(IntIntMapA_size(ia) == 0);
	assert(IntIntMapA_insert(ia, hoge_int[0], hoge_int[0], NULL));
	assert(IntIntMapA_size(ia) == 1);
	assert(IntIntMapA_next(IntIntMapA_begin(ia)) == IntIntMapA_erase_range(ia, IntIntMapA_begin(ia), IntIntMapA_next(IntIntMapA_begin(ia))));
	assert(IntIntMapA_size(ia) == 0);
	assert(IntIntMapA_insert(ia, 100, 101, NULL));
	assert(IntIntMapA_insert(ia, 110, 111, NULL));
	assert(IntIntMapA_size(ia) == 2);
	assert(IntIntMapA_upper_bound(ia, 110) == IntIntMapA_erase_range(ia, IntIntMapA_lower_bound(ia, 100), IntIntMapA_upper_bound(ia, 110)));
	assert(IntIntMapA_size(ia) == 0);
	/* erase_key */
	for (i = 0; i < SIZE/2; i++) {
		pos[i] = IntIntMapA_insert(ia, hoge_int[i], hoge_int[i], NULL);
		assert(pos[i] && pos[i] != IntIntMapA_end(ia));
	}
	assert(IntIntMapA_size(ia) == SIZE/2);
	for (i = 0; i < SIZE/2; i++) {
		assert(IntIntMapA_erase_key(ia, hoge_int[i]) == 1);
	}
	assert(IntIntMapA_size(ia) == 0);
	/* 大量にinsert */
	count = 0;
	while (count < 1000000 && IntIntMapA_insert(ia, count, 9999, NULL)) {
		count++;
	}
	assert(IntIntMapA_size(ia) == count);
	printf("count: %d\n", count);
	printf("size: %d\n", IntIntMapA_size(ia));
	/* clear */
	IntIntMapA_clear(ia);
	printf("size: %d\n", IntIntMapA_size(ia));
	assert(IntIntMapA_size(ia) == 0);
	assert(IntIntMapA_insert(ia, 100, 123, NULL));
	assert(IntIntMapA_size(ia) == 1);
	IntIntMapA_clear(ia);
	assert(IntIntMapA_size(ia) == 0);
	IntIntMapA_clear(ia);
	assert(IntIntMapA_size(ia) == 0);

	/* lookup */
	/* 挿入 */
	*IntIntMapA_lookup(ia, 1) = 482;
	*IntIntMapA_lookup(ia, 2) = 4;
	*IntIntMapA_lookup(ia, 3) = 554;
	*IntIntMapA_lookup(ia, 4) = 44;
	*IntIntMapA_lookup(ia, 5) = 89;
/*    IntIntMapA_print(ia);*/
	/* 書換え */
	*IntIntMapA_lookup(ia, 1) = 82;
/*    IntIntMapA_print(ia);*/

	/* swap */
	x = IntIntMapA_new();
	*IntIntMapA_lookup(x, 10) = 2;
	*IntIntMapA_lookup(x, 20) = 442;
	*IntIntMapA_lookup(x, 30) = 694;
/*    IntIntMapA_print(ia);*/
/*    IntIntMapA_print(x);*/
	IntIntMapA_swap(ia, x);
/*    IntIntMapA_print(ia);*/
/*    IntIntMapA_print(x);*/
	assert(IntIntMapA_size(ia) == 3);
	assert(IntIntMapA_size(x) == 5);
	/* insert_range */
/*    IntIntMapA_print(ia);*/
/*    IntIntMapA_print(x);*/
	assert(IntIntMapA_insert_range(x, IntIntMapA_begin(ia), IntIntMapA_end(ia)));
/*    IntIntMapA_print(ia);*/
/*    IntIntMapA_print(x);*/
	assert(IntIntMapA_size(x) == 8);
	assert(IntIntMapA_insert_range(x, IntIntMapA_begin(ia), IntIntMapA_end(ia)));
	assert(IntIntMapA_size(x) == 8);
	IntIntMapA_print(ia);
	IntIntMapA_print(x);



	HEAP_DUMP_OVERFLOW(&heap);
	IntIntMapA_delete(ia);
	IntIntMapA_delete(x);
}

void MapTest_test_1_2(void)
{
	int i;
	size_t count = 0;
	IntIntMMapAIterator pos[SIZE];
	IntIntMMapAIterator p;
	int flag[SIZE/2] = {0};
	printf("***** test_1_2 *****\n");
	ima = IntIntMMapA_new();
	/* 初期状態 */
	assert(IntIntMMapA_empty(ima));
	assert(IntIntMMapA_size(ima) == 0);
	assert(IntIntMMapA_begin(ima) == IntIntMMapA_end(ima));
	assert(IntIntMMapA_rbegin(ima) == IntIntMMapA_rend(ima));
	/* insert */
	for (i = 0; i < SIZE; i++) {
		pos[i] = IntIntMMapA_insert(ima, hoge_int[i], hoge_int[i]);
		assert(pos[i] && pos[i] != IntIntMMapA_end(ima));
		count++;
	}
/*    IntIntMMapA_print(ima);*/
	assert(!IntIntMMapA_empty(ima));
	assert(IntIntMMapA_size(ima) == count);
	assert(count == SIZE);
	/* count */
	for (i = 0; i < SIZE; i++) {
		if (i < SIZE/2/2) {
			assert(IntIntMMapA_count(ima, i) == 1);
		} else if (i < SIZE/2) {
			assert(IntIntMMapA_count(ima, i) == 3);
		} else {
			assert(IntIntMMapA_count(ima, i) == 0);
		}
	}
	/* find, lower_bound, upper_bound */
	for (i = 0; i < SIZE; i++) {
		if (IntIntMMapA_count(ima, hoge_int[i]) == 1) {
			assert(pos[i] == IntIntMMapA_find(ima, hoge_int[i]));
			assert(pos[i] == IntIntMMapA_lower_bound(ima, hoge_int[i]));
			assert(pos[i] == IntIntMMapA_upper_bound(ima, hoge_int[i]-1));
		} else if (IntIntMMapA_count(ima, hoge_int[i]) == 3) {
			if (!flag[hoge_int[i]]) {
				flag[hoge_int[i]] = 1;
				assert(pos[i] == IntIntMMapA_lower_bound(ima, hoge_int[i]));
				assert(pos[i] == IntIntMMapA_upper_bound(ima, hoge_int[i]-1));
			}
		} else {
			assert(0);
		}
		assert(IntIntMMapA_lower_bound(ima, hoge_int[i]+1) == IntIntMMapA_upper_bound(ima, hoge_int[i]));
	}
	assert(IntIntMMapA_find(ima, IntIntMMapA_key(IntIntMMapA_begin(ima)) -1) == IntIntMMapA_end(ima));
	assert(IntIntMMapA_lower_bound(ima, IntIntMMapA_key(IntIntMMapA_rbegin(ima)) +1) == IntIntMMapA_end(ima));
	assert(IntIntMMapA_upper_bound(ima, IntIntMMapA_key(IntIntMMapA_rbegin(ima))) == IntIntMMapA_end(ima));
	/* begin, end, next, key, value */
	for (p = IntIntMMapA_begin(ima), i = 0; p != IntIntMMapA_end(ima); p = IntIntMMapA_next(p), i++) {
/*        printf("%d, %d, %d\n", i, IntIntMMapA_key(p), *IntIntMMapA_value(p));*/
	}
	assert(i == SIZE);
	assert(IntIntMMapA_next(IntIntMMapA_rbegin(ima)) == IntIntMMapA_end(ima));
	/* rbegin, rend, prev, key, value */
	for (p = IntIntMMapA_rbegin(ima), i = SIZE -1; p != IntIntMMapA_rend(ima); p = IntIntMMapA_prev(p), i--) {
/*        printf("%d, %d, %d\n", i, IntIntMMapA_key(p), *IntIntMMapA_value(ima, p));*/
	}
	assert(i == -1);
	assert(IntIntMMapA_prev(IntIntMMapA_begin(ima)) == IntIntMMapA_rend(ima));
	/* erase */
	for (i = 0; i < SIZE; i++) {
		IntIntMMapAIterator itr = IntIntMMapA_next(pos[i]);
		assert(itr == IntIntMMapA_erase(ima, pos[i]));
		count--;
	}
	assert(IntIntMMapA_empty(ima));
	assert(IntIntMMapA_size(ima) == 0);
	assert(count == 0);
	/* erase_range */
	for (i = 0; i < SIZE; i++) {
		pos[i] = IntIntMMapA_insert(ima, hoge_int[i], hoge_int[i]);
		assert(pos[i] && pos[i] != IntIntMMapA_end(ima));
	}
	assert(IntIntMMapA_size(ima) == SIZE);
	assert(IntIntMMapA_next(IntIntMMapA_find(ima, SIZE/2/2 -1)) == IntIntMMapA_erase_range(ima, IntIntMMapA_find(ima, 0), IntIntMMapA_next(IntIntMMapA_find(ima, SIZE/2/2 -1))));
	assert(IntIntMMapA_size(ima) == SIZE - SIZE/2/2);
	assert(IntIntMMapA_end(ima) == IntIntMMapA_erase_range(ima, IntIntMMapA_begin(ima), IntIntMMapA_end(ima)));
	assert(IntIntMMapA_size(ima) == 0);
	assert(IntIntMMapA_insert(ima, hoge_int[0], hoge_int[0]));
	assert(IntIntMMapA_size(ima) == 1);
	assert(IntIntMMapA_next(IntIntMMapA_begin(ima)) == IntIntMMapA_erase_range(ima, IntIntMMapA_begin(ima), IntIntMMapA_next(IntIntMMapA_begin(ima))));
	assert(IntIntMMapA_size(ima) == 0);
	assert(IntIntMMapA_insert(ima, 100, 100));
	assert(IntIntMMapA_insert(ima, 100, 100));
	assert(IntIntMMapA_insert(ima, 100, 100));
	assert(IntIntMMapA_insert(ima, 110, 110));
	assert(IntIntMMapA_insert(ima, 110, 110));
	assert(IntIntMMapA_size(ima) == 5);
	assert(IntIntMMapA_upper_bound(ima, 110) == IntIntMMapA_erase_range(ima, IntIntMMapA_lower_bound(ima, 100), IntIntMMapA_upper_bound(ima, 110)));
	assert(IntIntMMapA_size(ima) == 0);
	/* erase_key */
	for (i = 0; i < SIZE; i++) {
		pos[i] = IntIntMMapA_insert(ima, hoge_int[i], hoge_int[i]);
		assert(pos[i] && pos[i] != IntIntMMapA_end(ima));
	}
	assert(IntIntMMapA_size(ima) == SIZE);
	for (i = 0; i < SIZE/2/2; i++) {
		assert(IntIntMMapA_erase_key(ima, i) == 1);
	}
	assert(IntIntMMapA_size(ima) == SIZE - SIZE/2/2);
	for (i = SIZE/2/2; i < SIZE/2; i++) {
		assert(IntIntMMapA_erase_key(ima, i) == 3);
	}
	assert(IntIntMMapA_size(ima) == 0);
	/* 大量にinsert */
	count = 0;
	while (count < 1000000 && IntIntMMapA_insert(ima, 0, count)) {
		count++;
	}
	assert(IntIntMMapA_size(ima) == count);
	printf("count: %d\n", count);

	HEAP_DUMP_OVERFLOW(&heap);
	IntIntMMapA_delete(ima);
}











void MapTest_run(void)
{
	printf("\n===== map test =====\n");
	map_init_hoge();

	MapTest_test_1_1();
	MapTest_test_1_2();
}


int main(void)
{
#ifdef MY_MALLOC
	Heap_init(&heap, buf, sizeof buf, sizeof buf[0]);
#endif
	MapTest_run();
#ifdef MY_MALLOC
	HEAP_DUMP_LEAK(&heap, 0);
#endif
	return 0;
}
