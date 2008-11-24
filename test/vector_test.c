#include <stdio.h>
#include <string.h>
#include <assert.h>
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
	printf("***** test_1_1 *****\n");
	uv = UCharVector_new(SIZE);
	vector_init_hoge();
	/* 初期状態 */
	assert(UCharVector_capacity(uv) == SIZE);
	assert(UCharVector_size(uv) == 0);
	assert(UCharVector_empty(uv));
	/* insert_array */
	UCharVector_clear(uv);
	assert(UCharVector_insert_array(uv, 0, hoge, SIZE));
	assert(memcmp(UCharVector_at(uv, 0), hoge, SIZE) == 0);
	assert(UCharVector_capacity(uv) == SIZE);
	assert(UCharVector_size(uv) == SIZE);
	assert(!UCharVector_empty(uv));
	/* clear */
	UCharVector_clear(uv);
	assert(UCharVector_size(uv) == 0);
	assert(UCharVector_empty(uv));
	/* insert_array */
	UCharVector_clear(uv);
	assert(UCharVector_insert_array(uv, 0, &hoge[1], 255));
	assert(memcmp(UCharVector_at(uv, 0), &hoge[1], 255) == 0);
	a = UCharVector_capacity(uv);
	assert(a > SIZE);
	assert(UCharVector_size(uv) == 255);
	assert(!UCharVector_empty(uv));
	/* clear */
	UCharVector_clear(uv);
	assert(UCharVector_size(uv) == 0);
	assert(UCharVector_empty(uv));
	/* insert_array */
	UCharVector_clear(uv);
	assert(UCharVector_insert_array(uv, 0, hoge, SIZE));
	assert(memcmp(UCharVector_at(uv, 0), hoge, SIZE) == 0);
	assert(UCharVector_capacity(uv) == a);
	assert(UCharVector_size(uv) == SIZE);
	assert(!UCharVector_empty(uv));
	/* clear */
	UCharVector_clear(uv);
	assert(UCharVector_size(uv) == 0);
	assert(UCharVector_empty(uv));
	/* reserve */
	assert(UCharVector_reserve(uv, 1024));
	assert(UCharVector_capacity(uv) == 1024);
	assert(UCharVector_size(uv) == 0);
	assert(UCharVector_empty(uv));

	assert(UCharVector_reserve(uv, 256));
	assert(UCharVector_capacity(uv) == 1024);
	assert(UCharVector_size(uv) == 0);
	assert(UCharVector_empty(uv));

	POOL_DUMP_OVERFLOW(&pool);
	UCharVector_delete(uv);
}

void VectorTest_test_1_2(void)
{
	int i;
	size_t a;
	printf("***** test_1_2 *****\n");
	uv = UCharVector_new(SIZE);
	vector_init_hoge();
	/* push_back */
	for (i = 0; i < SIZE; i++) {
		assert(UCharVector_push_back(uv, hoge[i]));
		assert(UCharVector_size(uv) == i+1);
		assert(*UCharVector_front(uv) == hoge[0]);
		assert(*UCharVector_back(uv) == hoge[i]);
	}
	assert(memcmp(UCharVector_at(uv, 0), hoge, SIZE) == 0);
	assert(UCharVector_capacity(uv) == SIZE);
	assert(UCharVector_size(uv) == SIZE);
	assert(!UCharVector_empty(uv));
	for (i = i; i < 256; i++) {
		assert(UCharVector_push_back(uv, hoge[i]));
		assert(UCharVector_size(uv) == i+1);
		assert(*UCharVector_front(uv) == hoge[0]);
		assert(*UCharVector_back(uv) == hoge[i]);
	}
	assert(memcmp(UCharVector_at(uv, 0), hoge, 256) == 0);
	a = UCharVector_capacity(uv);
	assert(a > SIZE);
	assert(UCharVector_size(uv) == 256);
	assert(!UCharVector_empty(uv));
	/* front, back */
	assert(*UCharVector_front(uv) == hoge[0]);
	assert(*UCharVector_back(uv) == hoge[255]);
	/* pop_back */
	for (i = 255; i >= 0; i--) {
		assert(*UCharVector_front(uv) == hoge[0]);
		assert(*UCharVector_back(uv) == hoge[i]);
		UCharVector_pop_back(uv);
		assert(UCharVector_size(uv) == i);
	}
	assert(UCharVector_size(uv) == 0);
	assert(UCharVector_empty(uv));
	assert(UCharVector_capacity(uv) == a);

	POOL_DUMP_OVERFLOW(&pool);
	UCharVector_delete(uv);
}

