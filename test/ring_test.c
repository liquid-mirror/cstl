#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "../cstl/ring.h"
#include "heap.h"
#ifdef MY_MALLOC
double buf[1024*1024/sizeof(double)];
Heap heap;
#define malloc(s)		Heap_alloc(&heap, s)
#define realloc(p, s)	Heap_realloc(&heap, p, s)
#define free(p)			Heap_free(&heap, p)
#endif


/* ring */
#ifdef CSTLGEN
#include "UCharRing.h"
#include "IntRing.h"
#else

CSTL_RING_INTERFACE(UCharRing, unsigned char)
CSTL_RING_INTERFACE(IntRing, int)

CSTL_RING_IMPLEMENT(UCharRing, unsigned char)
CSTL_RING_IMPLEMENT(IntRing, int)
#endif

static UCharRing *ur;
static IntRing *ir;

#define MAX		255

static unsigned char hoge[MAX];
static int piyo[MAX];

void ring_init_hoge(void)
{
	int i;
	for (i = 0; i < MAX; i++) {
		hoge[i] = i;
	}
}

void ring_init_piyo(void)
{
	int i;
	for (i = 0; i < MAX; i++) {
		piyo[i] = i;
	}
}

int ur_cmp(UCharRing *x, size_t idx, unsigned char *buf, size_t n)
{
	int i, j;
	for (i = idx, j = 0; i < idx + n; i++, j++) {
/*        printf("%d, %d\n", *UCharRing_at(x, i), buf[j]);*/
		if (*UCharRing_at(x, i) != buf[j]) return 1;
	}
	return 0;
}

int ir_cmp(IntRing *x, size_t idx, int *buf, size_t n)
{
	int i, j;
	for (i = idx, j = 0; i < idx + n; i++, j++) {
/*        printf("%d, %d\n", *IntRing_at(x, i), buf[j]);*/
		if (*IntRing_at(x, i) != buf[j]) return 1;
	}
	return 0;
}


void RingTest_test_1_1(void)
{
	int i, j;
	printf("***** test_1_1 *****\n");
	ur = UCharRing_new(MAX);
	/* 初期状態 */
	assert(UCharRing_max_size(ur) == MAX);
	assert(UCharRing_size(ur) == 0);
	assert(UCharRing_empty(ur));
	assert(!UCharRing_full(ur));
	for (j = 0; j < MAX+1; j++) {
		ur->begin = ur->end = j;
		assert(UCharRing_size(ur) == 0);
		/* insert_array */
		UCharRing_clear(ur);
		assert(UCharRing_insert_array(ur, 0, hoge, MAX));
		assert(ur_cmp(ur, 0, hoge, MAX) == 0);
		assert(UCharRing_size(ur) == MAX);
		assert(!UCharRing_empty(ur));
		assert(UCharRing_full(ur));
		/* at */
		for (i = 0; i < UCharRing_size(ur); i++) {
			assert(*UCharRing_at(ur, i) == hoge[i]);
			*UCharRing_at(ur, i) = hoge[MAX - i -1];
			assert(*UCharRing_at(ur, i) == hoge[MAX - i -1]);
		}
		/* clear */
		UCharRing_clear(ur);
		assert(UCharRing_size(ur) == 0);
		assert(UCharRing_empty(ur));
		assert(!UCharRing_full(ur));
		/* insert_array */
		UCharRing_clear(ur);
		assert(UCharRing_insert_array(ur, 0, hoge, MAX/2));
		assert(ur_cmp(ur, 0, hoge, MAX/2) == 0);
		assert(UCharRing_size(ur) == MAX/2);
		assert(!UCharRing_empty(ur));
		assert(!UCharRing_full(ur));
		/* clear */
		UCharRing_clear(ur);
		assert(UCharRing_size(ur) == 0);
	}

	HEAP_DUMP_OVERFLOW(&heap);
	UCharRing_delete(ur);
}

void RingTest_test_1_2(void)
{
	int i, j;
	size_t size;
	printf("***** test_1_2 *****\n");
	ur = UCharRing_new(MAX);
	for (j = 0; j < MAX+1; j++) {
		ur->begin = ur->end = j;
		assert(UCharRing_size(ur) == 0);
		/* push_back */
		/* MAXまで */
		for (i = 0; i < MAX; i++) {
			size = UCharRing_size(ur);
			assert(UCharRing_push_back(ur, hoge[i]));
			assert(UCharRing_size(ur) == size + 1);
		}
		assert(ur_cmp(ur, 0, hoge, MAX) == 0);
		assert(UCharRing_size(ur) == MAX);
		assert(!UCharRing_empty(ur));
		assert(UCharRing_full(ur));
		/* FULLで失敗 */
		assert(!UCharRing_push_back(ur, hoge[0]));
		assert(UCharRing_size(ur) == MAX);
		assert(!UCharRing_empty(ur));
		assert(UCharRing_full(ur));
		/* front */
		assert(UCharRing_front(ur) == hoge[0]);
		/* back */
		assert(UCharRing_back(ur) == hoge[MAX-1]);
		/* pop_front */
		size = UCharRing_size(ur);
		assert(UCharRing_pop_front(ur) == hoge[0]);
		assert(UCharRing_size(ur) == size - 1);
		/* push_back */
		size = UCharRing_size(ur);
		assert(UCharRing_push_back(ur, hoge[0]));
		assert(UCharRing_size(ur) == size + 1);
		assert(ur_cmp(ur, 0, &hoge[1], MAX-1) == 0);
		assert(ur_cmp(ur, MAX-1, hoge, 1) == 0);
		assert(UCharRing_size(ur) == MAX);
		assert(!UCharRing_empty(ur));
		assert(UCharRing_full(ur));
		/* pop_front */
		for (i = 0; i < MAX; i++) {
			size = UCharRing_size(ur);
			if (i < MAX-1) {
				assert(UCharRing_pop_front(ur) == hoge[i+1]);
			} else {
				assert(UCharRing_pop_front(ur) == hoge[0]);
			}
			assert(UCharRing_size(ur) == size - 1);
		}
		assert(UCharRing_size(ur) == 0);
		assert(UCharRing_empty(ur));
		assert(!UCharRing_full(ur));
	}

	HEAP_DUMP_OVERFLOW(&heap);
	UCharRing_delete(ur);
}

