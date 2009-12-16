#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include "../cstl/ring.h"
#include "../cstl/unordered_map.h"
#include "hashtable_debug.h"
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
#include "IntIntUMap.h"
#include "IntIntUMMap.h"
#else

CSTL_UNORDERED_MAP_INTERFACE(IntIntUMap, int, int)
CSTL_UNORDERED_MAP_DEBUG_INTERFACE(IntIntUMap)

CSTL_UNORDERED_MULTIMAP_INTERFACE(IntIntUMMap, int, int)
CSTL_UNORDERED_MAP_DEBUG_INTERFACE(IntIntUMMap)

/* int */
CSTL_UNORDERED_MAP_IMPLEMENT(IntIntUMap, int, int, IntIntUMap_hash_int, CSTL_EQUAL_TO)
CSTL_UNORDERED_MAP_DEBUG_IMPLEMENT(IntIntUMap, int, int, IntIntUMap_hash_int, CSTL_EQUAL_TO, %d, %d)

CSTL_UNORDERED_MULTIMAP_IMPLEMENT(IntIntUMMap, int, int, IntIntUMap_hash_int, CSTL_EQUAL_TO)
CSTL_UNORDERED_MAP_DEBUG_IMPLEMENT(IntIntUMMap, int, int, IntIntUMap_hash_int, CSTL_EQUAL_TO, %d, %d)
#endif
static IntIntUMap *ia;
static IntIntUMMap *ima;



#define SIZE	32
static int hoge_int[SIZE];
static double hoge_double[SIZE];
static int *hoge_ptr[SIZE];
static char *hoge_str[SIZE];
static char str[SIZE][16];

void unordered_map_init_hoge(void)
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

