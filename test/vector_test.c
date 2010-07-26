#include <stdio.h>
#include <string.h>
#include "../unittest/UnitTest.h"
#include "../cstl/vector.h"
#include "Pool.h"
#ifdef MY_MALLOC
double buf[1024*1024/sizeof(double)];
Pool pool;
#define malloc(s)		Pool_malloc(&pool, s)
#define realloc(p, s)	Pool_realloc(&pool, p, s)
#define free(p)			Pool_free(&pool, p)
#endif

/* vector */
#ifdef CSTLGEN
#include "UCharVector.h"
#include "IntVector.h"
#else

/*#undef CSTL_ASSERT*/
/*#define CSTL_ASSERT	ASSERT_FATAL*/

CSTL_VECTOR_INTERFACE(UCharVector, unsigned char)
CSTL_VECTOR_INTERFACE(IntVector, int)

CSTL_VECTOR_IMPLEMENT(UCharVector, unsigned char)
CSTL_VECTOR_IMPLEMENT(IntVector, int)
#endif

static UCharVector *uv;
static IntVector *iv;

#define SIZE	15


static unsigned char hoge[256];
static int piyo[256];

void vector_init_hoge(void)
{
	int i;
	for (i = 0; i < 256; i++) {
		hoge[i] = i;
	}
}

void vector_init_piyo(void)
{
	int i;
	for (i = 0; i < 256; i++) {
		piyo[i] = i;
	}
}


void VectorTest_test_1_1(void)
{
	size_t a;
	uv = UCharVector_new_reserve(SIZE);
	vector_init_hoge();
	/* 初期状態 */
	ASSERT(cstl_capacity(uv) == SIZE);
	ASSERT(cstl_size(uv) == 0);
	ASSERT(cstl_empty(uv));
	/* insert_array */
	cstl_clear(uv);
	ASSERT(cstl_insert_array(uv, cstl_begin(uv), hoge, SIZE));
	ASSERT(memcmp(cstl_at(uv, 0), hoge, SIZE) == 0);
	ASSERT(cstl_capacity(uv) == SIZE);
	ASSERT(cstl_size(uv) == SIZE);
	ASSERT(!cstl_empty(uv));
	/* clear */
	cstl_clear(uv);
	ASSERT(cstl_size(uv) == 0);
	ASSERT(cstl_empty(uv));
	/* insert_array */
	cstl_clear(uv);
	ASSERT(cstl_insert_array(uv, cstl_begin(uv), &hoge[1], 255));
	ASSERT(memcmp(cstl_at(uv, 0), &hoge[1], 255) == 0);
	a = cstl_capacity(uv);
	ASSERT(a > SIZE);
	ASSERT(cstl_size(uv) == 255);
	ASSERT(!cstl_empty(uv));
	/* clear */
	cstl_clear(uv);
	ASSERT(cstl_size(uv) == 0);
	ASSERT(cstl_empty(uv));
	/* insert_array */
	cstl_clear(uv);
	ASSERT(cstl_insert_array(uv, cstl_begin(uv), hoge, SIZE));
	ASSERT(memcmp(cstl_at(uv, 0), hoge, SIZE) == 0);
	ASSERT(cstl_capacity(uv) == a);
	ASSERT(cstl_size(uv) == SIZE);
	ASSERT(!cstl_empty(uv));
	/* clear */
	cstl_clear(uv);
	ASSERT(cstl_size(uv) == 0);
	ASSERT(cstl_empty(uv));
	/* reserve */
	ASSERT(cstl_reserve(uv, 1024));
	ASSERT(cstl_capacity(uv) == 1024);
	ASSERT(cstl_size(uv) == 0);
	ASSERT(cstl_empty(uv));

	ASSERT(cstl_reserve(uv, 256));
	ASSERT(cstl_capacity(uv) == 1024);
	ASSERT(cstl_size(uv) == 0);
	ASSERT(cstl_empty(uv));

	POOL_DUMP_OVERFLOW(&pool);
	cstl_delete(uv);

	/* new */
	uv = UCharVector_new();
	ASSERT(cstl_size(uv) == 0);
	ASSERT(cstl_empty(uv));
	cstl_delete(uv);
}

