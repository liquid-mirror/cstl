#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include "../cstl/ring.h"
#include "../cstl/set.h"
#include "../cstl/unordered_set.h"
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

/* set */
CSTL_SET_INTERFACE(IntSet, int)
CSTL_SET_IMPLEMENT(IntSet, int, CSTL_LESS)


/* unordered_set */
#ifdef CSTLGEN
#include "IntUSet.h"
#include "IntUMSet.h"
#include "StrUSet.h"
#include "UIntUSet.h"
#else

CSTL_UNORDERED_SET_INTERFACE(IntUSet, int)
CSTL_UNORDERED_SET_DEBUG_INTERFACE(IntUSet)

CSTL_UNORDERED_MULTISET_INTERFACE(IntUMSet, int)
CSTL_UNORDERED_SET_DEBUG_INTERFACE(IntUMSet)

CSTL_UNORDERED_SET_INTERFACE(StrUSet, const char*)
CSTL_UNORDERED_SET_DEBUG_INTERFACE(StrUSet)

CSTL_UNORDERED_SET_INTERFACE(UIntUSet, unsigned int)
CSTL_UNORDERED_SET_DEBUG_INTERFACE(UIntUSet)


/* int */
CSTL_UNORDERED_SET_IMPLEMENT(IntUSet, int, IntUSet_hash_int, CSTL_EQUAL_TO)
CSTL_UNORDERED_SET_DEBUG_IMPLEMENT(IntUSet, int, IntUSet_hash_int, CSTL_EQUAL_TO, %d)

CSTL_UNORDERED_MULTISET_IMPLEMENT(IntUMSet, int, IntUMSet_hash_int, CSTL_EQUAL_TO)
CSTL_UNORDERED_SET_DEBUG_IMPLEMENT(IntUMSet, int, IntUMSet_hash_int, CSTL_EQUAL_TO, %d)


/* str */
CSTL_UNORDERED_SET_IMPLEMENT(StrUSet, const char*, StrUSet_hash_string, strcmp)
CSTL_UNORDERED_SET_DEBUG_IMPLEMENT(StrUSet, const char*, StrUSet_hash_string, strcmp, %s)

/* unsigned int */
CSTL_UNORDERED_SET_IMPLEMENT(UIntUSet, unsigned int, UIntUSet_hash_uint, CSTL_EQUAL_TO)
CSTL_UNORDERED_SET_DEBUG_IMPLEMENT(UIntUSet, unsigned int, UIntUSet_hash_uint, CSTL_EQUAL_TO, %d)
#endif
static IntUSet *ia;
static IntUMSet *ima;
static StrUSet *sa;



#define SIZE	32
static int hoge_int[SIZE];
static double hoge_double[SIZE];
static int *hoge_ptr[SIZE];
static char *hoge_str[SIZE];
static char str[SIZE][16];

void unordered_set_init_hoge(void)
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