void RingTest_test_1_3(void)
{
	int i, j;
	size_t size;
	printf("***** test_1_3 *****\n");
	ur = UCharRing_new(MAX);
	for (j = 0; j < MAX+1; j++) {
		ur->begin = ur->end = j;
		assert(UCharRing_size(ur) == 0);
		/* push_front */
		/* MAXまで */
		for (i = 0; i < MAX; i++) {
			size = UCharRing_size(ur);
			assert(UCharRing_push_front(ur, hoge[MAX-i-1]));
			assert(UCharRing_size(ur) == size + 1);
		}
		assert(ur_cmp(ur, 0, hoge, MAX) == 0);
		assert(UCharRing_size(ur) == MAX);
		assert(!UCharRing_empty(ur));
		assert(UCharRing_full(ur));
		/* FULLで失敗 */
		assert(!UCharRing_push_front(ur, hoge[0]));
		assert(UCharRing_size(ur) == MAX);
		assert(!UCharRing_empty(ur));
		assert(UCharRing_full(ur));
		/* front */
		assert(UCharRing_front(ur) == hoge[0]);
		/* back */
		assert(UCharRing_back(ur) == hoge[MAX-1]);
		/* pop_back */
		size = UCharRing_size(ur);
		assert(UCharRing_pop_back(ur) == hoge[MAX-1]);
		assert(UCharRing_pop_back(ur) == hoge[MAX-2]);
		assert(UCharRing_size(ur) == size - 2);
		/* push_front */
		size = UCharRing_size(ur);
		assert(UCharRing_push_front(ur, hoge[MAX-1]));
		assert(UCharRing_push_front(ur, hoge[MAX-2]));
		assert(UCharRing_size(ur) == size + 2);
		assert(ur_cmp(ur, 0, &hoge[MAX-2], 2) == 0);
		assert(ur_cmp(ur, 2, hoge, MAX-2) == 0);
		assert(UCharRing_size(ur) == MAX);
		assert(!UCharRing_empty(ur));
		assert(UCharRing_full(ur));
		/* pop_back */
		for (i = MAX-3; i >= 0; i--) {
			size = UCharRing_size(ur);
			assert(UCharRing_pop_back(ur) == hoge[i]);
			assert(UCharRing_size(ur) == size - 1);
		}
		assert(UCharRing_pop_back(ur) == hoge[MAX-1]);
		assert(UCharRing_pop_back(ur) == hoge[MAX-2]);

		assert(UCharRing_size(ur) == 0);
		assert(UCharRing_empty(ur));
		assert(!UCharRing_full(ur));
	}

	HEAP_DUMP_OVERFLOW(&heap);
	UCharRing_delete(ur);
}