void VectorTest_test_1_3(void)
{
	int i;
	printf("***** test_1_3 *****\n");
	uv = UCharVector_new(SIZE);
	vector_init_hoge();
	/* insert_array */
	UCharVector_clear(uv);
	assert(UCharVector_insert_array(uv, 0, hoge, SIZE));
	assert(memcmp(UCharVector_at(uv, 0), hoge, SIZE) == 0);
	assert(UCharVector_capacity(uv) == SIZE);
	assert(UCharVector_size(uv) == SIZE);
	assert(!UCharVector_empty(uv));
	/* at */
	for (i = 0; i < SIZE; i++) {
		assert(*UCharVector_at(uv, i) == hoge[i]);
		*UCharVector_at(uv, i) = hoge[i]+1;
		assert(*UCharVector_at(uv, i) == hoge[i]+1);
	}
	assert(UCharVector_capacity(uv) == SIZE);
	assert(UCharVector_size(uv) == SIZE);
	assert(!UCharVector_empty(uv));

	POOL_DUMP_OVERFLOW(&pool);
	UCharVector_delete(uv);
}

void VectorTest_test_1_4(void)
{
	int i;
	unsigned char buf[256];
	printf("***** test_1_4 *****\n");
	uv = UCharVector_new(SIZE);
	/* insert */
	for (i = 0; i < SIZE; i++) {
		assert(UCharVector_insert(uv, 0, hoge[i]));
		buf[i] = SIZE - i - 1;
	}
	assert(memcmp(UCharVector_at(uv, 0), buf, SIZE) == 0);
	assert(UCharVector_capacity(uv) == SIZE);
	assert(UCharVector_size(uv) == SIZE);
	assert(!UCharVector_empty(uv));
	for (i = SIZE; i < 256; i++) {
		assert(UCharVector_insert(uv, UCharVector_size(uv), hoge[i]));
		buf[i] = i;
		assert(UCharVector_capacity(uv) > SIZE);
	}
	assert(memcmp(&UCharVector_at(uv, 0)[SIZE], &buf[SIZE], 256-SIZE) == 0);
	assert(UCharVector_size(uv) == 256);
	assert(UCharVector_insert(uv, SIZE, 0));
	assert(memcmp(UCharVector_at(uv, 0), buf, SIZE) == 0);
	assert(UCharVector_at(uv, 0)[SIZE] == 0);
	assert(memcmp(&UCharVector_at(uv, 0)[SIZE+1], &buf[SIZE], 256-SIZE) == 0);
	assert(UCharVector_size(uv) == 257);
	/* erase */
	UCharVector_erase(uv, SIZE, 1);
	assert(memcmp(UCharVector_at(uv, 0), buf, SIZE) == 0);
	assert(memcmp(&UCharVector_at(uv, 0)[SIZE], &buf[SIZE], 256-SIZE) == 0);
	for (i = SIZE; i < 256; i++) {
		UCharVector_erase(uv, UCharVector_size(uv)-1, 1);
	}
	assert(memcmp(UCharVector_at(uv, 0), buf, SIZE) == 0);
	assert(UCharVector_size(uv) == SIZE);
	for (i = 0; i < SIZE; i++) {
		assert(memcmp(UCharVector_at(uv, 0), &buf[i], SIZE-i) == 0);
		UCharVector_erase(uv, 0, 1);
		assert(UCharVector_size(uv) == SIZE-i-1);
	}
	assert(UCharVector_size(uv) == 0);

	POOL_DUMP_OVERFLOW(&pool);
	UCharVector_delete(uv);
}