void USetTest_test_1_1(void)
{
	int i;
	size_t count = 0;
	int success[SIZE];
	IntUSetIterator pos[SIZE];
	IntUSetIterator p;
	IntUSetIterator q;
	IntUSetLocalIterator lp;
	IntUSet *x;
	IntSet *set = IntSet_new();
	float mlf;
	size_t bc;
	int b[] = {100, 101, 102, 103, 104, 105, 106, 107, 108, 109};
	printf("***** test_1_1 *****\n");
	ia = IntUSet_new();
	/* 初期状態 */
	assert(IntUSet_empty(ia));
	assert(IntUSet_size(ia) == 0);
	assert(IntUSet_begin(ia) == IntUSet_end(ia));
	/* insert */
	for (i = 0; i < SIZE; i++) {
		pos[i] = IntUSet_insert(ia, hoge_int[i], &success[i]);
		assert(IntSet_insert(set, hoge_int[i], NULL)); /* iaと同じ値を挿入 */
		assert(IntUSet_verify(ia));
		assert(pos[i]);
		if (i < SIZE/2) {
			assert(success[i]);
			count++;
		} else {
			assert(!success[i]);
		}
	}
/*    IntUSet_print(ia);*/
	assert(!IntUSet_empty(ia));
	assert(IntUSet_size(ia) == count);
	assert(count == SIZE/2);
	/* count, find */
	for (i = 0; i < SIZE/2; i++) {
		assert(IntUSet_count(ia, hoge_int[i]) == 1);
		assert(pos[i] == IntUSet_find(ia, hoge_int[i]));
	}
	/* begin, end, next, data */
	for (p = IntUSet_begin(ia), i = 0; p != IntUSet_end(ia); p = IntUSet_next(p), i++) {
		assert(IntSet_find(set, *IntUSet_data(p)) != IntSet_end(set));
		assert(IntSet_erase_key(set, *IntUSet_data(p)) == 1);
	}
	assert(i == SIZE/2);
	assert(IntSet_empty(set));
	IntSet_delete(set);

	/* bucket interface */
	for (i = 0; i < IntUSet_bucket_count(ia); i++) {
/*        printf("bucket[%d]: ", i);*/
		for (lp = IntUSet_bucket_begin(ia, i); lp != IntUSet_bucket_end(ia, i); lp = IntUSet_bucket_next(lp)) {
			assert(IntUSet_find(ia, *IntUSet_data(lp)) != IntUSet_end(ia));
			assert(IntUSet_bucket_size(ia, i) > 0);
			assert(IntUSet_bucket(ia, *IntUSet_data(lp)) == (size_t) i);
/*            printf("[%d]->", *IntUSet_data(lp));*/
		}
/*        printf("\n");*/
	}

	/* hash policy */
	printf("bc:%d, lf:%g, mlf:%g\n", IntUSet_bucket_count(ia), IntUSet_load_factor(ia), IntUSet_get_max_load_factor(ia));
	assert(IntUSet_load_factor(ia) <= IntUSet_get_max_load_factor(ia));
	mlf = IntUSet_get_max_load_factor(ia);
	IntUSet_set_max_load_factor(ia, 0.001); /* load_factor() > max_load_factor()にする */
	printf("bc:%d, lf:%g, mlf:%g\n", IntUSet_bucket_count(ia), IntUSet_load_factor(ia), IntUSet_get_max_load_factor(ia));
	assert(IntUSet_verify(ia));
	IntUSet_insert(ia, 10000, NULL); /* 必ずrehash()が発生する */
	IntUSet_erase_key(ia, 10000);
	assert(IntUSet_verify(ia));
	printf("bc:%d, lf:%g, mlf:%g\n", IntUSet_bucket_count(ia), IntUSet_load_factor(ia), IntUSet_get_max_load_factor(ia));
	bc = IntUSet_bucket_count(ia);
	assert(!IntUSet_rehash(ia, bc * bc));
	assert(IntUSet_rehash(ia, bc + 1));
	assert(bc + 1 <= IntUSet_bucket_count(ia));
	printf("bc:%d, lf:%g, mlf:%g\n", IntUSet_bucket_count(ia), IntUSet_load_factor(ia), IntUSet_get_max_load_factor(ia));
	assert(IntUSet_rehash(ia, 0)); /* 小さくはならない */
	assert(bc + 1 <= IntUSet_bucket_count(ia));
	printf("bc:%d, lf:%g, mlf:%g\n", IntUSet_bucket_count(ia), IntUSet_load_factor(ia), IntUSet_get_max_load_factor(ia));
	IntUSet_set_max_load_factor(ia, mlf);
	printf("bc:%d, lf:%g, mlf:%g\n", IntUSet_bucket_count(ia), IntUSet_load_factor(ia), IntUSet_get_max_load_factor(ia));

	/* erase */
	for (i = 0; i < SIZE; i++) {
		if (pos[i] && success[i]) {
			IntUSetIterator itr = IntUSet_next(pos[i]);
			assert(itr == IntUSet_erase(ia, pos[i]));
			assert(IntUSet_verify(ia));
			count--;
		}
	}
	assert(IntUSet_empty(ia));
	assert(IntUSet_size(ia) == 0);
	assert(count == 0);
	/* erase_range */
	for (i = 0; i < SIZE/2; i++) {
		pos[i] = IntUSet_insert(ia, hoge_int[i], NULL);
		assert(pos[i]);
	}
	assert(IntUSet_size(ia) == SIZE/2);
	assert(IntUSet_end(ia) == IntUSet_erase_range(ia, IntUSet_begin(ia), IntUSet_end(ia)));
	assert(IntUSet_size(ia) == 0);
	assert(IntUSet_insert(ia, hoge_int[0], NULL));
	assert(IntUSet_size(ia) == 1);
	p = IntUSet_next(IntUSet_begin(ia));
	assert(p == IntUSet_erase_range(ia, IntUSet_begin(ia), IntUSet_next(IntUSet_begin(ia))));
	assert(IntUSet_size(ia) == 0);
	/* equal_range */
	assert(IntUSet_insert(ia, 100, NULL));
	assert(IntUSet_insert(ia, 110, NULL));
	assert(IntUSet_size(ia) == 2);
	IntUSet_equal_range(ia, 100, &p, &q);
	assert(q == IntUSet_erase_range(ia, p, q));
	assert(IntUSet_size(ia) == 1);
	IntUSet_equal_range(ia, 110, &p, &q);
	assert(q == IntUSet_erase_range(ia, p, q));
	assert(IntUSet_size(ia) == 0);
	/* erase_key */
	for (i = 0; i < SIZE/2; i++) {
		pos[i] = IntUSet_insert(ia, hoge_int[i], NULL);
		assert(pos[i]);
	}
	assert(IntUSet_size(ia) == SIZE/2);
	for (i = 0; i < SIZE/2; i++) {
		assert(IntUSet_erase_key(ia, hoge_int[i]) == 1);
		assert(IntUSet_verify(ia));
	}
	assert(IntUSet_size(ia) == 0);
	/* 大量にinsert */
	count = 0;
	while (count < 1000000 && IntUSet_insert(ia, count, NULL)) {
		count++;
	}
	assert(IntUSet_size(ia) == count);
	printf("bc:%d, lf:%g, mlf:%g\n", IntUSet_bucket_count(ia), IntUSet_load_factor(ia), IntUSet_get_max_load_factor(ia));
	printf("count: %d\n", count);
	printf("size: %d\n", IntUSet_size(ia));
	/* clear */
	IntUSet_clear(ia);
	printf("size: %d\n", IntUSet_size(ia));
	assert(IntUSet_size(ia) == 0);
	assert(IntUSet_insert(ia, 100, NULL));
	assert(IntUSet_size(ia) == 1);
	IntUSet_clear(ia);
	assert(IntUSet_size(ia) == 0);
	IntUSet_clear(ia);
	assert(IntUSet_size(ia) == 0);
	/* swap */
	for (i = 0; i < SIZE/2; i++) {
		pos[i] = IntUSet_insert(ia, hoge_int[i], &success[i]);
		assert(pos[i]);
		assert(success[i]);
	}
	assert(IntUSet_size(ia) == SIZE/2);
	x = IntUSet_new();
	for (i = 0; i < sizeof b / sizeof b[0]; i++) {
		pos[i] = IntUSet_insert(x, b[i], &success[i]);
		assert(pos[i]);
		assert(success[i]);
	}
	assert(IntUSet_size(x) == sizeof b / sizeof b[0]);
/*    IntUSet_print(ia);*/
/*    IntUSet_print(x);*/

	IntUSet_swap(ia, x);

	assert(IntUSet_verify(ia));
	assert(IntUSet_verify(x));
/*    IntUSet_print(ia);*/
/*    IntUSet_print(x);*/
	assert(IntUSet_size(x) == SIZE/2);
	assert(IntUSet_size(ia) == sizeof b / sizeof b[0]);
	/* insert_range */
	count = IntUSet_size(x);
	IntUSet_insert(x, b[0], 0);
	IntUSet_insert(ia, hoge_int[0], 0);
/*    IntUSet_print(ia);*/
/*    IntUSet_print(x);*/
	assert(IntUSet_insert_range(x, IntUSet_begin(ia), IntUSet_end(ia)));

/*    IntUSet_print(ia);*/
/*    IntUSet_print(x);*/
	assert(IntUSet_size(ia) == sizeof b / sizeof b[0] + 1);
	assert(IntUSet_size(x)  == count + sizeof b / sizeof b[0]);
	assert(IntUSet_verify(ia));
	assert(IntUSet_verify(x));

	assert(IntUSet_insert_range(x, IntUSet_begin(ia), IntUSet_end(ia)));
	assert(IntUSet_size(ia) == sizeof b / sizeof b[0] + 1);
	assert(IntUSet_size(x)  == count + sizeof b / sizeof b[0]);
	assert(IntUSet_verify(ia));
	assert(IntUSet_verify(x));

	POOL_DUMP_OVERFLOW(&pool);
	IntUSet_delete(ia);
	IntUSet_delete(x);
}