void VectorTest_test_1_2(void)
{
	int i;
	size_t a;
	uv = UCharVector_new_reserve(SIZE);
	vector_init_hoge();
	/* push_back */
	for (i = 0; i < SIZE; i++) {
		ASSERT(cstl_push_back(uv, hoge[i]));
		ASSERT(cstl_size(uv) == i+1);
		ASSERT(*cstl_front(uv) == hoge[0]);
		ASSERT(*cstl_back(uv) == hoge[i]);
	}
	ASSERT(memcmp(cstl_at(uv, 0), hoge, SIZE) == 0);
	ASSERT(cstl_capacity(uv) == SIZE);
	ASSERT(cstl_size(uv) == SIZE);
	ASSERT(!cstl_empty(uv));
	for (i = i; i < 256; i++) {
		ASSERT(cstl_push_back(uv, hoge[i]));
		ASSERT(cstl_size(uv) == i+1);
		ASSERT(*cstl_front(uv) == hoge[0]);
		ASSERT(*cstl_back(uv) == hoge[i]);
	}
	ASSERT(memcmp(cstl_at(uv, 0), hoge, 256) == 0);
	a = cstl_capacity(uv);
	ASSERT(a > SIZE);
	ASSERT(cstl_size(uv) == 256);
	ASSERT(!cstl_empty(uv));
	/* front, back */
	ASSERT(*cstl_front(uv) == hoge[0]);
	ASSERT(*cstl_back(uv) == hoge[255]);
	/* pop_back */
	for (i = 255; i >= 0; i--) {
		ASSERT(*cstl_front(uv) == hoge[0]);
		ASSERT(*cstl_back(uv) == hoge[i]);
		cstl_pop_back(uv);
		ASSERT(cstl_size(uv) == i);
	}
	ASSERT(cstl_size(uv) == 0);
	ASSERT(cstl_empty(uv));
	ASSERT(cstl_capacity(uv) == a);

	POOL_DUMP_OVERFLOW(&pool);
	cstl_delete(uv);
}

void VectorTest_test_1_3(void)
{
	int i;
	uv = UCharVector_new_reserve(SIZE);
	vector_init_hoge();
	/* insert_array */
	cstl_clear(uv);
	ASSERT(cstl_insert_array(uv, cstl_begin(uv), hoge, SIZE));
	ASSERT(memcmp(cstl_at(uv, 0), hoge, SIZE) == 0);
	ASSERT(cstl_capacity(uv) == SIZE);
	ASSERT(cstl_size(uv) == SIZE);
	ASSERT(!cstl_empty(uv));
	/* at */
	for (i = 0; i < SIZE; i++) {
		ASSERT(*cstl_at(uv, i) == hoge[i]);
		*cstl_at(uv, i) = hoge[i]+1;
		ASSERT(*cstl_at(uv, i) == hoge[i]+1);
	}
	ASSERT(cstl_capacity(uv) == SIZE);
	ASSERT(cstl_size(uv) == SIZE);
	ASSERT(!cstl_empty(uv));

	POOL_DUMP_OVERFLOW(&pool);
	cstl_delete(uv);
}

