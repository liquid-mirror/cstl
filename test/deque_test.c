#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include "../cstl/deque.h"
#include "heap.h"
#ifdef MY_MALLOC
double buf[1024*1024/sizeof(double)];
Heap heap;
#define malloc(s)		Heap_malloc(&heap, s)
#define realloc(p, s)	Heap_realloc(&heap, p, s)
#define free(p)			Heap_free(&heap, p)
#endif



/* deque */
#ifdef CSTLGEN
#include "UCharDeque.h"
#include "IntDeque.h"
#else
#include "deque_debug.h"
CSTL_DEQUE_INTERFACE(UCharDeque, unsigned char)
CSTL_DEQUE_INTERFACE(IntDeque, int)

CSTL_DEQUE_IMPLEMENT(UCharDeque, unsigned char)
CSTL_DEQUE_IMPLEMENT(IntDeque, int)

CSTL_DEQUE_DEBUG_INTERFACE(UCharDeque, unsigned char)
CSTL_DEQUE_DEBUG_INTERFACE(IntDeque, int)

CSTL_DEQUE_DEBUG_IMPLEMENT(UCharDeque, unsigned char, %d)
CSTL_DEQUE_DEBUG_IMPLEMENT(IntDeque, int, %d)
#endif

static UCharDeque *ud;
static IntDeque *id;

#define MAX		(255*5)

static unsigned char hoge[MAX];
static int piyo[MAX];

void deque_init_hoge(void)
{
	int i;
	for (i = 0; i < MAX; i++) {
		hoge[i] = i % UCHAR_MAX;
	}
}

void deque_init_piyo(void)
{
	int i;
	for (i = 0; i < MAX; i++) {
		piyo[i] = i % INT_MAX;
	}
}

int ud_cmp(UCharDeque *x, size_t idx, unsigned char *buf, size_t n)
{
	int i, j;
	for (i = idx, j = 0; i < idx + n; i++, j++) {
/*        printf("%d, %d\n", *UCharDeque_at(x, i), buf[j]);*/
		if (*UCharDeque_at(x, i) != buf[j]) return 1;
	}
	return 0;
}

#if 1
int id_cmp(IntDeque *x, size_t idx, int *buf, size_t n)
{
	int i, j;
	for (i = idx, j = 0; i < idx + n; i++, j++) {
/*        printf("%d, %d\n", *IntDeque_at(x, i), buf[j]);*/
		if (*IntDeque_at(x, i) != buf[j]) return 1;
	}
	return 0;
}
#endif


void DequeTest_test_1_1(void)
{
	int i;
	printf("***** test_1_1 *****\n");
	ud = UCharDeque_new();
	assert(UCharDeque_verify(ud));
	/* 初期状態 */
	assert(UCharDeque_size(ud) == 0);
	assert(UCharDeque_empty(ud));
/*    UCharDeque_print(ud);*/
	/* push_back */
	UCharDeque_clear(ud);
	for (i = 0; i < MAX; i++) {
		assert(UCharDeque_push_back(ud, hoge[i]));
		assert(UCharDeque_verify(ud));
	}
/*    UCharDeque_print(ud);*/
	assert(ud_cmp(ud, 0, hoge, MAX) == 0);
	assert(UCharDeque_size(ud) == MAX);
	assert(!UCharDeque_empty(ud));
	/* at */
	for (i = 0; i < UCharDeque_size(ud); i++) {
		assert(*UCharDeque_at(ud, i) == hoge[i]);
		*UCharDeque_at(ud, i) = hoge[MAX - i -1];
		assert(*UCharDeque_at(ud, i) == hoge[MAX - i -1]);
	}
	/* clear */
	UCharDeque_clear(ud);
/*    UCharDeque_print(ud);*/
	assert(UCharDeque_verify(ud));
	assert(UCharDeque_size(ud) == 0);
	assert(UCharDeque_empty(ud));
	/* push_back */
	UCharDeque_clear(ud);
	for (i = 0; i < MAX/2; i++) {
		assert(UCharDeque_push_back(ud, hoge[i]));
	}
	assert(ud_cmp(ud, 0, hoge, MAX/2) == 0);
	assert(UCharDeque_size(ud) == MAX/2);
	assert(!UCharDeque_empty(ud));
	/* clear */
	UCharDeque_clear(ud);
	assert(UCharDeque_size(ud) == 0);

	HEAP_DUMP_OVERFLOW(&heap);
	UCharDeque_delete(ud);
}

void DequeTest_test_1_2(void)
{
	int i;
	size_t size;
	int c = 0;
	printf("***** test_1_2 *****\n");
	ud = UCharDeque_new();

	assert(UCharDeque_size(ud) == 0);
	/* push_back */
	/* MAXまで */
	for (i = 0; i < MAX; i++) {
		size = UCharDeque_size(ud);
		assert(UCharDeque_push_back(ud, hoge[i]));
		assert(UCharDeque_size(ud) == size + 1);
		assert(UCharDeque_verify(ud));
	}
	assert(ud_cmp(ud, 0, hoge, MAX) == 0);
	assert(UCharDeque_size(ud) == MAX);
	assert(!UCharDeque_empty(ud));
/*    UCharDeque_print(ud);*/
	/* front */
	assert(UCharDeque_front(ud) == hoge[0]);
	/* back */
	assert(UCharDeque_back(ud) == hoge[MAX-1]);
	/* pop_front */
	size = UCharDeque_size(ud);
	assert(UCharDeque_pop_front(ud) == hoge[0]);
	assert(UCharDeque_size(ud) == size - 1);
	assert(UCharDeque_verify(ud));
	/* push_back */
	size = UCharDeque_size(ud);
	assert(UCharDeque_push_back(ud, hoge[0]));
	assert(UCharDeque_size(ud) == size + 1);
	assert(ud_cmp(ud, 0, &hoge[1], MAX-1) == 0);
	assert(ud_cmp(ud, MAX-1, hoge, 1) == 0);
	assert(UCharDeque_size(ud) == MAX);
	assert(!UCharDeque_empty(ud));
	assert(UCharDeque_verify(ud));
	/* pop_front */
	for (i = 0; i < MAX; i++) {
		size = UCharDeque_size(ud);
		if (i < MAX-1) {
			assert(UCharDeque_pop_front(ud) == hoge[i+1]);
		} else {
			assert(UCharDeque_pop_front(ud) == hoge[0]);
		}
		assert(UCharDeque_size(ud) == size - 1);
		assert(UCharDeque_verify(ud));
	}
	UCharDeque_print(ud);
	assert(UCharDeque_size(ud) == 0);
	assert(UCharDeque_empty(ud));
	/* 大量にpush_back */
	while (c < 10000000 && UCharDeque_push_back(ud, 0xff)) {
		c++;
	}
/*    UCharDeque_print(ud);*/
/*    printf("size: %d, c: %d\n", UCharDeque_size(ud), c);*/
	assert(UCharDeque_size(ud) == c);
	assert(!UCharDeque_empty(ud));
	assert(UCharDeque_verify(ud));

	UCharDeque_clear(ud);
/*    UCharDeque_print(ud);*/

	HEAP_DUMP_OVERFLOW(&heap);
	UCharDeque_delete(ud);
}