void USetTest_test_1_3(void)
{
	int i;
	size_t count = 0;
	IntUMSetIterator pos[SIZE];
	IntUMSetIterator p;
	IntUMSetIterator q;
	IntUMSet *x;
	int b[] = {100, 109, 101, 108, 102, 103, 104, 104, 105, 106, 107, 100, 101, 101, 108, 109};
	int flag[SIZE/2] = {0};
	printf("***** test_1_3 *****\n");
	ima = IntUMSet_new();
	/* 初期状態 */
	assert(IntUMSet_empty(ima));
	assert(IntUMSet_size(ima) == 0);
	assert(IntUMSet_begin(ima) == IntUMSet_end(ima));
	/* insert */
	for (i = 0; i < SIZE; i++) {
		pos[i] = IntUMSet_insert(ima, hoge_int[i]);
		assert(pos[i] && pos[i] != IntUMSet_end(ima));
		count++;
	}
/*    IntUMSet_print(ima);*/
	assert(!IntUMSet_empty(ima));
	assert(IntUMSet_size(ima) == count);
	assert(count == SIZE);
	/* count */
	for (i = 0; i < SIZE; i++) {
		if (i < SIZE/2/2) {
			assert(IntUMSet_count(ima, i) == 1);
		} else if (i < SIZE/2) {
			assert(IntUMSet_count(ima, i) == 3);
		} else {
			assert(IntUMSet_count(ima, i) == 0);
		}
	}
	/* find */
	for (i = 0; i < SIZE; i++) {
		if (IntUMSet_count(ima, hoge_int[i]) == 1) {
			assert(pos[i] == IntUMSet_find(ima, hoge_int[i]));
		} else if (IntUMSet_count(ima, hoge_int[i]) == 3) {
			if (!flag[hoge_int[i]]) {
				flag[hoge_int[i]] = 1;
			}
		} else {
			assert(0);
		}
	}
	/* begin, end, next, data */
	for (p = IntUMSet_begin(ima), i = 0; p != IntUMSet_end(ima); p = IntUMSet_next(p), i++) {
/*        printf("%d, %d\n", i, *IntUMSet_data(p));*/
	}
	assert(i == SIZE);
	/* erase */
	for (i = 0; i < SIZE; i++) {
		IntUMSetIterator itr = IntUMSet_next(pos[i]);
		assert(itr == IntUMSet_erase(ima, pos[i]));
		count--;
	}
	assert(IntUMSet_empty(ima));
	assert(IntUMSet_size(ima) == 0);
	assert(count == 0);
	/* erase_range */
	for (i = 0; i < SIZE; i++) {
		pos[i] = IntUMSet_insert(ima, hoge_int[i]);
		assert(pos[i] && pos[i] != IntUMSet_end(ima));
	}
/*    IntUMSet_print(ima);*/
	assert(IntUMSet_size(ima) == SIZE);
	IntUMSet_equal_range(ima, hoge_int[SIZE - 1], &p, &q);
	count = IntUMSet_count(ima, hoge_int[SIZE - 1]);
	assert(q == IntUMSet_erase_range(ima, p, q));
	assert(IntUMSet_size(ima) == SIZE - count);
/*    IntUMSet_print(ima);*/
	assert(IntUMSet_end(ima) == IntUMSet_erase_range(ima, IntUMSet_begin(ima), IntUMSet_end(ima)));
	assert(IntUMSet_size(ima) == 0);
	assert(IntUMSet_insert(ima, hoge_int[0]));
	assert(IntUMSet_size(ima) == 1);
	p = IntUMSet_next(IntUMSet_begin(ima));
	assert(p == IntUMSet_erase_range(ima, IntUMSet_begin(ima), IntUMSet_next(IntUMSet_begin(ima))));
	assert(IntUMSet_size(ima) == 0);
	/* equal_range */
	assert(IntUMSet_insert(ima, 100));
	assert(IntUMSet_insert(ima, 100));
	assert(IntUMSet_insert(ima, 100));
	assert(IntUMSet_insert(ima, 110));
	assert(IntUMSet_insert(ima, 110));
	assert(IntUMSet_size(ima) == 5);
	IntUMSet_equal_range(ima, 100, &p, &q);
	assert(q == IntUMSet_erase_range(ima, p, q));
	IntUMSet_equal_range(ima, 110, &p, &q);
	assert(q == IntUMSet_erase_range(ima, p, q));
	assert(IntUMSet_size(ima) == 0);
	/* erase_key */
	for (i = 0; i < SIZE; i++) {
		pos[i] = IntUMSet_insert(ima, hoge_int[i]);
		assert(pos[i] && pos[i] != IntUMSet_end(ima));
	}
	assert(IntUMSet_size(ima) == SIZE);
	for (i = 0; i < SIZE/2/2; i++) {
		assert(IntUMSet_erase_key(ima, i) == 1);
	}
	assert(IntUMSet_size(ima) == SIZE - SIZE/2/2);
	for (i = SIZE/2/2; i < SIZE/2; i++) {
		assert(IntUMSet_erase_key(ima, i) == 3);
	}
	assert(IntUMSet_size(ima) == 0);
	/* 大量にinsert */
	count = 0;
	while (count < 1000000 && IntUMSet_insert(ima, 0)) {
		count++;
	}
	assert(IntUMSet_size(ima) == count);
	printf("count: %d\n", count);

	IntUMSet_clear(ima);

	/* insert_range */
	for (i = 0; i < SIZE/2; i++) {
		pos[i] = IntUMSet_insert(ima, hoge_int[i]);
		assert(pos[i]);
	}
	assert(IntUMSet_size(ima) == SIZE/2);
	x = IntUMSet_new();
	for (i = 0; i < sizeof b / sizeof b[0]; i++) {
		pos[i] = IntUMSet_insert(x, b[i]);
		assert(pos[i]);
	}
	for (i = 0; i < SIZE/2; i++) {
		pos[i] = IntUMSet_insert(x, hoge_int[i]);
		assert(pos[i]);
	}
	assert(IntUMSet_size(x) == sizeof b / sizeof b[0] + SIZE/2);
/*    IntUMSet_print(ima);*/
/*    IntUMSet_print(x);*/
	assert(IntUMSet_insert_range(x, IntUMSet_begin(ima), IntUMSet_end(ima)));
	assert(IntUMSet_verify(ima));
	assert(IntUMSet_verify(x));
/*    IntUMSet_print(ima);*/
/*    IntUMSet_print(x);*/
	assert(IntUMSet_size(x) == sizeof b / sizeof b[0] + SIZE);


	POOL_DUMP_OVERFLOW(&pool);
	IntUMSet_delete(ima);
	IntUMSet_delete(x);
}