void VectorTest_test_1_5(void)
{
	printf("***** test_1_5 *****\n");
	uv = UCharVector_new(SIZE);
	vector_init_hoge();
	/* insert_array */
	assert(UCharVector_insert_array(uv, 0, hoge, SIZE));
	assert(memcmp(UCharVector_at(uv, 0), hoge, SIZE) == 0);
	assert(UCharVector_capacity(uv) == SIZE);
	assert(UCharVector_size(uv) == SIZE);

	assert(UCharVector_insert_array(uv, UCharVector_size(uv), &hoge[SIZE], 256-SIZE));
	assert(memcmp(UCharVector_at(uv, 0), hoge, 256) == 0);
	assert(UCharVector_capacity(uv) > SIZE);
	assert(UCharVector_size(uv) == 256);

	assert(UCharVector_insert_array(uv, SIZE, hoge, SIZE));
	assert(memcmp(UCharVector_at(uv, 0), hoge, SIZE) == 0);
	assert(memcmp(&UCharVector_at(uv, 0)[SIZE], hoge, 256) == 0);
	assert(UCharVector_size(uv) == 256+SIZE);

	assert(UCharVector_insert_array(uv, 0, hoge, 1));
	assert(*UCharVector_at(uv, 0) == hoge[0]);
	assert(memcmp(&UCharVector_at(uv, 0)[1], hoge, SIZE) == 0);
	assert(memcmp(&UCharVector_at(uv, 0)[SIZE+1], hoge, 256) == 0);
	assert(UCharVector_size(uv) == 257+SIZE);

	assert(UCharVector_insert_array(uv, 0, hoge, 0));
	assert(*UCharVector_at(uv, 0) == hoge[0]);
	assert(memcmp(&UCharVector_at(uv, 0)[1], hoge, SIZE) == 0);
	assert(memcmp(&UCharVector_at(uv, 0)[SIZE+1], hoge, 256) == 0);
	assert(UCharVector_size(uv) == 257+SIZE);
	/* erase */
	UCharVector_erase(uv, 0, 1);
	assert(memcmp(UCharVector_at(uv, 0), hoge, SIZE) == 0);
	assert(memcmp(&UCharVector_at(uv, 0)[SIZE], hoge, 256) == 0);
	assert(UCharVector_size(uv) == 256+SIZE);

	UCharVector_erase(uv, SIZE, SIZE);
	assert(memcmp(UCharVector_at(uv, 0), hoge, 256) == 0);
	assert(UCharVector_size(uv) == 256);

	UCharVector_erase(uv, SIZE, 256-SIZE);
	assert(memcmp(UCharVector_at(uv, 0), hoge, SIZE) == 0);
	assert(UCharVector_size(uv) == SIZE);

	UCharVector_erase(uv, 0, SIZE);
	assert(UCharVector_size(uv) == 0);

	POOL_DUMP_OVERFLOW(&pool);
	UCharVector_delete(uv);
}