void DequeTest_test_1_3(void)
{
	int i;
	size_t size;
	int c = 0;
	printf("***** test_1_3 *****\n");
	ud = UCharDeque_new();

	assert(UCharDeque_size(ud) == 0);
	/* push_front */
	/* MAXまで */
	for (i = 0; i < MAX; i++) {
		size = UCharDeque_size(ud);
		assert(UCharDeque_push_front(ud, hoge[MAX-i-1]));
		assert(UCharDeque_size(ud) == size + 1);
		assert(UCharDeque_verify(ud));
	}
	assert(ud_cmp(ud, 0, hoge, MAX) == 0);
	assert(UCharDeque_size(ud) == MAX);
	assert(!UCharDeque_empty(ud));
	/* front */
	assert(UCharDeque_front(ud) == hoge[0]);
	/* back */
	assert(UCharDeque_back(ud) == hoge[MAX-1]);
	/* pop_back */
	size = UCharDeque_size(ud);
	assert(UCharDeque_pop_back(ud) == hoge[MAX-1]);
	assert(UCharDeque_pop_back(ud) == hoge[MAX-2]);
	assert(UCharDeque_size(ud) == size - 2);
	assert(UCharDeque_verify(ud));
	/* push_front */
	size = UCharDeque_size(ud);
	assert(UCharDeque_push_front(ud, hoge[MAX-1]));
	assert(UCharDeque_push_front(ud, hoge[MAX-2]));
	assert(UCharDeque_size(ud) == size + 2);
	assert(ud_cmp(ud, 0, &hoge[MAX-2], 2) == 0);
	assert(ud_cmp(ud, 2, hoge, MAX-2) == 0);
	assert(UCharDeque_size(ud) == MAX);
	assert(!UCharDeque_empty(ud));
	assert(UCharDeque_verify(ud));
	/* pop_back */
	for (i = MAX-3; i >= 0; i--) {
		size = UCharDeque_size(ud);
		assert(UCharDeque_pop_back(ud) == hoge[i]);
		assert(UCharDeque_size(ud) == size - 1);
		assert(UCharDeque_verify(ud));
	}
	assert(UCharDeque_pop_back(ud) == hoge[MAX-1]);
	assert(UCharDeque_pop_back(ud) == hoge[MAX-2]);

	assert(UCharDeque_size(ud) == 0);
	assert(UCharDeque_empty(ud));
	assert(UCharDeque_verify(ud));
	/* 大量にpush_front */
	while (c < 10000000 && UCharDeque_push_front(ud, 0xff)) {
		c++;
	}
/*    printf("size: %d, c: %d\n", UCharDeque_size(ud), c);*/
	assert(UCharDeque_size(ud) == c);
	assert(!UCharDeque_empty(ud));
	assert(UCharDeque_verify(ud));

	HEAP_DUMP_OVERFLOW(&heap);
	UCharDeque_delete(ud);
}