void VectorTest_test_1_4(void)
{
	int i;
	unsigned char buf[256];
	UCharVectorIterator iter;
	uv = UCharVector_new_reserve(SIZE);
	/* insert */
	for (i = 0; i < SIZE; i++) {
		ASSERT(cstl_insert(uv, cstl_begin(uv), hoge[i], NULL));
		buf[i] = SIZE - i - 1;
	}
	ASSERT(memcmp(cstl_at(uv, 0), buf, SIZE) == 0);
	ASSERT(cstl_capacity(uv) == SIZE);
	ASSERT(cstl_size(uv) == SIZE);
	ASSERT(!cstl_empty(uv));
	for (i = SIZE; i < 256; i++) {
/*        ASSERT(cstl_insert(uv, cstl_size(uv), hoge[i]));*/
		ASSERT(cstl_insert(uv, cstl_end(uv), hoge[i], NULL));
		buf[i] = i;
		ASSERT(cstl_capacity(uv) > SIZE);
	}
	ASSERT(memcmp(&cstl_at(uv, 0)[SIZE], &buf[SIZE], 256-SIZE) == 0);
	ASSERT(cstl_size(uv) == 256);
/*    ASSERT(cstl_insert(uv, SIZE, 0, NULL));*/
	ASSERT(cstl_insert(uv, cstl_iter_add(cstl_begin(uv), SIZE), 0, NULL));
	ASSERT(memcmp(cstl_at(uv, 0), buf, SIZE) == 0);
	ASSERT(cstl_at(uv, 0)[SIZE] == 0);
	ASSERT(memcmp(&cstl_at(uv, 0)[SIZE+1], &buf[SIZE], 256-SIZE) == 0);
	ASSERT(cstl_size(uv) == 257);
	/* erase */
/*    cstl_erase(uv, SIZE, 1);*/
	iter = cstl_begin(uv);
	cstl_erase(uv, cstl_iter_add(iter, SIZE));
	ASSERT(memcmp(cstl_at(uv, 0), buf, SIZE) == 0);
	ASSERT(memcmp(&cstl_at(uv, 0)[SIZE], &buf[SIZE], 256-SIZE) == 0);
	for (i = SIZE; i < 256; i++) {
/*        cstl_erase(uv, cstl_size(uv)-1, 1);*/
		iter = cstl_end(uv);
		cstl_erase(uv, cstl_iter_prev(iter));
	}
	ASSERT(memcmp(cstl_at(uv, 0), buf, SIZE) == 0);
	ASSERT(cstl_size(uv) == SIZE);
	for (i = 0; i < SIZE; i++) {
		ASSERT(memcmp(cstl_at(uv, 0), &buf[i], SIZE-i) == 0);
/*        cstl_erase(uv, 0, 1);*/
		cstl_erase(uv, cstl_begin(uv));
		ASSERT(cstl_size(uv) == SIZE-i-1);
	}
	ASSERT(cstl_size(uv) == 0);

	POOL_DUMP_OVERFLOW(&pool);
	cstl_delete(uv);
}

void VectorTest_test_1_5(void)
{
	UCharVectorIterator iter;
	uv = UCharVector_new_reserve(SIZE);
	vector_init_hoge();
	/* insert_array */
	ASSERT(cstl_insert_array(uv, cstl_begin(uv), hoge, SIZE));
	ASSERT(memcmp(cstl_at(uv, 0), hoge, SIZE) == 0);
	ASSERT(cstl_capacity(uv) == SIZE);
	ASSERT(cstl_size(uv) == SIZE);

	ASSERT(cstl_insert_array(uv, cstl_end(uv), &hoge[SIZE], 256-SIZE));
	ASSERT(memcmp(cstl_at(uv, 0), hoge, 256) == 0);
	ASSERT(cstl_capacity(uv) > SIZE);
	ASSERT(cstl_size(uv) == 256);

	ASSERT(cstl_insert_array(uv, cstl_iter_add(cstl_begin(uv), SIZE), hoge, SIZE));
	ASSERT(memcmp(cstl_at(uv, 0), hoge, SIZE) == 0);
	ASSERT(memcmp(&cstl_at(uv, 0)[SIZE], hoge, 256) == 0);
	ASSERT(cstl_size(uv) == 256+SIZE);

	ASSERT(cstl_insert_array(uv, cstl_begin(uv), hoge, 1));
	ASSERT(*cstl_at(uv, 0) == hoge[0]);
	ASSERT(memcmp(&cstl_at(uv, 0)[1], hoge, SIZE) == 0);
	ASSERT(memcmp(&cstl_at(uv, 0)[SIZE+1], hoge, 256) == 0);
	ASSERT(cstl_size(uv) == 257+SIZE);

	ASSERT(cstl_insert_array(uv, cstl_begin(uv), hoge, 0));
	ASSERT(*cstl_at(uv, 0) == hoge[0]);
	ASSERT(memcmp(&cstl_at(uv, 0)[1], hoge, SIZE) == 0);
	ASSERT(memcmp(&cstl_at(uv, 0)[SIZE+1], hoge, 256) == 0);
	ASSERT(cstl_size(uv) == 257+SIZE);
	/* erase */
/*    cstl_erase(uv, 0, 1);*/
	cstl_erase(uv, cstl_begin(uv));
	ASSERT(memcmp(cstl_at(uv, 0), hoge, SIZE) == 0);
	ASSERT(memcmp(&cstl_at(uv, 0)[SIZE], hoge, 256) == 0);
	ASSERT(cstl_size(uv) == 256+SIZE);

/*    cstl_erase(uv, SIZE, SIZE);*/
	iter = cstl_begin(uv);
	iter = cstl_iter_add(iter, SIZE);
	cstl_erase_range(uv, iter, cstl_iter_add(iter, SIZE));
	ASSERT(memcmp(cstl_at(uv, 0), hoge, 256) == 0);
	ASSERT(cstl_size(uv) == 256);

/*    cstl_erase(uv, SIZE, 256-SIZE);*/
	iter = cstl_begin(uv);
	iter = cstl_iter_add(iter, SIZE);
	cstl_erase_range(uv, iter, cstl_iter_add(iter, 256-SIZE));
	ASSERT(memcmp(cstl_at(uv, 0), hoge, SIZE) == 0);
	ASSERT(cstl_size(uv) == SIZE);

/*    cstl_erase(uv, 0, SIZE);*/
	iter = cstl_begin(uv);
	cstl_erase_range(uv, iter, cstl_iter_add(iter, SIZE));
	ASSERT(cstl_size(uv) == 0);

	POOL_DUMP_OVERFLOW(&pool);
	cstl_delete(uv);
}