void VectorTest_test_1_6(void)
{
	UCharVector *x;
	size_t i;
	unsigned char b[] = {1, 2, 4, 43, 2, 54, 1, 0, 2, 24};
	size_t cap1, cap2;
	printf("***** test_1_6 *****\n");
	uv = UCharVector_new(SIZE);
	vector_init_hoge();

	UCharVector_clear(uv);
	assert(UCharVector_insert_array(uv, 0, hoge, SIZE));
	assert(memcmp(UCharVector_at(uv, 0), hoge, SIZE) == 0);
	assert(UCharVector_size(uv) == SIZE);
	assert(UCharVector_capacity(uv) == SIZE);
	/* resize */
	assert(UCharVector_resize(uv, SIZE + 10, 255));
	assert(UCharVector_size(uv) == SIZE + 10);
	assert(memcmp(UCharVector_at(uv, 0), hoge, SIZE) == 0);
	assert(*UCharVector_at(uv, SIZE) == 255);
	assert(*UCharVector_at(uv, SIZE + 9) == 255);
	assert(UCharVector_capacity(uv) > SIZE);

	assert(UCharVector_resize(uv, SIZE - 10, 255));
	assert(UCharVector_size(uv) == SIZE - 10);

	assert(UCharVector_resize(uv, 0, 255));
	assert(UCharVector_empty(uv));
	/* shrink */
	assert(UCharVector_resize(uv, 1000, 255));
	assert(UCharVector_capacity(uv) == 1000);
	assert(UCharVector_size(uv) == 1000);
	assert(UCharVector_resize(uv, 100, 255));
	assert(UCharVector_capacity(uv) == 1000);
	assert(UCharVector_size(uv) == 100);
	UCharVector_shrink(uv, 500);
	assert(UCharVector_capacity(uv) == 500);
	assert(UCharVector_size(uv) == 100);
	UCharVector_shrink(uv, 0);
	assert(UCharVector_capacity(uv) == 100);
	assert(UCharVector_size(uv) == 100);
	assert(UCharVector_resize(uv, 10, 255));
	UCharVector_shrink(uv, 500);
	assert(UCharVector_capacity(uv) == 100);
	assert(UCharVector_size(uv) == 10);
	UCharVector_shrink(uv, 0);
	assert(UCharVector_capacity(uv) == 10);
	assert(UCharVector_size(uv) == 10);
	/* swap */
	x = UCharVector_new(SIZE);
	UCharVector_clear(uv);
	UCharVector_shrink(uv, 0);
	UCharVector_clear(uv);
	assert(UCharVector_insert_array(uv, 0, hoge, SIZE));
	assert(memcmp(UCharVector_at(uv, 0), hoge, SIZE) == 0);
	assert(UCharVector_size(uv) == SIZE);
	cap1 = UCharVector_capacity(uv);
	UCharVector_clear(x);
	UCharVector_shrink(x, 0);
	UCharVector_clear(x);
	assert(UCharVector_insert_array(x, 0, b, sizeof b));
	assert(memcmp(UCharVector_at(x, 0), b, sizeof b) == 0);
	assert(UCharVector_size(x) == sizeof b);
	cap2 = UCharVector_capacity(x);

	UCharVector_swap(uv, x);

	assert(memcmp(UCharVector_at(uv, 0), b, sizeof b) == 0);
	assert(UCharVector_size(uv) == sizeof b);
	assert(UCharVector_capacity(uv) == cap2);
	assert(memcmp(UCharVector_at(x, 0), hoge, SIZE) == 0);
	assert(UCharVector_size(x) == SIZE);
	assert(UCharVector_capacity(x) == cap1);
	/* insert_range */
	UCharVector_clear(uv);
	UCharVector_shrink(uv, 0);
	UCharVector_clear(x);
	UCharVector_shrink(x, 0);
	assert(UCharVector_insert_array(uv, 0, hoge, SIZE));
	assert(UCharVector_insert_array(x, 0, b, sizeof b));
	assert(UCharVector_insert_range(uv, 0, x, 0, sizeof b));
	for (i = 0; i < UCharVector_size(uv); i++) {
		if (i < sizeof b) {
			assert(*UCharVector_at(uv, i) == *UCharVector_at(x, i));
		} else {
			assert(*UCharVector_at(uv, i) == hoge[i - sizeof b]);
		}
		printf("%02d ", *UCharVector_at(uv, i));
	}
	printf("\n");
	UCharVector_clear(uv);
	assert(UCharVector_insert_array(uv, 0, hoge, SIZE));
	for (i = 0; i < UCharVector_size(uv); i++) {
		printf("%02d ", *UCharVector_at(uv, i));
	}
	printf("\n");

	assert(UCharVector_insert_range(uv, 0, uv, 1, 4));
	for (i = 0; i < UCharVector_size(uv); i++) {
		if (i < 4) {
			assert(*UCharVector_at(uv, i) == hoge[1 + i]);
		} else {
			assert(*UCharVector_at(uv, i) == hoge[i - 4]);
		}
		printf("%02d ", *UCharVector_at(uv, i));
	}
	printf("\n");
	UCharVector_clear(uv);
	assert(UCharVector_insert_array(uv, 0, hoge, SIZE));
	assert(UCharVector_insert_range(uv, 3, uv, 1, 4));
	for (i = 0; i < UCharVector_size(uv); i++) {
		if (i < 3) {
			assert(*UCharVector_at(uv, i) == hoge[i]);
		} else if (3 <= i && i < 3 + 4) {
			assert(*UCharVector_at(uv, i) == hoge[1 + i - 3]);
		} else {
			assert(*UCharVector_at(uv, i) == hoge[i - 4]);
		}
		printf("%02d ", *UCharVector_at(uv, i));
	}
	printf("\n");
	UCharVector_clear(uv);
	assert(UCharVector_insert_array(uv, 0, hoge, SIZE));
	assert(UCharVector_insert_range(uv, 5, uv, 1, 4));
	for (i = 0; i < UCharVector_size(uv); i++) {
		if (i < 5) {
			assert(*UCharVector_at(uv, i) == hoge[i]);
		} else if (5 <= i && i < 5 + 4) {
			assert(*UCharVector_at(uv, i) == hoge[1 + i - 5]);
		} else {
			assert(*UCharVector_at(uv, i) == hoge[i - 4]);
		}
		printf("%02d ", *UCharVector_at(uv, i));
	}
	printf("\n");



	POOL_DUMP_OVERFLOW(&pool);
	UCharVector_delete(uv);
	UCharVector_delete(x);
}