void DequeTest_test_1_4(void)
{
	int i;
	printf("***** test_1_4 *****\n");
	ud = UCharDeque_new();
	assert(UCharDeque_size(ud) == 0);
	/* insert */
	/* 空状態 */
	for (i = 0; i < MAX/2; i++) {
		assert(ud_cmp(ud, 0, hoge, i) == 0);
		assert(UCharDeque_insert(ud, i, hoge[i]));
		assert(UCharDeque_verify(ud));
	}
	/* 先頭・前寄り */
	assert(UCharDeque_insert(ud, 0, hoge[0]));
	assert(UCharDeque_verify(ud));
	assert(UCharDeque_insert(ud, 1, hoge[1]));
	assert(UCharDeque_verify(ud));
	assert(UCharDeque_insert(ud, 2, hoge[2]));
	assert(UCharDeque_verify(ud));
	assert(ud_cmp(ud, 0, hoge, 3) == 0);
	assert(ud_cmp(ud, 3, hoge, MAX/2) == 0);
	assert(UCharDeque_size(ud) == MAX/2 + 3);

	/* erase */
	/* 先頭・前寄り */
	UCharDeque_erase(ud, 2, 1);
	assert(ud_cmp(ud, 0, hoge, 2) == 0);
	assert(ud_cmp(ud, 2, hoge, MAX/2) == 0);
	assert(UCharDeque_size(ud) == MAX/2 + 2);
	assert(UCharDeque_verify(ud));
	UCharDeque_erase(ud, 1, 1);
	assert(ud_cmp(ud, 0, hoge, 1) == 0);
	assert(ud_cmp(ud, 1, hoge, MAX/2) == 0);
	assert(UCharDeque_size(ud) == MAX/2 + 1);
	assert(UCharDeque_verify(ud));
	UCharDeque_erase(ud, 0, 1);
	assert(ud_cmp(ud, 0, hoge, MAX/2) == 0);
	assert(UCharDeque_size(ud) == MAX/2);
	assert(UCharDeque_verify(ud));

	/* insert */
	/* 末尾・後ろ寄り */
	assert(UCharDeque_insert(ud, MAX/2, hoge[0]));
	assert(UCharDeque_verify(ud));
	assert(UCharDeque_insert(ud, MAX/2, hoge[1]));
	assert(UCharDeque_verify(ud));
	assert(UCharDeque_insert(ud, MAX/2, hoge[2]));
	assert(UCharDeque_verify(ud));
	assert(ud_cmp(ud, 0, hoge, MAX/2) == 0);
	assert(*UCharDeque_at(ud, MAX/2 + 0) == hoge[2]);
	assert(*UCharDeque_at(ud, MAX/2 + 1) == hoge[1]);
	assert(*UCharDeque_at(ud, MAX/2 + 2) == hoge[0]);

	/* erase */
	/* 末尾・後ろ寄り */
	UCharDeque_erase(ud, MAX/2, 1);
	assert(ud_cmp(ud, 0, hoge, MAX/2) == 0);
	assert(*UCharDeque_at(ud, MAX/2 + 0) == hoge[1]);
	assert(*UCharDeque_at(ud, MAX/2 + 1) == hoge[0]);
	assert(UCharDeque_size(ud) == MAX/2 + 2);
	assert(UCharDeque_verify(ud));
	UCharDeque_erase(ud, MAX/2, 1);
	assert(ud_cmp(ud, 0, hoge, MAX/2) == 0);
	assert(*UCharDeque_at(ud, MAX/2 + 0) == hoge[0]);
	assert(UCharDeque_size(ud) == MAX/2 + 1);
	assert(UCharDeque_verify(ud));
	UCharDeque_erase(ud, MAX/2, 1);
	assert(ud_cmp(ud, 0, hoge, MAX/2) == 0);
	assert(UCharDeque_size(ud) == MAX/2);
	assert(UCharDeque_verify(ud));

	/* 先頭から全削除 */
	for (i = 0; i < MAX/2; i++) {
		assert(ud_cmp(ud, 0, &hoge[i], MAX/2 - i) == 0);
		UCharDeque_erase(ud, 0, 1);
		assert(UCharDeque_verify(ud));
	}
	assert(UCharDeque_size(ud) == 0);

	for (i = 0; i < MAX/2; i++) {
		assert(ud_cmp(ud, 0, hoge, i) == 0);
		assert(UCharDeque_insert(ud, i, hoge[i]));
		assert(UCharDeque_verify(ud));
	}
	assert(UCharDeque_size(ud) == MAX/2);

	/* 末尾から全削除 */
	for (i = 0; i < MAX/2; i++) {
		assert(ud_cmp(ud, 0, hoge, MAX/2 - i) == 0);
		UCharDeque_erase(ud, UCharDeque_size(ud) -1, 1);
		assert(UCharDeque_verify(ud));
	}
	assert(UCharDeque_size(ud) == 0);
	assert(UCharDeque_verify(ud));

	HEAP_DUMP_OVERFLOW(&heap);
	UCharDeque_delete(ud);
}