void UMapTest_test_1_1(void)
{
	int i;
	int success[SIZE];
	size_t count = 0;
	IntIntUMapIterator pos[SIZE];
	IntIntUMapIterator p;
	IntIntUMap *x;
	printf("***** test_1_1 *****\n");
	ia = IntIntUMap_new();
	/* 初期状態 */
	assert(IntIntUMap_empty(ia));
	assert(IntIntUMap_size(ia) == 0);
	assert(IntIntUMap_begin(ia) == IntIntUMap_end(ia));
	/* insert */
	for (i = 0; i < SIZE; i++) {
		pos[i] = IntIntUMap_insert(ia, hoge_int[i], hoge_int[i], &success[i]);
		if (i < SIZE/2) {
			assert(success[i]);
			count++;
		} else {
			assert(!success[i]);
		}
		assert(pos[i]);
	}
/*    IntIntUMap_print(ia);*/
	assert(!IntIntUMap_empty(ia));
	assert(IntIntUMap_size(ia) == count);
	assert(count == SIZE/2);
	/* count, find */
	for (i = 0; i < SIZE/2; i++) {
		assert(IntIntUMap_count(ia, hoge_int[i]) == 1);
		assert(pos[i] == IntIntUMap_find(ia, hoge_int[i]));
	}
	/* begin, end, next, key, value, at */
	for (p = IntIntUMap_begin(ia), i = 0; p != IntIntUMap_end(ia); p = IntIntUMap_next(p), i++) {
		assert(*IntIntUMap_key(p) == i);
		assert(*IntIntUMap_value(p) == i);
		assert(*IntIntUMap_at(ia, *IntIntUMap_key(p)) == i);
		*IntIntUMap_value(p) = ~i;
		assert(*IntIntUMap_at(ia, *IntIntUMap_key(p)) == ~i);
		*IntIntUMap_at(ia, *IntIntUMap_key(p)) = i;
	}
	assert(i == SIZE/2);
	/* erase */
	for (i = 0; i < SIZE; i++) {
		if (pos[i] && success[i]) {
			IntIntUMapIterator itr = IntIntUMap_next(pos[i]);
			assert(itr == IntIntUMap_erase(ia, pos[i]));
			count--;
		}
	}
	assert(IntIntUMap_empty(ia));
	assert(IntIntUMap_size(ia) == 0);
	assert(count == 0);
	/* erase_range */
	for (i = 0; i < SIZE/2; i++) {
		pos[i] = IntIntUMap_insert(ia, hoge_int[i], hoge_int[i], NULL);
		assert(pos[i] && pos[i] != IntIntUMap_end(ia));
	}
	assert(IntIntUMap_size(ia) == SIZE/2);
	assert(IntIntUMap_find(ia, SIZE/2 -2) == IntIntUMap_erase_range(ia, IntIntUMap_find(ia, 2), IntIntUMap_find(ia, SIZE/2 -2)));
	assert(IntIntUMap_size(ia) == 4);
	assert(IntIntUMap_end(ia) == IntIntUMap_erase_range(ia, IntIntUMap_begin(ia), IntIntUMap_end(ia)));
	assert(IntIntUMap_size(ia) == 0);
	assert(IntIntUMap_insert(ia, hoge_int[0], hoge_int[0], NULL));
	assert(IntIntUMap_size(ia) == 1);
	assert(IntIntUMap_next(IntIntUMap_begin(ia)) == IntIntUMap_erase_range(ia, IntIntUMap_begin(ia), IntIntUMap_next(IntIntUMap_begin(ia))));
	assert(IntIntUMap_size(ia) == 0);
	/* erase_key */
	for (i = 0; i < SIZE/2; i++) {
		pos[i] = IntIntUMap_insert(ia, hoge_int[i], hoge_int[i], NULL);
		assert(pos[i] && pos[i] != IntIntUMap_end(ia));
	}
	assert(IntIntUMap_size(ia) == SIZE/2);
	for (i = 0; i < SIZE/2; i++) {
		assert(IntIntUMap_erase_key(ia, hoge_int[i]) == 1);
	}
	assert(IntIntUMap_size(ia) == 0);
	/* 大量にinsert */
	count = 0;
	while (count < 1000000 && IntIntUMap_insert(ia, count, 9999, NULL)) {
		count++;
	}
	assert(IntIntUMap_size(ia) == count);
	printf("count: %d\n", count);
	printf("size: %d\n", IntIntUMap_size(ia));
	/* clear */
	IntIntUMap_clear(ia);
	printf("size: %d\n", IntIntUMap_size(ia));
	assert(IntIntUMap_size(ia) == 0);
	assert(IntIntUMap_insert(ia, 100, 123, NULL));
	assert(IntIntUMap_size(ia) == 1);
	IntIntUMap_clear(ia);
	assert(IntIntUMap_size(ia) == 0);
	IntIntUMap_clear(ia);
	assert(IntIntUMap_size(ia) == 0);

	/* at */
	/* 挿入 */
	*IntIntUMap_at(ia, 1) = 482;
	*IntIntUMap_at(ia, 2) = 4;
	*IntIntUMap_at(ia, 3) = 554;
	*IntIntUMap_at(ia, 4) = 44;
	*IntIntUMap_at(ia, 5) = 89;
/*    IntIntUMap_print(ia);*/
	/* 書換え */
	*IntIntUMap_at(ia, 1) = 82;
/*    IntIntUMap_print(ia);*/

	/* swap */
	x = IntIntUMap_new();
	*IntIntUMap_at(x, 10) = 2;
	*IntIntUMap_at(x, 20) = 442;
	*IntIntUMap_at(x, 30) = 694;
/*    IntIntUMap_print(ia);*/
/*    IntIntUMap_print(x);*/
	IntIntUMap_swap(ia, x);
/*    IntIntUMap_print(ia);*/
/*    IntIntUMap_print(x);*/
	assert(IntIntUMap_size(ia) == 3);
	assert(IntIntUMap_size(x) == 5);
	/* insert_range */
/*    IntIntUMap_print(ia);*/
/*    IntIntUMap_print(x);*/
	assert(IntIntUMap_insert_range(x, IntIntUMap_begin(ia), IntIntUMap_end(ia)));
/*    IntIntUMap_print(ia);*/
/*    IntIntUMap_print(x);*/
	assert(IntIntUMap_size(x) == 8);
	assert(IntIntUMap_insert_range(x, IntIntUMap_begin(ia), IntIntUMap_end(ia)));
	assert(IntIntUMap_size(x) == 8);
	IntIntUMap_print(ia);
	IntIntUMap_print(x);



	POOL_DUMP_OVERFLOW(&pool);
	IntIntUMap_delete(ia);
	IntIntUMap_delete(x);
}