void RingTest_test_1_4(void)
{
	int i, j;
	printf("***** test_1_4 *****\n");
	ur = UCharRing_new(MAX);
	for (j = 0; j < MAX+1; j++) {
		ur->begin = ur->end = j;
		assert(UCharRing_size(ur) == 0);
		/* insert */
		/* 空状態 */
		for (i = 0; i < MAX/2; i++) {
			assert(ur_cmp(ur, 0, hoge, i) == 0);
			assert(UCharRing_insert(ur, i, hoge[i]));
		}
		/* 先頭・前寄り */
		assert(UCharRing_insert(ur, 0, hoge[0]));
		assert(UCharRing_insert(ur, 1, hoge[1]));
		assert(UCharRing_insert(ur, 2, hoge[2]));
		assert(ur_cmp(ur, 0, hoge, 3) == 0);
		assert(ur_cmp(ur, 3, hoge, MAX/2) == 0);
		assert(UCharRing_size(ur) == MAX/2 + 3);

		/* erase */
		/* 先頭・前寄り */
		UCharRing_erase(ur, 2, 1);
		assert(ur_cmp(ur, 0, hoge, 2) == 0);
		assert(ur_cmp(ur, 2, hoge, MAX/2) == 0);
		assert(UCharRing_size(ur) == MAX/2 + 2);
		UCharRing_erase(ur, 1, 1);
		assert(ur_cmp(ur, 0, hoge, 1) == 0);
		assert(ur_cmp(ur, 1, hoge, MAX/2) == 0);
		assert(UCharRing_size(ur) == MAX/2 + 1);
		UCharRing_erase(ur, 0, 1);
		assert(ur_cmp(ur, 0, hoge, MAX/2) == 0);
		assert(UCharRing_size(ur) == MAX/2);

		/* insert */
		/* 末尾・後ろ寄り */
		assert(UCharRing_insert(ur, MAX/2, hoge[0]));
		assert(UCharRing_insert(ur, MAX/2, hoge[1]));
		assert(UCharRing_insert(ur, MAX/2, hoge[2]));
		assert(ur_cmp(ur, 0, hoge, MAX/2) == 0);
		assert(*UCharRing_at(ur, MAX/2 + 0) == hoge[2]);
		assert(*UCharRing_at(ur, MAX/2 + 1) == hoge[1]);
		assert(*UCharRing_at(ur, MAX/2 + 2) == hoge[0]);

		/* erase */
		/* 末尾・後ろ寄り */
		UCharRing_erase(ur, MAX/2, 1);
		assert(ur_cmp(ur, 0, hoge, MAX/2) == 0);
		assert(*UCharRing_at(ur, MAX/2 + 0) == hoge[1]);
		assert(*UCharRing_at(ur, MAX/2 + 1) == hoge[0]);
		assert(UCharRing_size(ur) == MAX/2 + 2);
		UCharRing_erase(ur, MAX/2, 1);
		assert(ur_cmp(ur, 0, hoge, MAX/2) == 0);
		assert(*UCharRing_at(ur, MAX/2 + 0) == hoge[0]);
		assert(UCharRing_size(ur) == MAX/2 + 1);
		UCharRing_erase(ur, MAX/2, 1);
		assert(ur_cmp(ur, 0, hoge, MAX/2) == 0);
		assert(UCharRing_size(ur) == MAX/2);

		/* 先頭から全削除 */
		for (i = 0; i < MAX/2; i++) {
			assert(ur_cmp(ur, 0, &hoge[i], MAX/2 - i) == 0);
			UCharRing_erase(ur, 0, 1);
		}
		assert(UCharRing_size(ur) == 0);

		for (i = 0; i < MAX/2; i++) {
			assert(ur_cmp(ur, 0, hoge, i) == 0);
			assert(UCharRing_insert(ur, i, hoge[i]));
		}
		assert(UCharRing_size(ur) == MAX/2);

		/* 末尾から全削除 */
		for (i = 0; i < MAX/2; i++) {
			assert(ur_cmp(ur, 0, hoge, MAX/2 - i) == 0);
			UCharRing_erase(ur, UCharRing_size(ur) -1, 1);
		}
		assert(UCharRing_size(ur) == 0);

	}

	HEAP_DUMP_OVERFLOW(&heap);
	UCharRing_delete(ur);
}