void VectorTest_test_1_6(void)
{
	UCharVector *x;
	UCharVectorIterator iter;
	size_t i;
	unsigned char b[] = {1, 2, 4, 43, 2, 54, 1, 0, 2, 24};
	size_t cap1, cap2;
	uv = UCharVector_new_reserve(SIZE);
	vector_init_hoge();

	cstl_clear(uv);
	ASSERT(cstl_insert_array(uv, cstl_begin(uv), hoge, SIZE));
	ASSERT(memcmp(cstl_at(uv, 0), hoge, SIZE) == 0);
	ASSERT(cstl_size(uv) == SIZE);
	ASSERT(cstl_capacity(uv) == SIZE);
	/* resize */
	ASSERT(cstl_resize(uv, SIZE + 10, 255));
	ASSERT(cstl_size(uv) == SIZE + 10);
	ASSERT(memcmp(cstl_at(uv, 0), hoge, SIZE) == 0);
	ASSERT(*cstl_at(uv, SIZE) == 255);
	ASSERT(*cstl_at(uv, SIZE + 9) == 255);
	ASSERT(cstl_capacity(uv) > SIZE);

	ASSERT(cstl_resize(uv, SIZE - 10, 255));
	ASSERT(cstl_size(uv) == SIZE - 10);

	ASSERT(cstl_resize(uv, 0, 255));
	ASSERT(cstl_empty(uv));
	/* shrink */
	ASSERT(cstl_resize(uv, 1000, 255));
	ASSERT(cstl_capacity(uv) == 1000);
	ASSERT(cstl_size(uv) == 1000);
	ASSERT(cstl_resize(uv, 100, 255));
	ASSERT(cstl_capacity(uv) == 1000);
	ASSERT(cstl_size(uv) == 100);
	cstl_shrink(uv, 500);
	ASSERT(cstl_capacity(uv) == 500);
	ASSERT(cstl_size(uv) == 100);
	cstl_shrink(uv, 0);
	ASSERT(cstl_capacity(uv) == 100);
	ASSERT(cstl_size(uv) == 100);
	ASSERT(cstl_resize(uv, 10, 255));
	cstl_shrink(uv, 500);
	ASSERT(cstl_capacity(uv) == 100);
	ASSERT(cstl_size(uv) == 10);
	cstl_shrink(uv, 0);
	ASSERT(cstl_capacity(uv) == 10);
	ASSERT(cstl_size(uv) == 10);
	/* swap */
	x = UCharVector_new_reserve(SIZE);
	cstl_clear(uv);
	cstl_shrink(uv, 0);
	cstl_clear(uv);
	ASSERT(cstl_insert_array(uv, cstl_begin(uv), hoge, SIZE));
	ASSERT(memcmp(cstl_at(uv, 0), hoge, SIZE) == 0);
	ASSERT(cstl_size(uv) == SIZE);
	cap1 = cstl_capacity(uv);
	cstl_clear(x);
	cstl_shrink(x, 0);
	cstl_clear(x);
	ASSERT(cstl_insert_array(x, cstl_begin(uv), b, sizeof b));
	ASSERT(memcmp(cstl_at(x, 0), b, sizeof b) == 0);
	ASSERT(cstl_size(x) == sizeof b);
	cap2 = cstl_capacity(x);

	cstl_swap(uv, x);

	ASSERT(memcmp(cstl_at(uv, 0), b, sizeof b) == 0);
	ASSERT(cstl_size(uv) == sizeof b);
	ASSERT(cstl_capacity(uv) == cap2);
	ASSERT(memcmp(cstl_at(x, 0), hoge, SIZE) == 0);
	ASSERT(cstl_size(x) == SIZE);
	ASSERT(cstl_capacity(x) == cap1);
	/* insert_range */
	cstl_clear(uv);
	cstl_shrink(uv, 0);
	cstl_clear(x);
	cstl_shrink(x, 0);
	ASSERT(cstl_insert_array(uv, cstl_begin(uv), hoge, SIZE));
	ASSERT(cstl_insert_array(x, cstl_begin(uv), b, sizeof b));
/*    ASSERT(cstl_insert_range(uv, 0, x, 0, sizeof b));*/
	ASSERT(cstl_insert_range(uv, cstl_begin(uv), cstl_begin(x), cstl_iter_add(cstl_begin(x), sizeof b)));
	for (i = 0; i < cstl_size(uv); i++) {
		if (i < sizeof b) {
			ASSERT(*cstl_at(uv, i) == *cstl_at(x, i));
		} else {
			ASSERT(*cstl_at(uv, i) == hoge[i - sizeof b]);
		}
		printf("%02d ", *cstl_at(uv, i));
	}
	printf("\n");
	cstl_clear(uv);
	ASSERT(cstl_insert_array(uv, cstl_begin(uv), hoge, SIZE));
	for (i = 0; i < cstl_size(uv); i++) {
		printf("%02d ", *cstl_at(uv, i));
	}
	printf("\n");

/*    ASSERT(cstl_insert_range(uv, 0, uv, 1, 4));*/
	iter = cstl_begin(uv);
	ASSERT(cstl_insert_range(uv, iter, cstl_iter_add(iter, 1), cstl_iter_add(iter, 5)));
	for (i = 0; i < cstl_size(uv); i++) {
		if (i < 4) {
			ASSERT(*cstl_at(uv, i) == hoge[1 + i]);
		} else {
			ASSERT(*cstl_at(uv, i) == hoge[i - 4]);
		}
		printf("%02d ", *cstl_at(uv, i));
	}
	printf("\n");
	cstl_clear(uv);
	ASSERT(cstl_insert_array(uv, cstl_begin(uv), hoge, SIZE));
/*    ASSERT(cstl_insert_range(uv, 3, uv, 1, 4));*/
	iter = cstl_begin(uv);
	ASSERT(cstl_insert_range(uv, cstl_iter_add(iter, 3), cstl_iter_add(iter, 1), cstl_iter_add(iter, 5)));
	for (i = 0; i < cstl_size(uv); i++) {
		if (i < 3) {
			ASSERT(*cstl_at(uv, i) == hoge[i]);
		} else if (3 <= i && i < 3 + 4) {
			ASSERT(*cstl_at(uv, i) == hoge[1 + i - 3]);
		} else {
			ASSERT(*cstl_at(uv, i) == hoge[i - 4]);
		}
		printf("%02d ", *cstl_at(uv, i));
	}
	printf("\n");
	cstl_clear(uv);
	ASSERT(cstl_insert_array(uv, cstl_begin(uv), hoge, SIZE));
/*    ASSERT(cstl_insert_range(uv, 5, uv, 1, 4));*/
	iter = cstl_begin(uv);
	ASSERT(cstl_insert_range(uv, cstl_iter_add(iter, 5), cstl_iter_add(iter, 1), cstl_iter_add(iter, 5)));
	for (i = 0; i < cstl_size(uv); i++) {
		if (i < 5) {
			ASSERT(*cstl_at(uv, i) == hoge[i]);
		} else if (5 <= i && i < 5 + 4) {
			ASSERT(*cstl_at(uv, i) == hoge[1 + i - 5]);
		} else {
			ASSERT(*cstl_at(uv, i) == hoge[i - 4]);
		}
		printf("%02d ", *cstl_at(uv, i));
	}
	printf("\n");



	POOL_DUMP_OVERFLOW(&pool);
	cstl_delete(uv);
	cstl_delete(x);
}