void DequeTest_test_1_5(void)
{
	printf("***** test_1_5 *****\n");
	ud = UCharDeque_new();

	assert(UCharDeque_size(ud) == 0);
	/* insert_array */
	/* 空状態 */
	assert(UCharDeque_insert_array(ud, 0, hoge, MAX/2));
	assert(ud_cmp(ud, 0, hoge, MAX/2) == 0);
	assert(UCharDeque_size(ud) == MAX/2);
	assert(UCharDeque_verify(ud));

	/* insert_array */
	/* 先頭 */
	assert(UCharDeque_insert_array(ud, 0, hoge, 3));
	assert(ud_cmp(ud, 0, hoge, 3) == 0);
	assert(ud_cmp(ud, 3, hoge, MAX/2) == 0);
	assert(UCharDeque_size(ud) == MAX/2 + 3);
	assert(UCharDeque_verify(ud));
	/* erase */
	UCharDeque_erase(ud, 0, 3);
	assert(ud_cmp(ud, 0, hoge, MAX/2) == 0);
	assert(UCharDeque_size(ud) == MAX/2);
	assert(UCharDeque_verify(ud));

	/* insert_array */
	/* 前寄り */
	assert(UCharDeque_insert_array(ud, 2, hoge, 3));
	assert(ud_cmp(ud, 0, hoge, 2) == 0);
	assert(ud_cmp(ud, 2, hoge, 3) == 0);
	assert(ud_cmp(ud, 5, &hoge[2], MAX/2 -2) == 0);
	assert(UCharDeque_size(ud) == MAX/2 + 3);
	assert(UCharDeque_verify(ud));
	/* 0個指定 */
	assert(UCharDeque_insert_array(ud, 1, &hoge[MAX/2], 0));
	assert(ud_cmp(ud, 0, hoge, 2) == 0);
	assert(ud_cmp(ud, 2, hoge, 3) == 0);
	assert(ud_cmp(ud, 5, &hoge[2], MAX/2 -2) == 0);
	assert(UCharDeque_size(ud) == MAX/2 + 3);
	assert(UCharDeque_verify(ud));
	/* 1個指定 */
	assert(UCharDeque_insert_array(ud, 1, &hoge[MAX-1], 1));
	assert(ud_cmp(ud, 0, &hoge[0], 1) == 0);
	assert(ud_cmp(ud, 1, &hoge[MAX-1], 1) == 0);
	assert(ud_cmp(ud, 2, &hoge[1], 1) == 0);
	assert(ud_cmp(ud, 3, &hoge[0], 3) == 0);
	assert(ud_cmp(ud, 6, &hoge[2], MAX/2 -2) == 0);
	assert(UCharDeque_size(ud) == MAX/2 + 4);
	assert(UCharDeque_verify(ud));
	/* erase */
	/* 0個指定 */
	UCharDeque_erase(ud, 1, 0);
	assert(ud_cmp(ud, 0, &hoge[0], 1) == 0);
	assert(ud_cmp(ud, 1, &hoge[MAX-1], 1) == 0);
	assert(ud_cmp(ud, 2, &hoge[1], 1) == 0);
	assert(ud_cmp(ud, 3, &hoge[0], 3) == 0);
	assert(ud_cmp(ud, 6, &hoge[2], MAX/2 -2) == 0);
	assert(UCharDeque_size(ud) == MAX/2 + 4);
	assert(UCharDeque_verify(ud));
	/* 1個指定 */
	UCharDeque_erase(ud, 1, 1);
	assert(ud_cmp(ud, 0, hoge, 2) == 0);
	assert(ud_cmp(ud, 2, hoge, 3) == 0);
	assert(ud_cmp(ud, 5, &hoge[2], MAX/2 -2) == 0);
	assert(UCharDeque_size(ud) == MAX/2 + 3);
	assert(UCharDeque_verify(ud));
	/* 前寄り */
	UCharDeque_erase(ud, 2, 3);
	assert(ud_cmp(ud, 0, hoge, MAX/2) == 0);
	assert(UCharDeque_size(ud) == MAX/2);
	assert(UCharDeque_verify(ud));

	/* insert_array */
	/* 末尾 */
	assert(UCharDeque_insert_array(ud, UCharDeque_size(ud), hoge, 3));
	assert(ud_cmp(ud, 0, hoge, MAX/2) == 0);
	assert(ud_cmp(ud, MAX/2, hoge, 3) == 0);
	assert(UCharDeque_size(ud) == MAX/2 + 3);
	assert(UCharDeque_verify(ud));
	/* erase */
	UCharDeque_erase(ud, UCharDeque_size(ud) - 3, 3);
	assert(ud_cmp(ud, 0, hoge, MAX/2) == 0);
	assert(UCharDeque_size(ud) == MAX/2);
	assert(UCharDeque_verify(ud));

	/* insert_array */
	/* 後ろ寄り */
	assert(UCharDeque_insert_array(ud, UCharDeque_size(ud) - 2, hoge, 3));
	assert(ud_cmp(ud, 0, hoge, MAX/2 -2) == 0);
	assert(ud_cmp(ud, MAX/2 -2, hoge, 3) == 0);
	assert(ud_cmp(ud, MAX/2 +1, &hoge[MAX/2 -2], 2) == 0);
	assert(UCharDeque_size(ud) == MAX/2 + 3);
	assert(UCharDeque_verify(ud));
	/* 0個指定 */
	assert(UCharDeque_insert_array(ud, UCharDeque_size(ud) - 1, &hoge[MAX-1], 0));
	assert(ud_cmp(ud, 0, hoge, MAX/2 -2) == 0);
	assert(ud_cmp(ud, MAX/2 -2, hoge, 3) == 0);
	assert(ud_cmp(ud, MAX/2 +1, &hoge[MAX/2 -2], 2) == 0);
	assert(UCharDeque_size(ud) == MAX/2 + 3);
	assert(UCharDeque_verify(ud));
	/* 1個指定 */
	assert(UCharDeque_insert_array(ud, UCharDeque_size(ud) - 1, &hoge[MAX-1], 1));
	assert(ud_cmp(ud, 0, hoge, MAX/2 -2) == 0);
	assert(ud_cmp(ud, MAX/2 -2, hoge, 3) == 0);
	assert(ud_cmp(ud, MAX/2 +1, &hoge[MAX/2 -2], 1) == 0);
	assert(ud_cmp(ud, MAX/2 +2, &hoge[MAX-1], 1) == 0);
	assert(ud_cmp(ud, MAX/2 +3, &hoge[MAX/2 -1], 1) == 0);
	assert(UCharDeque_size(ud) == MAX/2 + 4);
	assert(UCharDeque_verify(ud));
	/* erase */
	/* 0個指定 */
	UCharDeque_erase(ud, UCharDeque_size(ud) - 2, 0);
	assert(ud_cmp(ud, 0, hoge, MAX/2 -2) == 0);
	assert(ud_cmp(ud, MAX/2 -2, hoge, 3) == 0);
	assert(ud_cmp(ud, MAX/2 +1, &hoge[MAX/2 -2], 1) == 0);
	assert(ud_cmp(ud, MAX/2 +2, &hoge[MAX-1], 1) == 0);
	assert(ud_cmp(ud, MAX/2 +3, &hoge[MAX/2 -1], 1) == 0);
	assert(UCharDeque_size(ud) == MAX/2 + 4);
	assert(UCharDeque_verify(ud));
	/* 1個指定 */
	UCharDeque_erase(ud, UCharDeque_size(ud) - 2, 1);
	assert(ud_cmp(ud, 0, hoge, MAX/2 -2) == 0);
	assert(ud_cmp(ud, MAX/2 -2, hoge, 3) == 0);
	assert(ud_cmp(ud, MAX/2 +1, &hoge[MAX/2 -2], 2) == 0);
	assert(UCharDeque_size(ud) == MAX/2 + 3);
	assert(UCharDeque_verify(ud));
	/* 後ろ寄り */
	UCharDeque_erase(ud, UCharDeque_size(ud) - 5, 3);
	assert(ud_cmp(ud, 0, hoge, MAX/2) == 0);
	assert(UCharDeque_size(ud) == MAX/2);
	assert(UCharDeque_verify(ud));

	/* MAXになるまで */
	assert(UCharDeque_insert_array(ud, 0, hoge, MAX - MAX/2));
	assert(UCharDeque_size(ud) == MAX);
	assert(UCharDeque_verify(ud));
	/* 先頭から全削除 */
	UCharDeque_erase(ud, 0, UCharDeque_size(ud));
	assert(UCharDeque_size(ud) == 0);
	assert(UCharDeque_verify(ud));

	HEAP_DUMP_OVERFLOW(&heap);
	UCharDeque_delete(ud);
}