void RingTest_test_1_5(void)
{
	int i;
	printf("***** test_1_5 *****\n");
	ur = UCharRing_new(MAX);
	for (i = 0; i < MAX+1; i++) {
		ur->begin = ur->end = i;
		assert(UCharRing_size(ur) == 0);
		/* insert_array */
		/* 空状態 */
		assert(UCharRing_insert_array(ur, 0, hoge, MAX/2));
		assert(ur_cmp(ur, 0, hoge, MAX/2) == 0);
		assert(UCharRing_size(ur) == MAX/2);

		/* insert_array */
		/* 先頭 */
		assert(UCharRing_insert_array(ur, 0, hoge, 3));
		assert(ur_cmp(ur, 0, hoge, 3) == 0);
		assert(ur_cmp(ur, 3, hoge, MAX/2) == 0);
		assert(UCharRing_size(ur) == MAX/2 + 3);
		/* erase */
		UCharRing_erase(ur, 0, 3);
		assert(ur_cmp(ur, 0, hoge, MAX/2) == 0);
		assert(UCharRing_size(ur) == MAX/2);

		/* insert_array */
		/* 前寄り */
		assert(UCharRing_insert_array(ur, 2, hoge, 3));
		assert(ur_cmp(ur, 0, hoge, 2) == 0);
		assert(ur_cmp(ur, 2, hoge, 3) == 0);
		assert(ur_cmp(ur, 5, &hoge[2], MAX/2 -2) == 0);
		assert(UCharRing_size(ur) == MAX/2 + 3);
		/* 0個指定 */
		assert(UCharRing_insert_array(ur, 1, &hoge[MAX/2], 0));
		assert(ur_cmp(ur, 0, hoge, 2) == 0);
		assert(ur_cmp(ur, 2, hoge, 3) == 0);
		assert(ur_cmp(ur, 5, &hoge[2], MAX/2 -2) == 0);
		assert(UCharRing_size(ur) == MAX/2 + 3);
		/* 1個指定 */
		assert(UCharRing_insert_array(ur, 1, &hoge[MAX-1], 1));
		assert(ur_cmp(ur, 0, &hoge[0], 1) == 0);
		assert(ur_cmp(ur, 1, &hoge[MAX-1], 1) == 0);
		assert(ur_cmp(ur, 2, &hoge[1], 1) == 0);
		assert(ur_cmp(ur, 3, &hoge[0], 3) == 0);
		assert(ur_cmp(ur, 6, &hoge[2], MAX/2 -2) == 0);
		assert(UCharRing_size(ur) == MAX/2 + 4);
		/* erase */
		/* 0個指定 */
		UCharRing_erase(ur, 1, 0);
		assert(ur_cmp(ur, 0, &hoge[0], 1) == 0);
		assert(ur_cmp(ur, 1, &hoge[MAX-1], 1) == 0);
		assert(ur_cmp(ur, 2, &hoge[1], 1) == 0);
		assert(ur_cmp(ur, 3, &hoge[0], 3) == 0);
		assert(ur_cmp(ur, 6, &hoge[2], MAX/2 -2) == 0);
		assert(UCharRing_size(ur) == MAX/2 + 4);
		/* 1個指定 */
		UCharRing_erase(ur, 1, 1);
		assert(ur_cmp(ur, 0, hoge, 2) == 0);
		assert(ur_cmp(ur, 2, hoge, 3) == 0);
		assert(ur_cmp(ur, 5, &hoge[2], MAX/2 -2) == 0);
		assert(UCharRing_size(ur) == MAX/2 + 3);
		/* 前寄り */
		UCharRing_erase(ur, 2, 3);
		assert(ur_cmp(ur, 0, hoge, MAX/2) == 0);
		assert(UCharRing_size(ur) == MAX/2);

		/* insert_array */
		/* 末尾 */
		assert(UCharRing_insert_array(ur, UCharRing_size(ur), hoge, 3));
		assert(ur_cmp(ur, 0, hoge, MAX/2) == 0);
		assert(ur_cmp(ur, MAX/2, hoge, 3) == 0);
		assert(UCharRing_size(ur) == MAX/2 + 3);
		/* erase */
		UCharRing_erase(ur, UCharRing_size(ur) - 3, 3);
		assert(ur_cmp(ur, 0, hoge, MAX/2) == 0);
		assert(UCharRing_size(ur) == MAX/2);

		/* insert_array */
		/* 後ろ寄り */
		assert(UCharRing_insert_array(ur, UCharRing_size(ur) - 2, hoge, 3));
		assert(ur_cmp(ur, 0, hoge, MAX/2 -2) == 0);
		assert(ur_cmp(ur, MAX/2 -2, hoge, 3) == 0);
		assert(ur_cmp(ur, MAX/2 +1, &hoge[MAX/2 -2], 2) == 0);
		assert(UCharRing_size(ur) == MAX/2 + 3);
		/* 0個指定 */
		assert(UCharRing_insert_array(ur, UCharRing_size(ur) - 1, &hoge[MAX-1], 0));
		assert(ur_cmp(ur, 0, hoge, MAX/2 -2) == 0);
		assert(ur_cmp(ur, MAX/2 -2, hoge, 3) == 0);
		assert(ur_cmp(ur, MAX/2 +1, &hoge[MAX/2 -2], 2) == 0);
		assert(UCharRing_size(ur) == MAX/2 + 3);
		/* 1個指定 */
		assert(UCharRing_insert_array(ur, UCharRing_size(ur) - 1, &hoge[MAX-1], 1));
		assert(ur_cmp(ur, 0, hoge, MAX/2 -2) == 0);
		assert(ur_cmp(ur, MAX/2 -2, hoge, 3) == 0);
		assert(ur_cmp(ur, MAX/2 +1, &hoge[MAX/2 -2], 1) == 0);
		assert(ur_cmp(ur, MAX/2 +2, &hoge[MAX-1], 1) == 0);
		assert(ur_cmp(ur, MAX/2 +3, &hoge[MAX/2 -1], 1) == 0);
		assert(UCharRing_size(ur) == MAX/2 + 4);
		/* erase */
		/* 0個指定 */
		UCharRing_erase(ur, UCharRing_size(ur) - 2, 0);
		assert(ur_cmp(ur, 0, hoge, MAX/2 -2) == 0);
		assert(ur_cmp(ur, MAX/2 -2, hoge, 3) == 0);
		assert(ur_cmp(ur, MAX/2 +1, &hoge[MAX/2 -2], 1) == 0);
		assert(ur_cmp(ur, MAX/2 +2, &hoge[MAX-1], 1) == 0);
		assert(ur_cmp(ur, MAX/2 +3, &hoge[MAX/2 -1], 1) == 0);
		assert(UCharRing_size(ur) == MAX/2 + 4);
		/* 1個指定 */
		UCharRing_erase(ur, UCharRing_size(ur) - 2, 1);
		assert(ur_cmp(ur, 0, hoge, MAX/2 -2) == 0);
		assert(ur_cmp(ur, MAX/2 -2, hoge, 3) == 0);
		assert(ur_cmp(ur, MAX/2 +1, &hoge[MAX/2 -2], 2) == 0);
		assert(UCharRing_size(ur) == MAX/2 + 3);
		/* 後ろ寄り */
		UCharRing_erase(ur, UCharRing_size(ur) - 5, 3);
		assert(ur_cmp(ur, 0, hoge, MAX/2) == 0);
		assert(UCharRing_size(ur) == MAX/2);

		/* MAX以上で失敗 */
		assert(!UCharRing_insert_array(ur, 0, hoge, MAX - MAX/2 +1));
		assert(ur_cmp(ur, 0, hoge, MAX/2) == 0);
		assert(UCharRing_size(ur) == MAX/2);
		/* MAXになるまで */
		assert(UCharRing_insert_array(ur, 0, hoge, MAX - MAX/2));
		assert(UCharRing_size(ur) == MAX);
		assert(UCharRing_full(ur));
		/* 先頭から全削除 */
		UCharRing_erase(ur, 0, UCharRing_size(ur));
		assert(UCharRing_size(ur) == 0);
	}

	HEAP_DUMP_OVERFLOW(&heap);
	UCharRing_delete(ur);
}