void VectorTest_test_2_1(void)
{
	size_t a;
	iv = IntVector_new_reserve(SIZE);
	vector_init_piyo();
	/* 初期状態 */
	ASSERT(cstl_capacity(iv) == SIZE);
	ASSERT(cstl_size(iv) == 0);
	ASSERT(cstl_empty(iv));
	/* insert_array */
	cstl_clear(iv);
	ASSERT(cstl_insert_array(iv, cstl_begin(iv), piyo, SIZE));
	ASSERT(memcmp(cstl_at(iv, 0), piyo, SIZE) == 0);
	ASSERT(cstl_capacity(iv) == SIZE);
	ASSERT(cstl_size(iv) == SIZE);
	ASSERT(!cstl_empty(iv));
	/* clear */
	cstl_clear(iv);
	ASSERT(cstl_size(iv) == 0);
	ASSERT(cstl_empty(iv));
	/* insert_array */
	cstl_clear(iv);
	ASSERT(cstl_insert_array(iv, cstl_begin(iv), &piyo[1], 255));
	ASSERT(memcmp(cstl_at(iv, 0), &piyo[1], 255) == 0);
	a = cstl_capacity(iv);
	ASSERT(a > SIZE);
	ASSERT(cstl_size(iv) == 255);
	ASSERT(!cstl_empty(iv));
	/* clear */
	cstl_clear(iv);
	ASSERT(cstl_size(iv) == 0);
	ASSERT(cstl_empty(iv));
	/* insert_array */
	cstl_clear(iv);
	ASSERT(cstl_insert_array(iv, cstl_begin(iv), piyo, SIZE));
	ASSERT(memcmp(cstl_at(iv, 0), piyo, SIZE) == 0);
	ASSERT(cstl_capacity(iv) == a);
	ASSERT(cstl_size(iv) == SIZE);
	ASSERT(!cstl_empty(iv));
	/* clear */
	cstl_clear(iv);
	ASSERT(cstl_size(iv) == 0);
	ASSERT(cstl_empty(iv));
	/* reserve */
	ASSERT(cstl_reserve(iv, 1024));
	ASSERT(cstl_capacity(iv) == 1024);
	ASSERT(cstl_size(iv) == 0);
	ASSERT(cstl_empty(iv));

	ASSERT(cstl_reserve(iv, 256));
	ASSERT(cstl_capacity(iv) == 1024);
	ASSERT(cstl_size(iv) == 0);
	ASSERT(cstl_empty(iv));

	POOL_DUMP_OVERFLOW(&pool);
	cstl_delete(iv);
}