void VectorTest_test_2_1(void)
{
	size_t a;
	printf("***** test_2_1 *****\n");
	iv = IntVector_new(SIZE);
	vector_init_piyo();
	/* 初期状態 */
	assert(IntVector_capacity(iv) == SIZE);
	assert(IntVector_size(iv) == 0);
	assert(IntVector_empty(iv));
	/* insert_array */
	IntVector_clear(iv);
	assert(IntVector_insert_array(iv, 0, piyo, SIZE));
	assert(memcmp(IntVector_at(iv, 0), piyo, SIZE) == 0);
	assert(IntVector_capacity(iv) == SIZE);
	assert(IntVector_size(iv) == SIZE);
	assert(!IntVector_empty(iv));
	/* clear */
	IntVector_clear(iv);
	assert(IntVector_size(iv) == 0);
	assert(IntVector_empty(iv));
	/* insert_array */
	IntVector_clear(iv);
	assert(IntVector_insert_array(iv, 0, &piyo[1], 255));
	assert(memcmp(IntVector_at(iv, 0), &piyo[1], 255) == 0);
	a = IntVector_capacity(iv);
	assert(a > SIZE);
	assert(IntVector_size(iv) == 255);
	assert(!IntVector_empty(iv));
	/* clear */
	IntVector_clear(iv);
	assert(IntVector_size(iv) == 0);
	assert(IntVector_empty(iv));
	/* insert_array */
	IntVector_clear(iv);
	assert(IntVector_insert_array(iv, 0, piyo, SIZE));
	assert(memcmp(IntVector_at(iv, 0), piyo, SIZE) == 0);
	assert(IntVector_capacity(iv) == a);
	assert(IntVector_size(iv) == SIZE);
	assert(!IntVector_empty(iv));
	/* clear */
	IntVector_clear(iv);
	assert(IntVector_size(iv) == 0);
	assert(IntVector_empty(iv));
	/* reserve */
	assert(IntVector_reserve(iv, 1024));
	assert(IntVector_capacity(iv) == 1024);
	assert(IntVector_size(iv) == 0);
	assert(IntVector_empty(iv));

	assert(IntVector_reserve(iv, 256));
	assert(IntVector_capacity(iv) == 1024);
	assert(IntVector_size(iv) == 0);
	assert(IntVector_empty(iv));

	POOL_DUMP_OVERFLOW(&pool);
	IntVector_delete(iv);
}