void DequeTest_test_1_6(void)
{
	int i;
	UCharDeque *x;
	unsigned char b[] = {1, 2, 4, 43, 2, 54, 1, 0, 2, 24};
	printf("***** test_1_6 *****\n");
	ud = UCharDeque_new();
	for (i = 0; i < MAX; i++) {
		assert(UCharDeque_push_back(ud, hoge[i]));
	}
	/* resize */
	UCharDeque_clear(ud);
	assert(UCharDeque_resize(ud, 64, 100));
	assert(UCharDeque_verify(ud));
	assert(UCharDeque_size(ud) == 64);
	for (i = 0; i < UCharDeque_size(ud); i++) {
		assert(*UCharDeque_at(ud, i) == 100);
	}
	assert(UCharDeque_resize(ud, 16, 99));
	assert(UCharDeque_verify(ud));
	assert(UCharDeque_size(ud) == 16);
	for (i = 0; i < UCharDeque_size(ud); i++) {
		assert(*UCharDeque_at(ud, i) == 100);
	}
	assert(UCharDeque_resize(ud, 32, 99));
	assert(UCharDeque_verify(ud));
	for (i = 0; i < UCharDeque_size(ud); i++) {
		if (i < 16) {
			assert(*UCharDeque_at(ud, i) == 100);
		} else {
			assert(*UCharDeque_at(ud, i) == 99);
		}
	}
	assert(UCharDeque_size(ud) == 32);
/*    assert(UCharDeque_resize(ud, MAX+1, 99) == 0);*/
	assert(UCharDeque_resize(ud, 0, 100));
	assert(UCharDeque_size(ud) == 0);
	/* swap */
	x = UCharDeque_new();
	UCharDeque_clear(ud);
	assert(UCharDeque_insert_array(ud, 0, hoge, MAX));
	assert(ud_cmp(ud, 0, hoge, MAX) == 0);
	assert(UCharDeque_size(ud) == MAX);
	UCharDeque_clear(x);
	assert(UCharDeque_insert_array(x, 0, b, sizeof b));
	assert(ud_cmp(x, 0, b, sizeof b) == 0);
	assert(UCharDeque_size(x) == sizeof b);

	assert(UCharDeque_verify(ud));
	assert(UCharDeque_verify(x));
	UCharDeque_swap(ud, x);
	assert(UCharDeque_verify(ud));
	assert(UCharDeque_verify(x));

	assert(ud_cmp(ud, 0, b, sizeof b) == 0);
	assert(UCharDeque_size(ud) == sizeof b);
	assert(ud_cmp(x, 0, hoge, MAX) == 0);
	assert(UCharDeque_size(x) == MAX);
	/* insert_range */
	UCharDeque_clear(ud);
	UCharDeque_clear(x);
	assert(UCharDeque_insert_array(ud, 0, hoge, 15));
	assert(UCharDeque_insert_array(x, 0, b, sizeof b));
	assert(UCharDeque_insert_range(ud, 0, x, 0, sizeof b));
	assert(UCharDeque_verify(ud));
	assert(UCharDeque_verify(x));
	for (i = 0; i < UCharDeque_size(ud); i++) {
		if (i < sizeof b) {
			assert(*UCharDeque_at(ud, i) == *UCharDeque_at(x, i));
		} else {
			assert(*UCharDeque_at(ud, i) == hoge[i - sizeof b]);
		}
		printf("%02d ", *UCharDeque_at(ud, i));
	}
	printf("\n");
	UCharDeque_clear(ud);
	assert(UCharDeque_insert_array(ud, 0, hoge, 15));
	for (i = 0; i < UCharDeque_size(ud); i++) {
		printf("%02d ", *UCharDeque_at(ud, i));
	}
	printf("\n");

	assert(UCharDeque_insert_range(ud, 0, ud, 1, 4));
	assert(UCharDeque_verify(ud));
	for (i = 0; i < UCharDeque_size(ud); i++) {
		if (i < 4) {
			assert(*UCharDeque_at(ud, i) == hoge[1 + i]);
		} else {
			assert(*UCharDeque_at(ud, i) == hoge[i - 4]);
		}
		printf("%02d ", *UCharDeque_at(ud, i));
	}
	printf("\n");
	UCharDeque_clear(ud);
	assert(UCharDeque_insert_array(ud, 0, hoge, 15));
	assert(UCharDeque_insert_range(ud, 3, ud, 1, 4));
	assert(UCharDeque_verify(ud));
	for (i = 0; i < UCharDeque_size(ud); i++) {
		if (i < 3) {
			assert(*UCharDeque_at(ud, i) == hoge[i]);
		} else if (3 <= i && i < 3 + 4) {
			assert(*UCharDeque_at(ud, i) == hoge[1 + i - 3]);
		} else {
			assert(*UCharDeque_at(ud, i) == hoge[i - 4]);
		}
		printf("%02d ", *UCharDeque_at(ud, i));
	}
	printf("\n");
	UCharDeque_clear(ud);
	assert(UCharDeque_insert_array(ud, 0, hoge, 15));
	assert(UCharDeque_insert_range(ud, 5, ud, 1, 4));
	assert(UCharDeque_verify(ud));
	for (i = 0; i < UCharDeque_size(ud); i++) {
		if (i < 5) {
			assert(*UCharDeque_at(ud, i) == hoge[i]);
		} else if (5 <= i && i < 5 + 4) {
			assert(*UCharDeque_at(ud, i) == hoge[1 + i - 5]);
		} else {
			assert(*UCharDeque_at(ud, i) == hoge[i - 4]);
		}
		printf("%02d ", *UCharDeque_at(ud, i));
	}
	printf("\n");



	HEAP_DUMP_OVERFLOW(&heap);
	UCharDeque_delete(ud);
	UCharDeque_delete(x);
}