void VectorTest_test_2_2(void)
{
	int i;
	size_t a;
	iv = IntVector_new_reserve(SIZE);
	vector_init_piyo();
	/* push_back */
	for (i = 0; i < SIZE; i++) {
		ASSERT(cstl_push_back(iv, piyo[i]));
		ASSERT(cstl_size(iv) == i+1);
		ASSERT(*cstl_front(iv) == piyo[0]);
		ASSERT(*cstl_back(iv) == piyo[i]);
	}
	ASSERT(memcmp(cstl_at(iv, 0), piyo, SIZE) == 0);
	ASSERT(cstl_capacity(iv) == SIZE);
	ASSERT(cstl_size(iv) == SIZE);
	ASSERT(!cstl_empty(iv));
	for (i = i; i < 256; i++) {
		ASSERT(cstl_push_back(iv, piyo[i]));
		ASSERT(cstl_size(iv) == i+1);
		ASSERT(*cstl_front(iv) == piyo[0]);
		ASSERT(*cstl_back(iv) == piyo[i]);
	}
	ASSERT(memcmp(cstl_at(iv, 0), piyo, 256) == 0);
	a = cstl_capacity(iv);
	ASSERT(a > SIZE);
	ASSERT(cstl_size(iv) == 256);
	ASSERT(!cstl_empty(iv));
	/* front, back */
	ASSERT(*cstl_front(iv) == piyo[0]);
	ASSERT(*cstl_back(iv) == piyo[255]);
	/* pop_back */
	for (i = 255; i >= 0; i--) {
		ASSERT(*cstl_front(iv) == piyo[0]);
		ASSERT(*cstl_back(iv) == piyo[i]);
		cstl_pop_back(iv);
		ASSERT(cstl_size(iv) == i);
	}
	ASSERT(cstl_size(iv) == 0);
	ASSERT(cstl_empty(iv));
	ASSERT(cstl_capacity(iv) == a);

	POOL_DUMP_OVERFLOW(&pool);
	cstl_delete(iv);
}