void RingTest_test_1_6(void)
{
	int i;
	UCharRing *x;
	unsigned char b[] = {1, 2, 4, 43, 2, 54, 1, 0, 2, 24};
	printf("***** test_1_6 *****\n");
	ur = UCharRing_new(MAX);
	for (i = 0; i < MAX; i++) {
		assert(UCharRing_push_back(ur, hoge[i]));
	}
	/* resize */
	UCharRing_clear(ur);
	assert(UCharRing_resize(ur, 64, 100));
	assert(UCharRing_size(ur) == 64);
	for (i = 0; i < UCharRing_size(ur); i++) {
		assert(*UCharRing_at(ur, i) == 100);
	}
	assert(UCharRing_resize(ur, 16, 99));
	assert(UCharRing_size(ur) == 16);
	for (i = 0; i < UCharRing_size(ur); i++) {
		assert(*UCharRing_at(ur, i) == 100);
	}
	assert(UCharRing_resize(ur, 32, 99));
	for (i = 0; i < UCharRing_size(ur); i++) {
		if (i < 16) {
			assert(*UCharRing_at(ur, i) == 100);
		} else {
			assert(*UCharRing_at(ur, i) == 99);
		}
	}
	assert(UCharRing_size(ur) == 32);
	assert(UCharRing_resize(ur, MAX+1, 99) == 0);
	assert(UCharRing_resize(ur, 0, 100));
	assert(UCharRing_size(ur) == 0);
	/* swap */
	x = UCharRing_new(MAX);
	UCharRing_clear(ur);
	assert(UCharRing_insert_array(ur, 0, hoge, MAX));
	assert(ur_cmp(ur, 0, hoge, MAX) == 0);
	assert(UCharRing_size(ur) == MAX);
	UCharRing_clear(x);
	assert(UCharRing_insert_array(x, 0, b, sizeof b));
	assert(ur_cmp(x, 0, b, sizeof b) == 0);
	assert(UCharRing_size(x) == sizeof b);

	UCharRing_swap(ur, x);

	assert(ur_cmp(ur, 0, b, sizeof b) == 0);
	assert(UCharRing_size(ur) == sizeof b);
	assert(ur_cmp(x, 0, hoge, MAX) == 0);
	assert(UCharRing_size(x) == MAX);


	HEAP_DUMP_OVERFLOW(&heap);
	UCharRing_delete(ur);
	UCharRing_delete(x);
}



void RingTest_test_2_1(void)
{
	int i, j;
	printf("***** test_2_1 *****\n");
	ir = IntRing_new(MAX);
	/* 初期状態 */
	assert(IntRing_max_size(ir) == MAX);
	assert(IntRing_size(ir) == 0);
	assert(IntRing_empty(ir));
	assert(!IntRing_full(ir));
	for (j = 0; j < MAX+1; j++) {
		ir->begin = ir->end = j;
		assert(IntRing_size(ir) == 0);
		/* insert_array */
		IntRing_clear(ir);
		assert(IntRing_insert_array(ir, 0, piyo, MAX));
		assert(ir_cmp(ir, 0, piyo, MAX) == 0);
		assert(IntRing_size(ir) == MAX);
		assert(!IntRing_empty(ir));
		assert(IntRing_full(ir));
		/* at */
		for (i = 0; i < IntRing_size(ir); i++) {
			assert(*IntRing_at(ir, i) == hoge[i]);
			*IntRing_at(ir, i) = hoge[MAX - i -1];
			assert(*IntRing_at(ir, i) == hoge[MAX - i -1]);
		}
		/* clear */
		IntRing_clear(ir);
		assert(IntRing_size(ir) == 0);
		assert(IntRing_empty(ir));
		assert(!IntRing_full(ir));
		/* insert_array */
		IntRing_clear(ir);
		assert(IntRing_insert_array(ir, 0, piyo, MAX/2));
		assert(ir_cmp(ir, 0, piyo, MAX/2) == 0);
		assert(IntRing_size(ir) == MAX/2);
		assert(!IntRing_empty(ir));
		assert(!IntRing_full(ir));
		/* clear */
		IntRing_clear(ir);
		assert(IntRing_size(ir) == 0);
	}

	HEAP_DUMP_OVERFLOW(&heap);
	IntRing_delete(ir);
}

void RingTest_test_2_2(void)
{
	int i, j;
	size_t size;
	printf("***** test_2_2 *****\n");
	ir = IntRing_new(MAX);
	for (j = 0; j < MAX+1; j++) {
		ir->begin = ir->end = j;
		assert(IntRing_size(ir) == 0);
		/* push_back */
		/* MAXまで */
		for (i = 0; i < MAX; i++) {
			size = IntRing_size(ir);
			assert(IntRing_push_back(ir, piyo[i]));
			assert(IntRing_size(ir) == size + 1);
		}
		assert(ir_cmp(ir, 0, piyo, MAX) == 0);
		assert(IntRing_size(ir) == MAX);
		assert(!IntRing_empty(ir));
		assert(IntRing_full(ir));
		/* FULLで失敗 */
		assert(!IntRing_push_back(ir, piyo[0]));
		assert(IntRing_size(ir) == MAX);
		assert(!IntRing_empty(ir));
		assert(IntRing_full(ir));
		/* front */
		assert(IntRing_front(ir) == piyo[0]);
		/* back */
		assert(IntRing_back(ir) == piyo[MAX-1]);
		/* pop_front */
		size = IntRing_size(ir);
		assert(IntRing_pop_front(ir) == piyo[0]);
		assert(IntRing_size(ir) == size - 1);
		/* push_back */
		size = IntRing_size(ir);
		assert(IntRing_push_back(ir, piyo[0]));
		assert(IntRing_size(ir) == size + 1);
		assert(ir_cmp(ir, 0, &piyo[1], MAX-1) == 0);
		assert(ir_cmp(ir, MAX-1, piyo, 1) == 0);
		assert(IntRing_size(ir) == MAX);
		assert(!IntRing_empty(ir));
		assert(IntRing_full(ir));
		/* pop_front */
		for (i = 0; i < MAX; i++) {
			size = IntRing_size(ir);
			if (i < MAX-1) {
				assert(IntRing_pop_front(ir) == piyo[i+1]);
			} else {
				assert(IntRing_pop_front(ir) == piyo[0]);
			}
			assert(IntRing_size(ir) == size - 1);
		}
		assert(IntRing_size(ir) == 0);
		assert(IntRing_empty(ir));
		assert(!IntRing_full(ir));
	}

	HEAP_DUMP_OVERFLOW(&heap);
	IntRing_delete(ir);
}