void DequeTest_test_2_1(void)
{
	int i;
	printf("***** test_2_1 *****\n");
	id = IntDeque_new();
	/* 初期状態 */
/*    assert(IntDeque_max_size(id) == MAX);*/
	assert(IntDeque_size(id) == 0);
	assert(IntDeque_empty(id));

	assert(IntDeque_size(id) == 0);
	/* insert_array */
	IntDeque_clear(id);
	assert(IntDeque_insert_array(id, 0, piyo, MAX));
	assert(id_cmp(id, 0, piyo, MAX) == 0);
	assert(IntDeque_size(id) == MAX);
	assert(!IntDeque_empty(id));
	/* at */
	for (i = 0; i < IntDeque_size(id); i++) {
		assert(*IntDeque_at(id, i) == piyo[i]);
		*IntDeque_at(id, i) = piyo[MAX - i -1];
		assert(*IntDeque_at(id, i) == piyo[MAX - i -1]);
	}
	/* clear */
	IntDeque_clear(id);
	assert(IntDeque_size(id) == 0);
	assert(IntDeque_empty(id));
	/* insert_array */
	IntDeque_clear(id);
	assert(IntDeque_insert_array(id, 0, piyo, MAX/2));
	assert(id_cmp(id, 0, piyo, MAX/2) == 0);
	assert(IntDeque_size(id) == MAX/2);
	assert(!IntDeque_empty(id));
	/* clear */
	IntDeque_clear(id);
	assert(IntDeque_size(id) == 0);

	HEAP_DUMP_OVERFLOW(&heap);
	IntDeque_delete(id);
}

void DequeTest_test_2_2(void)
{
	int i;
	size_t size;
	int c = 0;
	printf("***** test_2_2 *****\n");
	id = IntDeque_new();
	assert(IntDeque_size(id) == 0);
	/* push_back */
	/* MAXまで */
	for (i = 0; i < MAX; i++) {
		size = IntDeque_size(id);
		assert(IntDeque_push_back(id, piyo[i]));
		assert(IntDeque_size(id) == size + 1);
	}
	assert(id_cmp(id, 0, piyo, MAX) == 0);
	assert(IntDeque_size(id) == MAX);
	assert(!IntDeque_empty(id));
	/* front */
	assert(IntDeque_front(id) == piyo[0]);
	/* back */
	assert(IntDeque_back(id) == piyo[MAX-1]);
	/* pop_front */
	size = IntDeque_size(id);
	assert(IntDeque_pop_front(id) == piyo[0]);
	assert(IntDeque_size(id) == size - 1);
	/* push_back */
	size = IntDeque_size(id);
	assert(IntDeque_push_back(id, piyo[0]));
	assert(IntDeque_size(id) == size + 1);
	assert(id_cmp(id, 0, &piyo[1], MAX-1) == 0);
	assert(id_cmp(id, MAX-1, piyo, 1) == 0);
	assert(IntDeque_size(id) == MAX);
	assert(!IntDeque_empty(id));
	/* pop_front */
	for (i = 0; i < MAX; i++) {
		size = IntDeque_size(id);
		if (i < MAX-1) {
			assert(IntDeque_pop_front(id) == piyo[i+1]);
		} else {
			assert(IntDeque_pop_front(id) == piyo[0]);
		}
		assert(IntDeque_size(id) == size - 1);
	}
	assert(IntDeque_size(id) == 0);
	assert(IntDeque_empty(id));
	/* 大量にpush_back */
	while (c < 10000000 && IntDeque_push_back(id, 0xff)) {
		c++;
	}
/*    printf("size: %d, c: %d\n", IntDeque_size(id), c);*/
	assert(IntDeque_size(id) == c);
	assert(!IntDeque_empty(id));

	HEAP_DUMP_OVERFLOW(&heap);
	IntDeque_delete(id);
}

void DequeTest_test_2_3(void)
{
	int i;
	size_t size;
	int c = 0;
	printf("***** test_2_3 *****\n");
	id = IntDeque_new();
	assert(IntDeque_size(id) == 0);
	/* push_front */
	/* MAXまで */
	for (i = 0; i < MAX; i++) {
		size = IntDeque_size(id);
		assert(IntDeque_push_front(id, piyo[MAX-i-1]));
		assert(IntDeque_size(id) == size + 1);
	}
	assert(id_cmp(id, 0, piyo, MAX) == 0);
	assert(IntDeque_size(id) == MAX);
	assert(!IntDeque_empty(id));
	/* front */
	assert(IntDeque_front(id) == piyo[0]);
	/* back */
	assert(IntDeque_back(id) == piyo[MAX-1]);
	/* pop_back */
	size = IntDeque_size(id);
	assert(IntDeque_pop_back(id) == piyo[MAX-1]);
	assert(IntDeque_pop_back(id) == piyo[MAX-2]);
	assert(IntDeque_size(id) == size - 2);
	/* push_front */
	size = IntDeque_size(id);
	assert(IntDeque_push_front(id, piyo[MAX-1]));
	assert(IntDeque_push_front(id, piyo[MAX-2]));
	assert(IntDeque_size(id) == size + 2);
	assert(id_cmp(id, 0, &piyo[MAX-2], 2) == 0);
	assert(id_cmp(id, 2, piyo, MAX-2) == 0);
	assert(IntDeque_size(id) == MAX);
	assert(!IntDeque_empty(id));
	/* pop_back */
	for (i = MAX-3; i >= 0; i--) {
		size = IntDeque_size(id);
		assert(IntDeque_pop_back(id) == piyo[i]);
		assert(IntDeque_size(id) == size - 1);
	}
	assert(IntDeque_pop_back(id) == piyo[MAX-1]);
	assert(IntDeque_pop_back(id) == piyo[MAX-2]);

	assert(IntDeque_size(id) == 0);
	assert(IntDeque_empty(id));
	/* 大量にpush_front */
	while (c < 10000000 && IntDeque_push_front(id, 0xff)) {
		c++;
	}
/*    printf("size: %d, c: %d\n", IntDeque_size(id), c);*/
	assert(IntDeque_size(id) == c);
	assert(!IntDeque_empty(id));

	HEAP_DUMP_OVERFLOW(&heap);
	IntDeque_delete(id);
}