void VectorTest_test_2_3(void)
{
	int i;
	iv = IntVector_new_reserve(SIZE);
	vector_init_piyo();
	/* insert_array */
	cstl_clear(iv);
	ASSERT(cstl_insert_array(iv, cstl_begin(iv), piyo, SIZE));
	ASSERT(memcmp(cstl_at(iv, 0), piyo, SIZE) == 0);
	ASSERT(cstl_capacity(iv) == SIZE);
	ASSERT(cstl_size(iv) == SIZE);
	ASSERT(!cstl_empty(iv));
	/* at */
	for (i = 0; i < SIZE; i++) {
		ASSERT(*cstl_at(iv, i) == piyo[i]);
		*cstl_at(iv, i) = piyo[i]+1;
		ASSERT(*cstl_at(iv, i) == piyo[i]+1);
	}
	ASSERT(cstl_capacity(iv) == SIZE);
	ASSERT(cstl_size(iv) == SIZE);
	ASSERT(!cstl_empty(iv));

	POOL_DUMP_OVERFLOW(&pool);
	cstl_delete(iv);
}

void VectorTest_test_2_4(void)
{
	int i;
	int buf[256];
	IntVectorIterator iter;
	iv = IntVector_new_reserve(SIZE);
	/* insert */
	for (i = 0; i < SIZE; i++) {
/*        ASSERT(cstl_insert(iv, 0, piyo[i]));*/
		ASSERT(cstl_insert(iv, cstl_begin(iv), piyo[i], NULL));
		buf[i] = SIZE - i - 1;
	}
	ASSERT(memcmp(cstl_at(iv, 0), buf, SIZE) == 0);
	ASSERT(cstl_capacity(iv) == SIZE);
	ASSERT(cstl_size(iv) == SIZE);
	ASSERT(!cstl_empty(iv));
	for (i = SIZE; i < 256; i++) {
/*        ASSERT(cstl_insert(iv, cstl_size(iv), piyo[i]));*/
		ASSERT(cstl_insert(iv, cstl_end(iv), piyo[i], NULL));
		buf[i] = i;
		ASSERT(cstl_capacity(iv) > SIZE);
	}
	ASSERT(memcmp(&cstl_at(iv, 0)[SIZE], &buf[SIZE], 256-SIZE) == 0);
	ASSERT(cstl_size(iv) == 256);
/*    ASSERT(cstl_insert(iv, SIZE, 0));*/
	ASSERT(cstl_insert(iv, cstl_iter_add(cstl_begin(iv), SIZE), 0, NULL));
	ASSERT(memcmp(cstl_at(iv, 0), buf, SIZE) == 0);
	ASSERT(cstl_at(iv, 0)[SIZE] == 0);
	ASSERT(memcmp(&cstl_at(iv, 0)[SIZE+1], &buf[SIZE], 256-SIZE) == 0);
	ASSERT(cstl_size(iv) == 257);
	/* erase */
/*    cstl_erase(iv, SIZE, 1);*/
	iter = cstl_begin(iv);
	cstl_erase(iv, cstl_iter_add(iter, SIZE));
	ASSERT(memcmp(cstl_at(iv, 0), buf, SIZE) == 0);
	ASSERT(memcmp(&cstl_at(iv, 0)[SIZE], &buf[SIZE], 256-SIZE) == 0);
	for (i = SIZE; i < 256; i++) {
/*        cstl_erase(iv, cstl_size(iv)-1, 1);*/
		iter = cstl_end(iv);
		cstl_erase(iv, cstl_iter_prev(iter));
	}
	ASSERT(memcmp(cstl_at(iv, 0), buf, SIZE) == 0);
	ASSERT(cstl_size(iv) == SIZE);
	for (i = 0; i < SIZE; i++) {
		ASSERT(memcmp(cstl_at(iv, 0), &buf[i], SIZE-i) == 0);
/*        cstl_erase(iv, 0, 1);*/
		cstl_erase(iv, cstl_begin(iv));
		ASSERT(cstl_size(iv) == SIZE-i-1);
	}
	ASSERT(cstl_size(iv) == 0);

	POOL_DUMP_OVERFLOW(&pool);
	cstl_delete(iv);
}