void RingTest_test_2_3(void)
{
	int i, j;
	size_t size;
	printf("***** test_2_3 *****\n");
	ir = IntRing_new(MAX);
	for (j = 0; j < MAX+1; j++) {
		ir->begin = ir->end = j;
		assert(IntRing_size(ir) == 0);
		/* push_front */
		/* MAXまで */
		for (i = 0; i < MAX; i++) {
			size = IntRing_size(ir);
			assert(IntRing_push_front(ir, piyo[MAX-i-1]));
			assert(IntRing_size(ir) == size + 1);
		}
		assert(ir_cmp(ir, 0, piyo, MAX) == 0);
		assert(IntRing_size(ir) == MAX);
		assert(!IntRing_empty(ir));
		assert(IntRing_full(ir));
		/* FULLで失敗 */
		assert(!IntRing_push_front(ir, piyo[0]));
		assert(IntRing_size(ir) == MAX);
		assert(!IntRing_empty(ir));
		assert(IntRing_full(ir));
		/* front */
		assert(IntRing_front(ir) == piyo[0]);
		/* back */
		assert(IntRing_back(ir) == piyo[MAX-1]);
		/* pop_back */
		size = IntRing_size(ir);
		assert(IntRing_pop_back(ir) == piyo[MAX-1]);
		assert(IntRing_pop_back(ir) == piyo[MAX-2]);
		assert(IntRing_size(ir) == size - 2);
		/* push_front */
		size = IntRing_size(ir);
		assert(IntRing_push_front(ir, piyo[MAX-1]));
		assert(IntRing_push_front(ir, piyo[MAX-2]));
		assert(IntRing_size(ir) == size + 2);
		assert(ir_cmp(ir, 0, &piyo[MAX-2], 2) == 0);
		assert(ir_cmp(ir, 2, piyo, MAX-2) == 0);
		assert(IntRing_size(ir) == MAX);
		assert(!IntRing_empty(ir));
		assert(IntRing_full(ir));
		/* pop_back */
		for (i = MAX-3; i >= 0; i--) {
			size = IntRing_size(ir);
			assert(IntRing_pop_back(ir) == piyo[i]);
			assert(IntRing_size(ir) == size - 1);
		}
		assert(IntRing_pop_back(ir) == piyo[MAX-1]);
		assert(IntRing_pop_back(ir) == piyo[MAX-2]);

		assert(IntRing_size(ir) == 0);
		assert(IntRing_empty(ir));
		assert(!IntRing_full(ir));
	}

	HEAP_DUMP_OVERFLOW(&heap);
	IntRing_delete(ir);
}

void RingTest_test_2_4(void)
{
	int i, j;
	printf("***** test_2_4 *****\n");
	ir = IntRing_new(MAX);
	for (j = 0; j < MAX+1; j++) {
		ir->begin = ir->end = j;
		assert(IntRing_size(ir) == 0);
		/* insert */
		/* 空状態 */
		for (i = 0; i < MAX/2; i++) {
			assert(ir_cmp(ir, 0, piyo, i) == 0);
			assert(IntRing_insert(ir, i, piyo[i]));
		}
		/* 先頭・前寄り */
		assert(IntRing_insert(ir, 0, piyo[0]));
		assert(IntRing_insert(ir, 1, piyo[1]));
		assert(IntRing_insert(ir, 2, piyo[2]));
		assert(ir_cmp(ir, 0, piyo, 3) == 0);
		assert(ir_cmp(ir, 3, piyo, MAX/2) == 0);
		assert(IntRing_size(ir) == MAX/2 + 3);

		/* erase */
		/* 先頭・前寄り */
		IntRing_erase(ir, 2, 1);
		assert(ir_cmp(ir, 0, piyo, 2) == 0);
		assert(ir_cmp(ir, 2, piyo, MAX/2) == 0);
		assert(IntRing_size(ir) == MAX/2 + 2);
		IntRing_erase(ir, 1, 1);
		assert(ir_cmp(ir, 0, piyo, 1) == 0);
		assert(ir_cmp(ir, 1, piyo, MAX/2) == 0);
		assert(IntRing_size(ir) == MAX/2 + 1);
		IntRing_erase(ir, 0, 1);
		assert(ir_cmp(ir, 0, piyo, MAX/2) == 0);
		assert(IntRing_size(ir) == MAX/2);

		/* insert */
		/* 末尾・後ろ寄り */
		assert(IntRing_insert(ir, MAX/2, piyo[0]));
		assert(IntRing_insert(ir, MAX/2, piyo[1]));
		assert(IntRing_insert(ir, MAX/2, piyo[2]));
		assert(ir_cmp(ir, 0, piyo, MAX/2) == 0);
		assert(*IntRing_at(ir, MAX/2 + 0) == piyo[2]);
		assert(*IntRing_at(ir, MAX/2 + 1) == piyo[1]);
		assert(*IntRing_at(ir, MAX/2 + 2) == piyo[0]);

		/* erase */
		/* 末尾・後ろ寄り */
		IntRing_erase(ir, MAX/2, 1);
		assert(ir_cmp(ir, 0, piyo, MAX/2) == 0);
		assert(*IntRing_at(ir, MAX/2 + 0) == piyo[1]);
		assert(*IntRing_at(ir, MAX/2 + 1) == piyo[0]);
		assert(IntRing_size(ir) == MAX/2 + 2);
		IntRing_erase(ir, MAX/2, 1);
		assert(ir_cmp(ir, 0, piyo, MAX/2) == 0);
		assert(*IntRing_at(ir, MAX/2 + 0) == piyo[0]);
		assert(IntRing_size(ir) == MAX/2 + 1);
		IntRing_erase(ir, MAX/2, 1);
		assert(ir_cmp(ir, 0, piyo, MAX/2) == 0);
		assert(IntRing_size(ir) == MAX/2);

		/* 先頭から全削除 */
		for (i = 0; i < MAX/2; i++) {
			assert(ir_cmp(ir, 0, &piyo[i], MAX/2 - i) == 0);
			IntRing_erase(ir, 0, 1);
		}
		assert(IntRing_size(ir) == 0);

		for (i = 0; i < MAX/2; i++) {
			assert(ir_cmp(ir, 0, piyo, i) == 0);
			assert(IntRing_insert(ir, i, piyo[i]));
		}
		assert(IntRing_size(ir) == MAX/2);

		/* 末尾から全削除 */
		for (i = 0; i < MAX/2; i++) {
			assert(ir_cmp(ir, 0, piyo, MAX/2 - i) == 0);
			IntRing_erase(ir, IntRing_size(ir) -1, 1);
		}
		assert(IntRing_size(ir) == 0);

	}

	HEAP_DUMP_OVERFLOW(&heap);
	IntRing_delete(ir);
}