void DequeTest_test_2_4(void)
{
	int i;
	printf("***** test_2_4 *****\n");
	id = IntDeque_new();
	assert(IntDeque_size(id) == 0);
	/* insert */
	/* 空状態 */
	for (i = 0; i < MAX/2; i++) {
		assert(id_cmp(id, 0, piyo, i) == 0);
		assert(IntDeque_insert(id, i, piyo[i]));
	}
	/* 先頭・前寄り */
	assert(IntDeque_insert(id, 0, piyo[0]));
	assert(IntDeque_insert(id, 1, piyo[1]));
	assert(IntDeque_insert(id, 2, piyo[2]));
	assert(id_cmp(id, 0, piyo, 3) == 0);
	assert(id_cmp(id, 3, piyo, MAX/2) == 0);
	assert(IntDeque_size(id) == MAX/2 + 3);

	/* erase */
	/* 先頭・前寄り */
	IntDeque_erase(id, 2, 1);
	assert(id_cmp(id, 0, piyo, 2) == 0);
	assert(id_cmp(id, 2, piyo, MAX/2) == 0);
	assert(IntDeque_size(id) == MAX/2 + 2);
	IntDeque_erase(id, 1, 1);
	assert(id_cmp(id, 0, piyo, 1) == 0);
	assert(id_cmp(id, 1, piyo, MAX/2) == 0);
	assert(IntDeque_size(id) == MAX/2 + 1);
	IntDeque_erase(id, 0, 1);
	assert(id_cmp(id, 0, piyo, MAX/2) == 0);
	assert(IntDeque_size(id) == MAX/2);

	/* insert */
	/* 末尾・後ろ寄り */
	assert(IntDeque_insert(id, MAX/2, piyo[0]));
	assert(IntDeque_insert(id, MAX/2, piyo[1]));
	assert(IntDeque_insert(id, MAX/2, piyo[2]));
	assert(id_cmp(id, 0, piyo, MAX/2) == 0);
	assert(*IntDeque_at(id, MAX/2 + 0) == piyo[2]);
	assert(*IntDeque_at(id, MAX/2 + 1) == piyo[1]);
	assert(*IntDeque_at(id, MAX/2 + 2) == piyo[0]);

	/* erase */
	/* 末尾・後ろ寄り */
	IntDeque_erase(id, MAX/2, 1);
	assert(id_cmp(id, 0, piyo, MAX/2) == 0);
	assert(*IntDeque_at(id, MAX/2 + 0) == piyo[1]);
	assert(*IntDeque_at(id, MAX/2 + 1) == piyo[0]);
	assert(IntDeque_size(id) == MAX/2 + 2);
	IntDeque_erase(id, MAX/2, 1);
	assert(id_cmp(id, 0, piyo, MAX/2) == 0);
	assert(*IntDeque_at(id, MAX/2 + 0) == piyo[0]);
	assert(IntDeque_size(id) == MAX/2 + 1);
	IntDeque_erase(id, MAX/2, 1);
	assert(id_cmp(id, 0, piyo, MAX/2) == 0);
	assert(IntDeque_size(id) == MAX/2);

	/* 先頭から全削除 */
	for (i = 0; i < MAX/2; i++) {
		assert(id_cmp(id, 0, &piyo[i], MAX/2 - i) == 0);
		IntDeque_erase(id, 0, 1);
	}
	assert(IntDeque_size(id) == 0);

	for (i = 0; i < MAX/2; i++) {
		assert(id_cmp(id, 0, piyo, i) == 0);
		assert(IntDeque_insert(id, i, piyo[i]));
	}
	assert(IntDeque_size(id) == MAX/2);

	/* 末尾から全削除 */
	for (i = 0; i < MAX/2; i++) {
		assert(id_cmp(id, 0, piyo, MAX/2 - i) == 0);
		IntDeque_erase(id, IntDeque_size(id) -1, 1);
	}
	assert(IntDeque_size(id) == 0);

	HEAP_DUMP_OVERFLOW(&heap);
	IntDeque_delete(id);
}