void VectorTest_test_2_5(void)
{
	IntVectorIterator iter;
	iv = IntVector_new_reserve(SIZE);
	vector_init_piyo();
	/* insert_array */
	ASSERT(cstl_insert_array(iv, cstl_begin(iv), piyo, SIZE));
	ASSERT(memcmp(cstl_at(iv, 0), piyo, SIZE) == 0);
	ASSERT(cstl_capacity(iv) == SIZE);
	ASSERT(cstl_size(iv) == SIZE);

	ASSERT(cstl_insert_array(iv, cstl_end(iv), &piyo[SIZE], 256-SIZE));
	ASSERT(memcmp(cstl_at(iv, 0), piyo, 256) == 0);
	ASSERT(cstl_capacity(iv) > SIZE);
	ASSERT(cstl_size(iv) == 256);

	ASSERT(cstl_insert_array(iv, cstl_iter_add(cstl_begin(iv), SIZE), piyo, SIZE));
	ASSERT(memcmp(cstl_at(iv, 0), piyo, SIZE) == 0);
	ASSERT(memcmp(&cstl_at(iv, 0)[SIZE], piyo, 256) == 0);
	ASSERT(cstl_size(iv) == 256+SIZE);

	ASSERT(cstl_insert_array(iv, cstl_begin(iv), piyo, 1));
	ASSERT(*cstl_at(iv, 0) == piyo[0]);
	ASSERT(memcmp(&cstl_at(iv, 0)[1], piyo, SIZE) == 0);
	ASSERT(memcmp(&cstl_at(iv, 0)[SIZE+1], piyo, 256) == 0);
	ASSERT(cstl_size(iv) == 257+SIZE);
	/* erase */
/*    cstl_erase(iv, 0, 1);*/
	cstl_erase(iv, cstl_begin(iv));
	ASSERT(memcmp(cstl_at(iv, 0), piyo, SIZE) == 0);
	ASSERT(memcmp(&cstl_at(iv, 0)[SIZE], piyo, 256) == 0);
	ASSERT(cstl_size(iv) == 256+SIZE);

/*    cstl_erase(iv, SIZE, SIZE);*/
	iter = cstl_begin(iv);
	iter = cstl_iter_add(iter, SIZE);
	cstl_erase_range(iv, iter, cstl_iter_add(iter, SIZE));
	ASSERT(memcmp(cstl_at(iv, 0), piyo, 256) == 0);
	ASSERT(cstl_size(iv) == 256);

/*    cstl_erase(iv, SIZE, 256-SIZE);*/
	iter = cstl_begin(iv);
	iter = cstl_iter_add(iter, SIZE);
	cstl_erase_range(iv, iter, cstl_iter_add(iter, 256-SIZE));
	ASSERT(memcmp(cstl_at(iv, 0), piyo, SIZE) == 0);
	ASSERT(cstl_size(iv) == SIZE);

/*    cstl_erase(iv, 0, SIZE);*/
	iter = cstl_begin(iv);
	cstl_erase_range(iv, iter, cstl_iter_add(iter, SIZE));
	ASSERT(cstl_size(iv) == 0);

	POOL_DUMP_OVERFLOW(&pool);
	cstl_delete(iv);
}


static TestCase vector_tests[] = {
	{ "VectorTest_test_1_1", VectorTest_test_1_1 },
	{ "VectorTest_test_1_2", VectorTest_test_1_2 },
	{ "VectorTest_test_1_3", VectorTest_test_1_3 },
	{ "VectorTest_test_1_4", VectorTest_test_1_4 },
	{ "VectorTest_test_1_5", VectorTest_test_1_5 },
	{ "VectorTest_test_1_6", VectorTest_test_1_6 },
	{ "VectorTest_test_2_1", VectorTest_test_2_1 },
	{ "VectorTest_test_2_2", VectorTest_test_2_2 },
	{ "VectorTest_test_2_3", VectorTest_test_2_3 },
	{ "VectorTest_test_2_4", VectorTest_test_2_4 },
	{ "VectorTest_test_2_5", VectorTest_test_2_5 },
	TEST_CASE_NULL,
};

static TestSuite suites[] = {
	{ "vector test", 0, 0, vector_tests },
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