void RingTest_test_2_5(void)
{
	int i;
	printf("***** test_2_5 *****\n");
	ir = IntRing_new(MAX);
	for (i = 0; i < MAX+1; i++) {
		ir->begin = ir->end = i;
		assert(IntRing_size(ir) == 0);
		/* insert_array */
		/* 空状態 */
		assert(IntRing_insert_array(ir, 0, piyo, MAX/2));
		assert(ir_cmp(ir, 0, piyo, MAX/2) == 0);
		assert(IntRing_size(ir) == MAX/2);

		/* insert_array */
		/* 先頭 */
		assert(IntRing_insert_array(ir, 0, piyo, 3));
		assert(ir_cmp(ir, 0, piyo, 3) == 0);
		assert(ir_cmp(ir, 3, piyo, MAX/2) == 0);
		assert(IntRing_size(ir) == MAX/2 + 3);
		/* erase */
		IntRing_erase(ir, 0, 3);
		assert(ir_cmp(ir, 0, piyo, MAX/2) == 0);
		assert(IntRing_size(ir) == MAX/2);

		/* insert_array */
		/* 前寄り */
		assert(IntRing_insert_array(ir, 2, piyo, 3));
		assert(ir_cmp(ir, 0, piyo, 2) == 0);
		assert(ir_cmp(ir, 2, piyo, 3) == 0);
		assert(ir_cmp(ir, 5, &piyo[2], MAX/2 -2) == 0);
		assert(IntRing_size(ir) == MAX/2 + 3);
		/* 0個指定 */
		assert(IntRing_insert_array(ir, 1, &piyo[MAX/2], 0));
		assert(ir_cmp(ir, 0, piyo, 2) == 0);
		assert(ir_cmp(ir, 2, piyo, 3) == 0);
		assert(ir_cmp(ir, 5, &piyo[2], MAX/2 -2) == 0);
		assert(IntRing_size(ir) == MAX/2 + 3);
		/* 1個指定 */
		assert(IntRing_insert_array(ir, 1, &piyo[MAX-1], 1));
		assert(ir_cmp(ir, 0, &piyo[0], 1) == 0);
		assert(ir_cmp(ir, 1, &piyo[MAX-1], 1) == 0);
		assert(ir_cmp(ir, 2, &piyo[1], 1) == 0);
		assert(ir_cmp(ir, 3, &piyo[0], 3) == 0);
		assert(ir_cmp(ir, 6, &piyo[2], MAX/2 -2) == 0);
		assert(IntRing_size(ir) == MAX/2 + 4);
		/* erase */
		/* 0個指定 */
		IntRing_erase(ir, 1, 0);
		assert(ir_cmp(ir, 0, &piyo[0], 1) == 0);
		assert(ir_cmp(ir, 1, &piyo[MAX-1], 1) == 0);
		assert(ir_cmp(ir, 2, &piyo[1], 1) == 0);
		assert(ir_cmp(ir, 3, &piyo[0], 3) == 0);
		assert(ir_cmp(ir, 6, &piyo[2], MAX/2 -2) == 0);
		assert(IntRing_size(ir) == MAX/2 + 4);
		/* 1個指定 */
		IntRing_erase(ir, 1, 1);
		assert(ir_cmp(ir, 0, piyo, 2) == 0);
		assert(ir_cmp(ir, 2, piyo, 3) == 0);
		assert(ir_cmp(ir, 5, &piyo[2], MAX/2 -2) == 0);
		assert(IntRing_size(ir) == MAX/2 + 3);
		/* 前寄り */
		IntRing_erase(ir, 2, 3);
		assert(ir_cmp(ir, 0, piyo, MAX/2) == 0);
		assert(IntRing_size(ir) == MAX/2);

		/* insert_array */
		/* 末尾 */
		assert(IntRing_insert_array(ir, IntRing_size(ir), piyo, 3));
		assert(ir_cmp(ir, 0, piyo, MAX/2) == 0);
		assert(ir_cmp(ir, MAX/2, piyo, 3) == 0);
		assert(IntRing_size(ir) == MAX/2 + 3);
		/* erase */
		IntRing_erase(ir, IntRing_size(ir) - 3, 3);
		assert(ir_cmp(ir, 0, piyo, MAX/2) == 0);
		assert(IntRing_size(ir) == MAX/2);

		/* insert_array */
		/* 後ろ寄り */
		assert(IntRing_insert_array(ir, IntRing_size(ir) - 2, piyo, 3));
		assert(ir_cmp(ir, 0, piyo, MAX/2 -2) == 0);
		assert(ir_cmp(ir, MAX/2 -2, piyo, 3) == 0);
		assert(ir_cmp(ir, MAX/2 +1, &piyo[MAX/2 -2], 2) == 0);
		assert(IntRing_size(ir) == MAX/2 + 3);
		/* 0個指定 */
		assert(IntRing_insert_array(ir, IntRing_size(ir) - 1, &piyo[MAX-1], 0));
		assert(ir_cmp(ir, 0, piyo, MAX/2 -2) == 0);
		assert(ir_cmp(ir, MAX/2 -2, piyo, 3) == 0);
		assert(ir_cmp(ir, MAX/2 +1, &piyo[MAX/2 -2], 2) == 0);
		assert(IntRing_size(ir) == MAX/2 + 3);
		/* 1個指定 */
		assert(IntRing_insert_array(ir, IntRing_size(ir) - 1, &piyo[MAX-1], 1));
		assert(ir_cmp(ir, 0, piyo, MAX/2 -2) == 0);
		assert(ir_cmp(ir, MAX/2 -2, piyo, 3) == 0);
		assert(ir_cmp(ir, MAX/2 +1, &piyo[MAX/2 -2], 1) == 0);
		assert(ir_cmp(ir, MAX/2 +2, &piyo[MAX-1], 1) == 0);
		assert(ir_cmp(ir, MAX/2 +3, &piyo[MAX/2 -1], 1) == 0);
		assert(IntRing_size(ir) == MAX/2 + 4);
		/* erase */
		/* 0個指定 */
		IntRing_erase(ir, IntRing_size(ir) - 2, 0);
		assert(ir_cmp(ir, 0, piyo, MAX/2 -2) == 0);
		assert(ir_cmp(ir, MAX/2 -2, piyo, 3) == 0);
		assert(ir_cmp(ir, MAX/2 +1, &piyo[MAX/2 -2], 1) == 0);
		assert(ir_cmp(ir, MAX/2 +2, &piyo[MAX-1], 1) == 0);
		assert(ir_cmp(ir, MAX/2 +3, &piyo[MAX/2 -1], 1) == 0);
		assert(IntRing_size(ir) == MAX/2 + 4);
		/* 1個指定 */
		IntRing_erase(ir, IntRing_size(ir) - 2, 1);
		assert(ir_cmp(ir, 0, piyo, MAX/2 -2) == 0);
		assert(ir_cmp(ir, MAX/2 -2, piyo, 3) == 0);
		assert(ir_cmp(ir, MAX/2 +1, &piyo[MAX/2 -2], 2) == 0);
		assert(IntRing_size(ir) == MAX/2 + 3);
		/* 後ろ寄り */
		IntRing_erase(ir, IntRing_size(ir) - 5, 3);
		assert(ir_cmp(ir, 0, piyo, MAX/2) == 0);
		assert(IntRing_size(ir) == MAX/2);

		/* MAX以上で失敗 */
		assert(!IntRing_insert_array(ir, 0, piyo, MAX - MAX/2 +1));
		assert(ir_cmp(ir, 0, piyo, MAX/2) == 0);
		assert(IntRing_size(ir) == MAX/2);
		/* MAXになるまで */
		assert(IntRing_insert_array(ir, 0, piyo, MAX - MAX/2));
		assert(IntRing_size(ir) == MAX);
		assert(IntRing_full(ir));
		/* 先頭から全削除 */
		IntRing_erase(ir, 0, IntRing_size(ir));
		assert(IntRing_size(ir) == 0);
	}

	HEAP_DUMP_OVERFLOW(&heap);
	IntRing_delete(ir);
}






void RingTest_run(void)
{
	printf("\n===== ring test =====\n");
	ring_init_hoge();
	ring_init_piyo();

	RingTest_test_1_1();
	RingTest_test_1_2();
	RingTest_test_1_3();
	RingTest_test_1_4();
	RingTest_test_1_5();
	RingTest_test_1_6();

	RingTest_test_2_1();
	RingTest_test_2_2();
	RingTest_test_2_3();
	RingTest_test_2_4();
	RingTest_test_2_5();
}


int main(void)
{
#ifdef MY_MALLOC
	Heap_init(&heap, buf, sizeof buf, sizeof buf[0]);
#endif
	RingTest_run();
#ifdef MY_MALLOC
	HEAP_DUMP_LEAK(&heap, 0);
#endif
	return 0;
}