void VectorTest_test_2_2(void)
{
	int i;
	size_t a;
	printf("***** test_2_2 *****\n");
	iv = IntVector_new(SIZE);
	vector_init_piyo();
	/* push_back */
	for (i = 0; i < SIZE; i++) {
		assert(IntVector_push_back(iv, piyo[i]));
		assert(IntVector_size(iv) == i+1);
		assert(*IntVector_front(iv) == piyo[0]);
		assert(*IntVector_back(iv) == piyo[i]);
	}
	assert(memcmp(IntVector_at(iv, 0), piyo, SIZE) == 0);
	assert(IntVector_capacity(iv) == SIZE);
	assert(IntVector_size(iv) == SIZE);
	assert(!IntVector_empty(iv));
	for (i = i; i < 256; i++) {
		assert(IntVector_push_back(iv, piyo[i]));
		assert(IntVector_size(iv) == i+1);
		assert(*IntVector_front(iv) == piyo[0]);
		assert(*IntVector_back(iv) == piyo[i]);
	}
	assert(memcmp(IntVector_at(iv, 0), piyo, 256) == 0);
	a = IntVector_capacity(iv);
	assert(a > SIZE);
	assert(IntVector_size(iv) == 256);
	assert(!IntVector_empty(iv));
	/* front, back */
	assert(*IntVector_front(iv) == piyo[0]);
	assert(*IntVector_back(iv) == piyo[255]);
	/* pop_back */
	for (i = 255; i >= 0; i--) {
		assert(*IntVector_front(iv) == piyo[0]);
		assert(*IntVector_back(iv) == piyo[i]);
		IntVector_pop_back(iv);
		assert(IntVector_size(iv) == i);
	}
	assert(IntVector_size(iv) == 0);
	assert(IntVector_empty(iv));
	assert(IntVector_capacity(iv) == a);

	POOL_DUMP_OVERFLOW(&pool);
	IntVector_delete(iv);
}

void VectorTest_test_2_3(void)
{
	int i;
	printf("***** test_2_3 *****\n");
	iv = IntVector_new(SIZE);
	vector_init_piyo();
	/* insert_array */
	IntVector_clear(iv);
	assert(IntVector_insert_array(iv, 0, piyo, SIZE));
	assert(memcmp(IntVector_at(iv, 0), piyo, SIZE) == 0);
	assert(IntVector_capacity(iv) == SIZE);
	assert(IntVector_size(iv) == SIZE);
	assert(!IntVector_empty(iv));
	/* at */
	for (i = 0; i < SIZE; i++) {
		assert(*IntVector_at(iv, i) == piyo[i]);
		*IntVector_at(iv, i) = piyo[i]+1;
		assert(*IntVector_at(iv, i) == piyo[i]+1);
	}
	assert(IntVector_capacity(iv) == SIZE);
	assert(IntVector_size(iv) == SIZE);
	assert(!IntVector_empty(iv));

	POOL_DUMP_OVERFLOW(&pool);
	IntVector_delete(iv);
}

void VectorTest_test_2_4(void)
{
	int i;
	int buf[256];
	printf("***** test_2_4 *****\n");
	iv = IntVector_new(SIZE);
	/* insert */
	for (i = 0; i < SIZE; i++) {
		assert(IntVector_insert(iv, 0, piyo[i]));
		buf[i] = SIZE - i - 1;
	}
	assert(memcmp(IntVector_at(iv, 0), buf, SIZE) == 0);
	assert(IntVector_capacity(iv) == SIZE);
	assert(IntVector_size(iv) == SIZE);
	assert(!IntVector_empty(iv));
	for (i = SIZE; i < 256; i++) {
		assert(IntVector_insert(iv, IntVector_size(iv), piyo[i]));
		buf[i] = i;
		assert(IntVector_capacity(iv) > SIZE);
	}
	assert(memcmp(&IntVector_at(iv, 0)[SIZE], &buf[SIZE], 256-SIZE) == 0);
	assert(IntVector_size(iv) == 256);
	assert(IntVector_insert(iv, SIZE, 0));
	assert(memcmp(IntVector_at(iv, 0), buf, SIZE) == 0);
	assert(IntVector_at(iv, 0)[SIZE] == 0);
	assert(memcmp(&IntVector_at(iv, 0)[SIZE+1], &buf[SIZE], 256-SIZE) == 0);
	assert(IntVector_size(iv) == 257);
	/* erase */
	IntVector_erase(iv, SIZE, 1);
	assert(memcmp(IntVector_at(iv, 0), buf, SIZE) == 0);
	assert(memcmp(&IntVector_at(iv, 0)[SIZE], &buf[SIZE], 256-SIZE) == 0);
	for (i = SIZE; i < 256; i++) {
		IntVector_erase(iv, IntVector_size(iv)-1, 1);
	}
	assert(memcmp(IntVector_at(iv, 0), buf, SIZE) == 0);
	assert(IntVector_size(iv) == SIZE);
	for (i = 0; i < SIZE; i++) {
		assert(memcmp(IntVector_at(iv, 0), &buf[i], SIZE-i) == 0);
		IntVector_erase(iv, 0, 1);
		assert(IntVector_size(iv) == SIZE-i-1);
	}
	assert(IntVector_size(iv) == 0);

	POOL_DUMP_OVERFLOW(&pool);
	IntVector_delete(iv);
}