void DequeTest_test_2_5(void)
{
	printf("***** test_2_5 *****\n");
	id = IntDeque_new();
	assert(IntDeque_size(id) == 0);
	/* insert_array */
	/* 空状態 */
	assert(IntDeque_insert_array(id, 0, piyo, MAX/2));
	assert(id_cmp(id, 0, piyo, MAX/2) == 0);
	assert(IntDeque_size(id) == MAX/2);

	/* insert_array */
	/* 先頭 */
	assert(IntDeque_insert_array(id, 0, piyo, 3));
	assert(id_cmp(id, 0, piyo, 3) == 0);
	assert(id_cmp(id, 3, piyo, MAX/2) == 0);
	assert(IntDeque_size(id) == MAX/2 + 3);
	/* erase */
	IntDeque_erase(id, 0, 3);
	assert(id_cmp(id, 0, piyo, MAX/2) == 0);
	assert(IntDeque_size(id) == MAX/2);

	/* insert_array */
	/* 前寄り */
	assert(IntDeque_insert_array(id, 2, piyo, 3));
	assert(id_cmp(id, 0, piyo, 2) == 0);
	assert(id_cmp(id, 2, piyo, 3) == 0);
	assert(id_cmp(id, 5, &piyo[2], MAX/2 -2) == 0);
	assert(IntDeque_size(id) == MAX/2 + 3);
	/* 0個指定 */
	assert(IntDeque_insert_array(id, 1, &piyo[MAX/2], 0));
	assert(id_cmp(id, 0, piyo, 2) == 0);
	assert(id_cmp(id, 2, piyo, 3) == 0);
	assert(id_cmp(id, 5, &piyo[2], MAX/2 -2) == 0);
	assert(IntDeque_size(id) == MAX/2 + 3);
	/* 1個指定 */
	assert(IntDeque_insert_array(id, 1, &piyo[MAX-1], 1));
	assert(id_cmp(id, 0, &piyo[0], 1) == 0);
	assert(id_cmp(id, 1, &piyo[MAX-1], 1) == 0);
	assert(id_cmp(id, 2, &piyo[1], 1) == 0);
	assert(id_cmp(id, 3, &piyo[0], 3) == 0);
	assert(id_cmp(id, 6, &piyo[2], MAX/2 -2) == 0);
	assert(IntDeque_size(id) == MAX/2 + 4);
	/* erase */
	/* 0個指定 */
	IntDeque_erase(id, 1, 0);
	assert(id_cmp(id, 0, &piyo[0], 1) == 0);
	assert(id_cmp(id, 1, &piyo[MAX-1], 1) == 0);
	assert(id_cmp(id, 2, &piyo[1], 1) == 0);
	assert(id_cmp(id, 3, &piyo[0], 3) == 0);
	assert(id_cmp(id, 6, &piyo[2], MAX/2 -2) == 0);
	assert(IntDeque_size(id) == MAX/2 + 4);
	/* 1個指定 */
	IntDeque_erase(id, 1, 1);
	assert(id_cmp(id, 0, piyo, 2) == 0);
	assert(id_cmp(id, 2, piyo, 3) == 0);
	assert(id_cmp(id, 5, &piyo[2], MAX/2 -2) == 0);
	assert(IntDeque_size(id) == MAX/2 + 3);
	/* 前寄り */
	IntDeque_erase(id, 2, 3);
	assert(id_cmp(id, 0, piyo, MAX/2) == 0);
	assert(IntDeque_size(id) == MAX/2);

	/* insert_array */
	/* 末尾 */
	assert(IntDeque_insert_array(id, IntDeque_size(id), piyo, 3));
	assert(id_cmp(id, 0, piyo, MAX/2) == 0);
	assert(id_cmp(id, MAX/2, piyo, 3) == 0);
	assert(IntDeque_size(id) == MAX/2 + 3);
	/* erase */
	IntDeque_erase(id, IntDeque_size(id) - 3, 3);
	assert(id_cmp(id, 0, piyo, MAX/2) == 0);
	assert(IntDeque_size(id) == MAX/2);

	/* insert_array */
	/* 後ろ寄り */
	assert(IntDeque_insert_array(id, IntDeque_size(id) - 2, piyo, 3));
	assert(id_cmp(id, 0, piyo, MAX/2 -2) == 0);
	assert(id_cmp(id, MAX/2 -2, piyo, 3) == 0);
	assert(id_cmp(id, MAX/2 +1, &piyo[MAX/2 -2], 2) == 0);
	assert(IntDeque_size(id) == MAX/2 + 3);
	/* 0個指定 */
	assert(IntDeque_insert_array(id, IntDeque_size(id) - 1, &piyo[MAX-1], 0));
	assert(id_cmp(id, 0, piyo, MAX/2 -2) == 0);
	assert(id_cmp(id, MAX/2 -2, piyo, 3) == 0);
	assert(id_cmp(id, MAX/2 +1, &piyo[MAX/2 -2], 2) == 0);
	assert(IntDeque_size(id) == MAX/2 + 3);
	/* 1個指定 */
	assert(IntDeque_insert_array(id, IntDeque_size(id) - 1, &piyo[MAX-1], 1));
	assert(id_cmp(id, 0, piyo, MAX/2 -2) == 0);
	assert(id_cmp(id, MAX/2 -2, piyo, 3) == 0);
	assert(id_cmp(id, MAX/2 +1, &piyo[MAX/2 -2], 1) == 0);
	assert(id_cmp(id, MAX/2 +2, &piyo[MAX-1], 1) == 0);
	assert(id_cmp(id, MAX/2 +3, &piyo[MAX/2 -1], 1) == 0);
	assert(IntDeque_size(id) == MAX/2 + 4);
	/* erase */
	/* 0個指定 */
	IntDeque_erase(id, IntDeque_size(id) - 2, 0);
	assert(id_cmp(id, 0, piyo, MAX/2 -2) == 0);
	assert(id_cmp(id, MAX/2 -2, piyo, 3) == 0);
	assert(id_cmp(id, MAX/2 +1, &piyo[MAX/2 -2], 1) == 0);
	assert(id_cmp(id, MAX/2 +2, &piyo[MAX-1], 1) == 0);
	assert(id_cmp(id, MAX/2 +3, &piyo[MAX/2 -1], 1) == 0);
	assert(IntDeque_size(id) == MAX/2 + 4);
	/* 1個指定 */
	IntDeque_erase(id, IntDeque_size(id) - 2, 1);
	assert(id_cmp(id, 0, piyo, MAX/2 -2) == 0);
	assert(id_cmp(id, MAX/2 -2, piyo, 3) == 0);
	assert(id_cmp(id, MAX/2 +1, &piyo[MAX/2 -2], 2) == 0);
	assert(IntDeque_size(id) == MAX/2 + 3);
	/* 後ろ寄り */
	IntDeque_erase(id, IntDeque_size(id) - 5, 3);
	assert(id_cmp(id, 0, piyo, MAX/2) == 0);
	assert(IntDeque_size(id) == MAX/2);

	/* MAXになるまで */
	assert(IntDeque_insert_array(id, 0, piyo, MAX - MAX/2));
	assert(IntDeque_size(id) == MAX);
	/* 先頭から全削除 */
	IntDeque_erase(id, 0, IntDeque_size(id));
	assert(IntDeque_size(id) == 0);

	HEAP_DUMP_OVERFLOW(&heap);
	IntDeque_delete(id);
}






void DequeTest_run(void)
{
	printf("\n===== deque test =====\n");
	deque_init_hoge();
	deque_init_piyo();

	DequeTest_test_1_1();
	DequeTest_test_1_2();
	DequeTest_test_1_3();
	DequeTest_test_1_4();
	DequeTest_test_1_5();
	DequeTest_test_1_6();

	DequeTest_test_2_1();
	DequeTest_test_2_2();
	DequeTest_test_2_3();
	DequeTest_test_2_4();
	DequeTest_test_2_5();
}


int main(void)
{
#ifdef MY_MALLOC
	Heap_init(&heap, buf, sizeof buf, sizeof buf[0]);
#endif
	DequeTest_run();
#ifdef MY_MALLOC
	HEAP_DUMP_LEAK(&heap, 0);
#endif
	return 0;
}