void USetTest_test_4_1(void)
{
	int i;
	int success[SIZE];
	size_t count = 0;
	StrUSetIterator pos[SIZE];
	StrUSetIterator p;
	StrUSetIterator q;
	printf("***** test_4_1 *****\n");
	sa = StrUSet_new();
	/* 初期状態 */
	assert(StrUSet_empty(sa));
	assert(StrUSet_size(sa) == 0);
	assert(StrUSet_begin(sa) == StrUSet_end(sa));
	/* insert */
	for (i = 0; i < SIZE; i++) {
		pos[i] = StrUSet_insert(sa, hoge_str[i], &success[i]);
		if (i < SIZE/2) {
			assert(success[i]);
			count++;
		} else {
			assert(!success[i]);
		}
		assert(pos[i]);
	}
/*    StrUSet_print(sa);*/
/*    assert(StrUSet_rehash(sa, 20));*/
/*    StrUSet_print(sa);*/
	assert(!StrUSet_empty(sa));
	assert(StrUSet_size(sa) == count);
	assert(count == SIZE/2);
	/* count, find */
	for (i = 0; i < SIZE/2; i++) {
		assert(StrUSet_count(sa, hoge_str[i]) == 1);
		assert(pos[i] == StrUSet_find(sa, hoge_str[i]));
		if (i != SIZE/2-1) {
		}
	}
	assert(StrUSet_find(sa, "hoge") == StrUSet_end(sa));
	/* begin, end, next, data */
	for (p = StrUSet_begin(sa), i = 0; p != StrUSet_end(sa); p = StrUSet_next(p), i++) {
/*        assert(*StrUSet_data(p) == hoge_str[i]);*/
	}
	assert(i == SIZE/2);
	/* erase */
	for (i = 0; i < SIZE; i++) {
		if (pos[i] && success[i]) {
			StrUSetIterator itr = StrUSet_next(pos[i]);
			assert(itr == StrUSet_erase(sa, pos[i]));
			count--;
		}
	}
	assert(StrUSet_empty(sa));
	assert(StrUSet_size(sa) == 0);
	assert(count == 0);
	/* erase_range */
	for (i = 0; i < SIZE/2; i++) {
		pos[i] = StrUSet_insert(sa, hoge_str[i], NULL);
		assert(pos[i]);
	}
	assert(StrUSet_size(sa) == SIZE/2);
	StrUSet_print(sa);
	StrUSet_equal_range(sa, hoge_str[SIZE - 1], &p, &q);
	count = StrUSet_count(sa, hoge_str[SIZE - 1]);
	assert(q == StrUSet_erase_range(sa, p, q));
	assert(StrUSet_size(sa) == SIZE/2 - count);
	StrUSet_print(sa);

	assert(StrUSet_end(sa) == StrUSet_erase_range(sa, StrUSet_begin(sa), StrUSet_end(sa)));
	assert(StrUSet_size(sa) == 0);
	assert(StrUSet_insert(sa, hoge_str[0], NULL));
	assert(StrUSet_size(sa) == 1);
	p = StrUSet_next(StrUSet_begin(sa));
	assert(p == StrUSet_erase_range(sa, StrUSet_begin(sa), StrUSet_next(StrUSet_begin(sa))));
	assert(StrUSet_size(sa) == 0);
	/* equal_range */
	assert(StrUSet_insert(sa, "100", NULL));
	assert(StrUSet_insert(sa, "110", NULL));
	assert(StrUSet_size(sa) == 2);
	StrUSet_equal_range(sa, "100", &p, &q);
	assert(q == StrUSet_erase_range(sa, p, q));
	assert(StrUSet_size(sa) == 1);
	StrUSet_equal_range(sa, "110", &p, &q);
	assert(q == StrUSet_erase_range(sa, p, q));
	assert(StrUSet_size(sa) == 0);
	/* erase_key */
	for (i = 0; i < SIZE/2; i++) {
		pos[i] = StrUSet_insert(sa, hoge_str[i], NULL);
		assert(pos[i]);
	}
	assert(StrUSet_size(sa) == SIZE/2);
	for (i = 0; i < SIZE/2; i++) {
		assert(StrUSet_erase_key(sa, hoge_str[i]) == 1);
	}
	assert(StrUSet_size(sa) == 0);
	/* clear */
	assert(StrUSet_insert(sa, "103", NULL));
	assert(StrUSet_insert(sa, "101", NULL));
	assert(StrUSet_insert(sa, "100", NULL));
	assert(StrUSet_insert(sa, "104", NULL));
	assert(StrUSet_insert(sa, "106", NULL));
	assert(StrUSet_insert(sa, "105", NULL));
	assert(StrUSet_insert(sa, "102", NULL));
	printf("size: %d\n", StrUSet_size(sa));
	StrUSet_clear(sa);
	printf("size: %d\n", StrUSet_size(sa));
	assert(StrUSet_size(sa) == 0);
	assert(StrUSet_insert(sa, "100", NULL));
	assert(StrUSet_size(sa) == 1);
	StrUSet_clear(sa);
	assert(StrUSet_size(sa) == 0);
	StrUSet_clear(sa);
	assert(StrUSet_size(sa) == 0);

	POOL_DUMP_OVERFLOW(&pool);
	StrUSet_delete(sa);
}




void USetTest_run(void)
{
	printf("\n===== unordered_set test =====\n");
	unordered_set_init_hoge();

	USetTest_test_1_1();
	USetTest_test_1_3();
	USetTest_test_4_1();
}


int main(void)
{
#ifdef MY_MALLOC
	Pool_init(&pool, buf, sizeof buf, sizeof buf[0]);
#endif
	USetTest_run();
#ifdef MY_MALLOC
	POOL_DUMP_LEAK(&pool, 0);
#endif
	return 0;
}