void VectorTest_test_2_5(void)
{
	printf("***** test_2_5 *****\n");
	iv = IntVector_new(SIZE);
	vector_init_piyo();
	/* insert_array */
	assert(IntVector_insert_array(iv, 0, piyo, SIZE));
	assert(memcmp(IntVector_at(iv, 0), piyo, SIZE) == 0);
	assert(IntVector_capacity(iv) == SIZE);
	assert(IntVector_size(iv) == SIZE);

	assert(IntVector_insert_array(iv, IntVector_size(iv), &piyo[SIZE], 256-SIZE));
	assert(memcmp(IntVector_at(iv, 0), piyo, 256) == 0);
	assert(IntVector_capacity(iv) > SIZE);
	assert(IntVector_size(iv) == 256);

	assert(IntVector_insert_array(iv, SIZE, piyo, SIZE));
	assert(memcmp(IntVector_at(iv, 0), piyo, SIZE) == 0);
	assert(memcmp(&IntVector_at(iv, 0)[SIZE], piyo, 256) == 0);
	assert(IntVector_size(iv) == 256+SIZE);

	assert(IntVector_insert_array(iv, 0, piyo, 1));
	assert(*IntVector_at(iv, 0) == piyo[0]);
	assert(memcmp(&IntVector_at(iv, 0)[1], piyo, SIZE) == 0);
	assert(memcmp(&IntVector_at(iv, 0)[SIZE+1], piyo, 256) == 0);
	assert(IntVector_size(iv) == 257+SIZE);
	/* erase */
	IntVector_erase(iv, 0, 1);
	assert(memcmp(IntVector_at(iv, 0), piyo, SIZE) == 0);
	assert(memcmp(&IntVector_at(iv, 0)[SIZE], piyo, 256) == 0);
	assert(IntVector_size(iv) == 256+SIZE);

	IntVector_erase(iv, SIZE, SIZE);
	assert(memcmp(IntVector_at(iv, 0), piyo, 256) == 0);
	assert(IntVector_size(iv) == 256);

	IntVector_erase(iv, SIZE, 256-SIZE);
	assert(memcmp(IntVector_at(iv, 0), piyo, SIZE) == 0);
	assert(IntVector_size(iv) == SIZE);

	IntVector_erase(iv, 0, SIZE);
	assert(IntVector_size(iv) == 0);

	POOL_DUMP_OVERFLOW(&pool);
	IntVector_delete(iv);
}




void VectorTest_run(void)
{
	printf("\n===== vector test =====\n");
	VectorTest_test_1_1();
	VectorTest_test_1_2();
	VectorTest_test_1_3();
	VectorTest_test_1_4();
	VectorTest_test_1_5();
	VectorTest_test_1_6();
	VectorTest_test_2_1();
	VectorTest_test_2_2();
	VectorTest_test_2_3();
	VectorTest_test_2_4();
	VectorTest_test_2_5();
}


int main(void)
{
#ifdef MY_MALLOC
	Pool_init(&pool, buf, sizeof buf, sizeof buf[0]);
#endif
	VectorTest_run();
#ifdef MY_MALLOC
	POOL_DUMP_LEAK(&pool, 0);
#endif
	return 0;
}