void UMapTest_test_1_2(void)
{
	int i;
	size_t count = 0;
	IntIntUMMapIterator pos[SIZE];
	IntIntUMMapIterator p;
	IntIntUMMapIterator q;
	int flag[SIZE/2] = {0};
	printf("***** test_1_2 *****\n");
	ima = IntIntUMMap_new();
	/* 初期状態 */
	assert(IntIntUMMap_empty(ima));
	assert(IntIntUMMap_size(ima) == 0);
	assert(IntIntUMMap_begin(ima) == IntIntUMMap_end(ima));
	/* insert */
	for (i = 0; i < SIZE; i++) {
		pos[i] = IntIntUMMap_insert(ima, hoge_int[i], hoge_int[i]);
		assert(pos[i] && pos[i] != IntIntUMMap_end(ima));
		count++;
	}
/*    IntIntUMMap_print(ima);*/
	assert(!IntIntUMMap_empty(ima));
	assert(IntIntUMMap_size(ima) == count);
	assert(count == SIZE);
	/* count */
	for (i = 0; i < SIZE; i++) {
		if (i < SIZE/2/2) {
			assert(IntIntUMMap_count(ima, i) == 1);
		} else if (i < SIZE/2) {
			assert(IntIntUMMap_count(ima, i) == 3);
		} else {
			assert(IntIntUMMap_count(ima, i) == 0);
		}
	}
	/* find */
	for (i = 0; i < SIZE; i++) {
		if (IntIntUMMap_count(ima, hoge_int[i]) == 1) {
			assert(pos[i] == IntIntUMMap_find(ima, hoge_int[i]));
		} else if (IntIntUMMap_count(ima, hoge_int[i]) == 3) {
			if (!flag[hoge_int[i]]) {
				flag[hoge_int[i]] = 1;
			}
		} else {
			assert(0);
		}
	}
	assert(IntIntUMMap_find(ima, *IntIntUMMap_key(IntIntUMMap_begin(ima)) -1) == IntIntUMMap_end(ima));
	/* begin, end, next, key, value */
	for (p = IntIntUMMap_begin(ima), i = 0; p != IntIntUMMap_end(ima); p = IntIntUMMap_next(p), i++) {
/*        printf("%d, %d, %d\n", i, *IntIntUMMap_key(p), *IntIntUMMap_value(p));*/
	}
	assert(i == SIZE);
	/* erase */
	for (i = 0; i < SIZE; i++) {
		IntIntUMMapIterator itr = IntIntUMMap_next(pos[i]);
		assert(itr == IntIntUMMap_erase(ima, pos[i]));
		count--;
	}
	assert(IntIntUMMap_empty(ima));
	assert(IntIntUMMap_size(ima) == 0);
	assert(count == 0);
	/* erase_range */
	for (i = 0; i < SIZE; i++) {
		pos[i] = IntIntUMMap_insert(ima, hoge_int[i], hoge_int[i]);
		assert(pos[i] && pos[i] != IntIntUMMap_end(ima));
	}
	assert(IntIntUMMap_size(ima) == SIZE);
	assert(IntIntUMMap_next(IntIntUMMap_find(ima, SIZE/2/2 -1)) == IntIntUMMap_erase_range(ima, IntIntUMMap_find(ima, 0), IntIntUMMap_next(IntIntUMMap_find(ima, SIZE/2/2 -1))));
	assert(IntIntUMMap_size(ima) == SIZE - SIZE/2/2);
	assert(IntIntUMMap_end(ima) == IntIntUMMap_erase_range(ima, IntIntUMMap_begin(ima), IntIntUMMap_end(ima)));
	assert(IntIntUMMap_size(ima) == 0);
	assert(IntIntUMMap_insert(ima, hoge_int[0], hoge_int[0]));
	assert(IntIntUMMap_size(ima) == 1);
	assert(IntIntUMMap_next(IntIntUMMap_begin(ima)) == IntIntUMMap_erase_range(ima, IntIntUMMap_begin(ima), IntIntUMMap_next(IntIntUMMap_begin(ima))));
	assert(IntIntUMMap_size(ima) == 0);
	assert(IntIntUMMap_insert(ima, 100, 100));
	assert(IntIntUMMap_insert(ima, 100, 100));
	assert(IntIntUMMap_insert(ima, 100, 100));
	assert(IntIntUMMap_insert(ima, 110, 110));
	assert(IntIntUMMap_insert(ima, 110, 110));
	assert(IntIntUMMap_size(ima) == 5);
	IntIntUMMap_print(ima);
	/* equal_range */
	IntIntUMMap_equal_range(ima, 100, &p, &q);
	assert(q == IntIntUMMap_erase_range(ima, p, q));
	assert(IntIntUMMap_size(ima) == 2);
	IntIntUMMap_print(ima);
	IntIntUMMap_equal_range(ima, 110, &p, &q);
	assert(q == IntIntUMMap_erase_range(ima, p, q));
	assert(IntIntUMMap_size(ima) == 0);
	/* erase_key */
	for (i = 0; i < SIZE; i++) {
		pos[i] = IntIntUMMap_insert(ima, hoge_int[i], hoge_int[i]);
		assert(pos[i] && pos[i] != IntIntUMMap_end(ima));
	}
	assert(IntIntUMMap_size(ima) == SIZE);
	for (i = 0; i < SIZE/2/2; i++) {
		assert(IntIntUMMap_erase_key(ima, i) == 1);
	}
	assert(IntIntUMMap_size(ima) == SIZE - SIZE/2/2);
	for (i = SIZE/2/2; i < SIZE/2; i++) {
		assert(IntIntUMMap_erase_key(ima, i) == 3);
	}
	assert(IntIntUMMap_size(ima) == 0);
	/* 大量にinsert */
	count = 0;
	while (count < 1000000 && IntIntUMMap_insert(ima, 0, count)) {
		count++;
	}
	assert(IntIntUMMap_size(ima) == count);
	printf("count: %d\n", count);

	POOL_DUMP_OVERFLOW(&pool);
	IntIntUMMap_delete(ima);
}






void UMapTest_run(void)
{
	printf("\n===== unordered_map test =====\n");
	unordered_map_init_hoge();

	UMapTest_test_1_1();
	UMapTest_test_1_2();
}


int main(void)
{
#ifdef MY_MALLOC
	Pool_init(&pool, buf, sizeof buf, sizeof buf[0]);
#endif
	UMapTest_run();
#ifdef MY_MALLOC
	POOL_DUMP_LEAK(&pool, 0);
#endif
	return 0;
}
