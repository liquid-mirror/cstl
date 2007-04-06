#include "test.h"



RING_IMPLEMENT(UCharRing, unsigned char)
RING_IMPLEMENT(IntRing, int)

static UCharRing *ud;
static IntRing *id;

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

int ud_cmp(UCharRing *x, size_t idx, unsigned char *buf, size_t n)
{
	int i, j;
	for (i = idx, j = 0; i < idx + n; i++, j++) {
/*        printf("%d, %d\n", *UCharRing_at(x, i), buf[j]);*/
		if (*UCharRing_at(x, i) != buf[j]) return 1;
	}
	return 0;
}

int id_cmp(IntRing *x, size_t idx, int *buf, size_t n)
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
	ud = UCharRing_new(MAX);
	/* 初期状態 */
	assert(UCharRing_max_size(ud) == MAX);
	assert(UCharRing_size(ud) == 0);
	assert(UCharRing_empty(ud));
	assert(!UCharRing_full(ud));
	for (j = 0; j < MAX+1; j++) {
		ud->begin = ud->end = j;
		assert(UCharRing_size(ud) == 0);
		/* insert_array */
		UCharRing_clear(ud);
		assert(UCharRing_insert_array(ud, 0, hoge, MAX));
		assert(ud_cmp(ud, 0, hoge, MAX) == 0);
		assert(UCharRing_size(ud) == MAX);
		assert(!UCharRing_empty(ud));
		assert(UCharRing_full(ud));
		/* at */
		for (i = 0; i < UCharRing_size(ud); i++) {
			assert(*UCharRing_at(ud, i) == hoge[i]);
			*UCharRing_at(ud, i) = hoge[MAX - i -1];
			assert(*UCharRing_at(ud, i) == hoge[MAX - i -1]);
		}
		/* clear */
		UCharRing_clear(ud);
		assert(UCharRing_size(ud) == 0);
		assert(UCharRing_empty(ud));
		assert(!UCharRing_full(ud));
		/* insert_array */
		UCharRing_clear(ud);
		assert(UCharRing_insert_array(ud, 0, hoge, MAX/2));
		assert(ud_cmp(ud, 0, hoge, MAX/2) == 0);
		assert(UCharRing_size(ud) == MAX/2);
		assert(!UCharRing_empty(ud));
		assert(!UCharRing_full(ud));
		/* clear */
		UCharRing_clear(ud);
		assert(UCharRing_size(ud) == 0);
	}

	UCharRing_delete(ud);
}

void RingTest_test_1_2(void)
{
	int i, j;
	size_t size;
	printf("***** test_1_2 *****\n");
	ud = UCharRing_new(MAX);
	for (j = 0; j < MAX+1; j++) {
		ud->begin = ud->end = j;
		assert(UCharRing_size(ud) == 0);
		/* push_back */
		/* MAXまで */
		for (i = 0; i < MAX; i++) {
			size = UCharRing_size(ud);
			assert(UCharRing_push_back(ud, hoge[i]));
			assert(UCharRing_size(ud) == size + 1);
		}
		assert(ud_cmp(ud, 0, hoge, MAX) == 0);
		assert(UCharRing_size(ud) == MAX);
		assert(!UCharRing_empty(ud));
		assert(UCharRing_full(ud));
		/* FULLで失敗 */
		assert(!UCharRing_push_back(ud, hoge[0]));
		assert(UCharRing_size(ud) == MAX);
		assert(!UCharRing_empty(ud));
		assert(UCharRing_full(ud));
		/* front */
		assert(UCharRing_front(ud) == hoge[0]);
		/* back */
		assert(UCharRing_back(ud) == hoge[MAX-1]);
		/* pop_front */
		size = UCharRing_size(ud);
		assert(UCharRing_pop_front(ud) == hoge[0]);
		assert(UCharRing_size(ud) == size - 1);
		/* push_back */
		size = UCharRing_size(ud);
		assert(UCharRing_push_back(ud, hoge[0]));
		assert(UCharRing_size(ud) == size + 1);
		assert(ud_cmp(ud, 0, &hoge[1], MAX-1) == 0);
		assert(ud_cmp(ud, MAX-1, hoge, 1) == 0);
		assert(UCharRing_size(ud) == MAX);
		assert(!UCharRing_empty(ud));
		assert(UCharRing_full(ud));
		/* pop_front */
		for (i = 0; i < MAX; i++) {
			size = UCharRing_size(ud);
			if (i < MAX-1) {
				assert(UCharRing_pop_front(ud) == hoge[i+1]);
			} else {
				assert(UCharRing_pop_front(ud) == hoge[0]);
			}
			assert(UCharRing_size(ud) == size - 1);
		}
		assert(UCharRing_size(ud) == 0);
		assert(UCharRing_empty(ud));
		assert(!UCharRing_full(ud));
	}

	UCharRing_delete(ud);
}

void RingTest_test_1_3(void)
{
	int i, j;
	size_t size;
	printf("***** test_1_3 *****\n");
	ud = UCharRing_new(MAX);
	for (j = 0; j < MAX+1; j++) {
		ud->begin = ud->end = j;
		assert(UCharRing_size(ud) == 0);
		/* push_front */
		/* MAXまで */
		for (i = 0; i < MAX; i++) {
			size = UCharRing_size(ud);
			assert(UCharRing_push_front(ud, hoge[MAX-i-1]));
			assert(UCharRing_size(ud) == size + 1);
		}
		assert(ud_cmp(ud, 0, hoge, MAX) == 0);
		assert(UCharRing_size(ud) == MAX);
		assert(!UCharRing_empty(ud));
		assert(UCharRing_full(ud));
		/* FULLで失敗 */
		assert(!UCharRing_push_front(ud, hoge[0]));
		assert(UCharRing_size(ud) == MAX);
		assert(!UCharRing_empty(ud));
		assert(UCharRing_full(ud));
		/* front */
		assert(UCharRing_front(ud) == hoge[0]);
		/* back */
		assert(UCharRing_back(ud) == hoge[MAX-1]);
		/* pop_back */
		size = UCharRing_size(ud);
		assert(UCharRing_pop_back(ud) == hoge[MAX-1]);
		assert(UCharRing_pop_back(ud) == hoge[MAX-2]);
		assert(UCharRing_size(ud) == size - 2);
		/* push_front */
		size = UCharRing_size(ud);
		assert(UCharRing_push_front(ud, hoge[MAX-1]));
		assert(UCharRing_push_front(ud, hoge[MAX-2]));
		assert(UCharRing_size(ud) == size + 2);
		assert(ud_cmp(ud, 0, &hoge[MAX-2], 2) == 0);
		assert(ud_cmp(ud, 2, hoge, MAX-2) == 0);
		assert(UCharRing_size(ud) == MAX);
		assert(!UCharRing_empty(ud));
		assert(UCharRing_full(ud));
		/* pop_back */
		for (i = MAX-3; i >= 0; i--) {
			size = UCharRing_size(ud);
			assert(UCharRing_pop_back(ud) == hoge[i]);
			assert(UCharRing_size(ud) == size - 1);
		}
		assert(UCharRing_pop_back(ud) == hoge[MAX-1]);
		assert(UCharRing_pop_back(ud) == hoge[MAX-2]);

		assert(UCharRing_size(ud) == 0);
		assert(UCharRing_empty(ud));
		assert(!UCharRing_full(ud));
	}

	UCharRing_delete(ud);
}

void RingTest_test_1_4(void)
{
	int i, j;
	printf("***** test_1_4 *****\n");
	ud = UCharRing_new(MAX);
	for (j = 0; j < MAX+1; j++) {
		ud->begin = ud->end = j;
		assert(UCharRing_size(ud) == 0);
		/* insert */
		/* 空状態 */
		for (i = 0; i < MAX/2; i++) {
			assert(ud_cmp(ud, 0, hoge, i) == 0);
			assert(UCharRing_insert(ud, i, hoge[i]));
		}
		/* 先頭・前寄り */
		assert(UCharRing_insert(ud, 0, hoge[0]));
		assert(UCharRing_insert(ud, 1, hoge[1]));
		assert(UCharRing_insert(ud, 2, hoge[2]));
		assert(ud_cmp(ud, 0, hoge, 3) == 0);
		assert(ud_cmp(ud, 3, hoge, MAX/2) == 0);
		assert(UCharRing_size(ud) == MAX/2 + 3);

		/* erase */
		/* 先頭・前寄り */
		UCharRing_erase(ud, 2, 1);
		assert(ud_cmp(ud, 0, hoge, 2) == 0);
		assert(ud_cmp(ud, 2, hoge, MAX/2) == 0);
		assert(UCharRing_size(ud) == MAX/2 + 2);
		UCharRing_erase(ud, 1, 1);
		assert(ud_cmp(ud, 0, hoge, 1) == 0);
		assert(ud_cmp(ud, 1, hoge, MAX/2) == 0);
		assert(UCharRing_size(ud) == MAX/2 + 1);
		UCharRing_erase(ud, 0, 1);
		assert(ud_cmp(ud, 0, hoge, MAX/2) == 0);
		assert(UCharRing_size(ud) == MAX/2);

		/* insert */
		/* 末尾・後ろ寄り */
		assert(UCharRing_insert(ud, MAX/2, hoge[0]));
		assert(UCharRing_insert(ud, MAX/2, hoge[1]));
		assert(UCharRing_insert(ud, MAX/2, hoge[2]));
		assert(ud_cmp(ud, 0, hoge, MAX/2) == 0);
		assert(*UCharRing_at(ud, MAX/2 + 0) == hoge[2]);
		assert(*UCharRing_at(ud, MAX/2 + 1) == hoge[1]);
		assert(*UCharRing_at(ud, MAX/2 + 2) == hoge[0]);

		/* erase */
		/* 末尾・後ろ寄り */
		UCharRing_erase(ud, MAX/2, 1);
		assert(ud_cmp(ud, 0, hoge, MAX/2) == 0);
		assert(*UCharRing_at(ud, MAX/2 + 0) == hoge[1]);
		assert(*UCharRing_at(ud, MAX/2 + 1) == hoge[0]);
		assert(UCharRing_size(ud) == MAX/2 + 2);
		UCharRing_erase(ud, MAX/2, 1);
		assert(ud_cmp(ud, 0, hoge, MAX/2) == 0);
		assert(*UCharRing_at(ud, MAX/2 + 0) == hoge[0]);
		assert(UCharRing_size(ud) == MAX/2 + 1);
		UCharRing_erase(ud, MAX/2, 1);
		assert(ud_cmp(ud, 0, hoge, MAX/2) == 0);
		assert(UCharRing_size(ud) == MAX/2);

		/* 先頭から全削除 */
		for (i = 0; i < MAX/2; i++) {
			assert(ud_cmp(ud, 0, &hoge[i], MAX/2 - i) == 0);
			UCharRing_erase(ud, 0, 1);
		}
		assert(UCharRing_size(ud) == 0);

		for (i = 0; i < MAX/2; i++) {
			assert(ud_cmp(ud, 0, hoge, i) == 0);
			assert(UCharRing_insert(ud, i, hoge[i]));
		}
		assert(UCharRing_size(ud) == MAX/2);

		/* 末尾から全削除 */
		for (i = 0; i < MAX/2; i++) {
			assert(ud_cmp(ud, 0, hoge, MAX/2 - i) == 0);
			UCharRing_erase(ud, UCharRing_size(ud) -1, 1);
		}
		assert(UCharRing_size(ud) == 0);

	}

	UCharRing_delete(ud);
}

void RingTest_test_1_5(void)
{
	int i;
	printf("***** test_1_5 *****\n");
	ud = UCharRing_new(MAX);
	for (i = 0; i < MAX+1; i++) {
		ud->begin = ud->end = i;
		assert(UCharRing_size(ud) == 0);
		/* insert_array */
		/* 空状態 */
		assert(UCharRing_insert_array(ud, 0, hoge, MAX/2));
		assert(ud_cmp(ud, 0, hoge, MAX/2) == 0);
		assert(UCharRing_size(ud) == MAX/2);

		/* insert_array */
		/* 先頭 */
		assert(UCharRing_insert_array(ud, 0, hoge, 3));
		assert(ud_cmp(ud, 0, hoge, 3) == 0);
		assert(ud_cmp(ud, 3, hoge, MAX/2) == 0);
		assert(UCharRing_size(ud) == MAX/2 + 3);
		/* erase */
		UCharRing_erase(ud, 0, 3);
		assert(ud_cmp(ud, 0, hoge, MAX/2) == 0);
		assert(UCharRing_size(ud) == MAX/2);

		/* insert_array */
		/* 前寄り */
		assert(UCharRing_insert_array(ud, 2, hoge, 3));
		assert(ud_cmp(ud, 0, hoge, 2) == 0);
		assert(ud_cmp(ud, 2, hoge, 3) == 0);
		assert(ud_cmp(ud, 5, &hoge[2], MAX/2 -2) == 0);
		assert(UCharRing_size(ud) == MAX/2 + 3);
		/* 0個指定 */
		assert(UCharRing_insert_array(ud, 1, &hoge[MAX/2], 0));
		assert(ud_cmp(ud, 0, hoge, 2) == 0);
		assert(ud_cmp(ud, 2, hoge, 3) == 0);
		assert(ud_cmp(ud, 5, &hoge[2], MAX/2 -2) == 0);
		assert(UCharRing_size(ud) == MAX/2 + 3);
		/* 1個指定 */
		assert(UCharRing_insert_array(ud, 1, &hoge[MAX-1], 1));
		assert(ud_cmp(ud, 0, &hoge[0], 1) == 0);
		assert(ud_cmp(ud, 1, &hoge[MAX-1], 1) == 0);
		assert(ud_cmp(ud, 2, &hoge[1], 1) == 0);
		assert(ud_cmp(ud, 3, &hoge[0], 3) == 0);
		assert(ud_cmp(ud, 6, &hoge[2], MAX/2 -2) == 0);
		assert(UCharRing_size(ud) == MAX/2 + 4);
		/* erase */
		/* 0個指定 */
		UCharRing_erase(ud, 1, 0);
		assert(ud_cmp(ud, 0, &hoge[0], 1) == 0);
		assert(ud_cmp(ud, 1, &hoge[MAX-1], 1) == 0);
		assert(ud_cmp(ud, 2, &hoge[1], 1) == 0);
		assert(ud_cmp(ud, 3, &hoge[0], 3) == 0);
		assert(ud_cmp(ud, 6, &hoge[2], MAX/2 -2) == 0);
		assert(UCharRing_size(ud) == MAX/2 + 4);
		/* 1個指定 */
		UCharRing_erase(ud, 1, 1);
		assert(ud_cmp(ud, 0, hoge, 2) == 0);
		assert(ud_cmp(ud, 2, hoge, 3) == 0);
		assert(ud_cmp(ud, 5, &hoge[2], MAX/2 -2) == 0);
		assert(UCharRing_size(ud) == MAX/2 + 3);
		/* 前寄り */
		UCharRing_erase(ud, 2, 3);
		assert(ud_cmp(ud, 0, hoge, MAX/2) == 0);
		assert(UCharRing_size(ud) == MAX/2);

		/* insert_array */
		/* 末尾 */
		assert(UCharRing_insert_array(ud, UCharRing_size(ud), hoge, 3));
		assert(ud_cmp(ud, 0, hoge, MAX/2) == 0);
		assert(ud_cmp(ud, MAX/2, hoge, 3) == 0);
		assert(UCharRing_size(ud) == MAX/2 + 3);
		/* erase */
		UCharRing_erase(ud, UCharRing_size(ud) - 3, 3);
		assert(ud_cmp(ud, 0, hoge, MAX/2) == 0);
		assert(UCharRing_size(ud) == MAX/2);

		/* insert_array */
		/* 後ろ寄り */
		assert(UCharRing_insert_array(ud, UCharRing_size(ud) - 2, hoge, 3));
		assert(ud_cmp(ud, 0, hoge, MAX/2 -2) == 0);
		assert(ud_cmp(ud, MAX/2 -2, hoge, 3) == 0);
		assert(ud_cmp(ud, MAX/2 +1, &hoge[MAX/2 -2], 2) == 0);
		assert(UCharRing_size(ud) == MAX/2 + 3);
		/* 0個指定 */
		assert(UCharRing_insert_array(ud, UCharRing_size(ud) - 1, &hoge[MAX-1], 0));
		assert(ud_cmp(ud, 0, hoge, MAX/2 -2) == 0);
		assert(ud_cmp(ud, MAX/2 -2, hoge, 3) == 0);
		assert(ud_cmp(ud, MAX/2 +1, &hoge[MAX/2 -2], 2) == 0);
		assert(UCharRing_size(ud) == MAX/2 + 3);
		/* 1個指定 */
		assert(UCharRing_insert_array(ud, UCharRing_size(ud) - 1, &hoge[MAX-1], 1));
		assert(ud_cmp(ud, 0, hoge, MAX/2 -2) == 0);
		assert(ud_cmp(ud, MAX/2 -2, hoge, 3) == 0);
		assert(ud_cmp(ud, MAX/2 +1, &hoge[MAX/2 -2], 1) == 0);
		assert(ud_cmp(ud, MAX/2 +2, &hoge[MAX-1], 1) == 0);
		assert(ud_cmp(ud, MAX/2 +3, &hoge[MAX/2 -1], 1) == 0);
		assert(UCharRing_size(ud) == MAX/2 + 4);
		/* erase */
		/* 0個指定 */
		UCharRing_erase(ud, UCharRing_size(ud) - 2, 0);
		assert(ud_cmp(ud, 0, hoge, MAX/2 -2) == 0);
		assert(ud_cmp(ud, MAX/2 -2, hoge, 3) == 0);
		assert(ud_cmp(ud, MAX/2 +1, &hoge[MAX/2 -2], 1) == 0);
		assert(ud_cmp(ud, MAX/2 +2, &hoge[MAX-1], 1) == 0);
		assert(ud_cmp(ud, MAX/2 +3, &hoge[MAX/2 -1], 1) == 0);
		assert(UCharRing_size(ud) == MAX/2 + 4);
		/* 1個指定 */
		UCharRing_erase(ud, UCharRing_size(ud) - 2, 1);
		assert(ud_cmp(ud, 0, hoge, MAX/2 -2) == 0);
		assert(ud_cmp(ud, MAX/2 -2, hoge, 3) == 0);
		assert(ud_cmp(ud, MAX/2 +1, &hoge[MAX/2 -2], 2) == 0);
		assert(UCharRing_size(ud) == MAX/2 + 3);
		/* 後ろ寄り */
		UCharRing_erase(ud, UCharRing_size(ud) - 5, 3);
		assert(ud_cmp(ud, 0, hoge, MAX/2) == 0);
		assert(UCharRing_size(ud) == MAX/2);

		/* MAX以上で失敗 */
		assert(!UCharRing_insert_array(ud, 0, hoge, MAX - MAX/2 +1));
		assert(ud_cmp(ud, 0, hoge, MAX/2) == 0);
		assert(UCharRing_size(ud) == MAX/2);
		/* MAXになるまで */
		assert(UCharRing_insert_array(ud, 0, hoge, MAX - MAX/2));
		assert(UCharRing_size(ud) == MAX);
		assert(UCharRing_full(ud));
		/* 先頭から全削除 */
		UCharRing_erase(ud, 0, UCharRing_size(ud));
		assert(UCharRing_size(ud) == 0);
	}

	UCharRing_delete(ud);
}

void RingTest_test_1_6(void)
{
	int i;
	UCharRing *x;
	unsigned char b[] = {1, 2, 4, 43, 2, 54, 1, 0, 2, 24};
	printf("***** test_1_6 *****\n");
	ud = UCharRing_new(MAX);
	for (i = 0; i < MAX; i++) {
		assert(UCharRing_push_back(ud, hoge[i]));
	}
	/* assign */
	assert(UCharRing_size(ud) == MAX);
	x = UCharRing_new(MAX);
	assert(UCharRing_assign(x, ud, 0, UCharRing_size(ud)));
	assert(UCharRing_size(ud) == UCharRing_size(x));
	assert(UCharRing_max_size(ud) == UCharRing_max_size(x));
	for (i = 0; i < UCharRing_size(ud); i++) {
		assert(*UCharRing_at(x, i) == *UCharRing_at(ud, i));
	}
	assert(UCharRing_assign(x, x, 0, UCharRing_size(x)));
	assert(UCharRing_size(ud) == UCharRing_size(x));
	for (i = 0; i < UCharRing_size(ud); i++) {
		assert(*UCharRing_at(x, i) == *UCharRing_at(ud, i));
	}
	assert(UCharRing_assign(x, x, 1, 2));
	assert(UCharRing_size(x) == 2);
	assert(*UCharRing_at(x, 0) == *UCharRing_at(ud, 1));
	assert(*UCharRing_at(x, 1) == *UCharRing_at(ud, 2));
	assert(UCharRing_assign(x, x, 0, 0));
	assert(UCharRing_size(x) == 0);
	/* resize */
	UCharRing_clear(ud);
	assert(UCharRing_resize(ud, 64, 100));
	assert(UCharRing_size(ud) == 64);
	for (i = 0; i < UCharRing_size(ud); i++) {
		assert(*UCharRing_at(ud, i) == 100);
	}
	assert(UCharRing_resize(ud, 16, 99));
	assert(UCharRing_size(ud) == 16);
	for (i = 0; i < UCharRing_size(ud); i++) {
		assert(*UCharRing_at(ud, i) == 100);
	}
	assert(UCharRing_resize(ud, 32, 99));
	for (i = 0; i < UCharRing_size(ud); i++) {
		if (i < 16) {
			assert(*UCharRing_at(ud, i) == 100);
		} else {
			assert(*UCharRing_at(ud, i) == 99);
		}
	}
	assert(UCharRing_size(ud) == 32);
	assert(UCharRing_resize(ud, MAX+1, 99) == 0);
	assert(UCharRing_resize(ud, 0, 100));
	assert(UCharRing_size(ud) == 0);
	/* swap */
	UCharRing_clear(ud);
	assert(UCharRing_insert_array(ud, 0, hoge, MAX));
	assert(ud_cmp(ud, 0, hoge, MAX) == 0);
	assert(UCharRing_size(ud) == MAX);
	UCharRing_clear(x);
	assert(UCharRing_insert_array(x, 0, b, sizeof b));
	assert(ud_cmp(x, 0, b, sizeof b) == 0);
	assert(UCharRing_size(x) == sizeof b);

	UCharRing_swap(ud, x);

	assert(ud_cmp(ud, 0, b, sizeof b) == 0);
	assert(UCharRing_size(ud) == sizeof b);
	assert(ud_cmp(x, 0, hoge, MAX) == 0);
	assert(UCharRing_size(x) == MAX);


	UCharRing_delete(ud);
	UCharRing_delete(x);
}



void RingTest_test_2_1(void)
{
	int i, j;
	printf("***** test_2_1 *****\n");
	id = IntRing_new(MAX);
	/* 初期状態 */
	assert(IntRing_max_size(id) == MAX);
	assert(IntRing_size(id) == 0);
	assert(IntRing_empty(id));
	assert(!IntRing_full(id));
	for (j = 0; j < MAX+1; j++) {
		id->begin = id->end = j;
		assert(IntRing_size(id) == 0);
		/* insert_array */
		IntRing_clear(id);
		assert(IntRing_insert_array(id, 0, piyo, MAX));
		assert(id_cmp(id, 0, piyo, MAX) == 0);
		assert(IntRing_size(id) == MAX);
		assert(!IntRing_empty(id));
		assert(IntRing_full(id));
		/* at */
		for (i = 0; i < IntRing_size(id); i++) {
			assert(*IntRing_at(id, i) == hoge[i]);
			*IntRing_at(id, i) = hoge[MAX - i -1];
			assert(*IntRing_at(id, i) == hoge[MAX - i -1]);
		}
		/* clear */
		IntRing_clear(id);
		assert(IntRing_size(id) == 0);
		assert(IntRing_empty(id));
		assert(!IntRing_full(id));
		/* insert_array */
		IntRing_clear(id);
		assert(IntRing_insert_array(id, 0, piyo, MAX/2));
		assert(id_cmp(id, 0, piyo, MAX/2) == 0);
		assert(IntRing_size(id) == MAX/2);
		assert(!IntRing_empty(id));
		assert(!IntRing_full(id));
		/* clear */
		IntRing_clear(id);
		assert(IntRing_size(id) == 0);
	}

	IntRing_delete(id);
}

void RingTest_test_2_2(void)
{
	int i, j;
	size_t size;
	printf("***** test_2_2 *****\n");
	id = IntRing_new(MAX);
	for (j = 0; j < MAX+1; j++) {
		id->begin = id->end = j;
		assert(IntRing_size(id) == 0);
		/* push_back */
		/* MAXまで */
		for (i = 0; i < MAX; i++) {
			size = IntRing_size(id);
			assert(IntRing_push_back(id, piyo[i]));
			assert(IntRing_size(id) == size + 1);
		}
		assert(id_cmp(id, 0, piyo, MAX) == 0);
		assert(IntRing_size(id) == MAX);
		assert(!IntRing_empty(id));
		assert(IntRing_full(id));
		/* FULLで失敗 */
		assert(!IntRing_push_back(id, piyo[0]));
		assert(IntRing_size(id) == MAX);
		assert(!IntRing_empty(id));
		assert(IntRing_full(id));
		/* front */
		assert(IntRing_front(id) == piyo[0]);
		/* back */
		assert(IntRing_back(id) == piyo[MAX-1]);
		/* pop_front */
		size = IntRing_size(id);
		assert(IntRing_pop_front(id) == piyo[0]);
		assert(IntRing_size(id) == size - 1);
		/* push_back */
		size = IntRing_size(id);
		assert(IntRing_push_back(id, piyo[0]));
		assert(IntRing_size(id) == size + 1);
		assert(id_cmp(id, 0, &piyo[1], MAX-1) == 0);
		assert(id_cmp(id, MAX-1, piyo, 1) == 0);
		assert(IntRing_size(id) == MAX);
		assert(!IntRing_empty(id));
		assert(IntRing_full(id));
		/* pop_front */
		for (i = 0; i < MAX; i++) {
			size = IntRing_size(id);
			if (i < MAX-1) {
				assert(IntRing_pop_front(id) == piyo[i+1]);
			} else {
				assert(IntRing_pop_front(id) == piyo[0]);
			}
			assert(IntRing_size(id) == size - 1);
		}
		assert(IntRing_size(id) == 0);
		assert(IntRing_empty(id));
		assert(!IntRing_full(id));
	}

	IntRing_delete(id);
}

void RingTest_test_2_3(void)
{
	int i, j;
	size_t size;
	printf("***** test_2_3 *****\n");
	id = IntRing_new(MAX);
	for (j = 0; j < MAX+1; j++) {
		id->begin = id->end = j;
		assert(IntRing_size(id) == 0);
		/* push_front */
		/* MAXまで */
		for (i = 0; i < MAX; i++) {
			size = IntRing_size(id);
			assert(IntRing_push_front(id, piyo[MAX-i-1]));
			assert(IntRing_size(id) == size + 1);
		}
		assert(id_cmp(id, 0, piyo, MAX) == 0);
		assert(IntRing_size(id) == MAX);
		assert(!IntRing_empty(id));
		assert(IntRing_full(id));
		/* FULLで失敗 */
		assert(!IntRing_push_front(id, piyo[0]));
		assert(IntRing_size(id) == MAX);
		assert(!IntRing_empty(id));
		assert(IntRing_full(id));
		/* front */
		assert(IntRing_front(id) == piyo[0]);
		/* back */
		assert(IntRing_back(id) == piyo[MAX-1]);
		/* pop_back */
		size = IntRing_size(id);
		assert(IntRing_pop_back(id) == piyo[MAX-1]);
		assert(IntRing_pop_back(id) == piyo[MAX-2]);
		assert(IntRing_size(id) == size - 2);
		/* push_front */
		size = IntRing_size(id);
		assert(IntRing_push_front(id, piyo[MAX-1]));
		assert(IntRing_push_front(id, piyo[MAX-2]));
		assert(IntRing_size(id) == size + 2);
		assert(id_cmp(id, 0, &piyo[MAX-2], 2) == 0);
		assert(id_cmp(id, 2, piyo, MAX-2) == 0);
		assert(IntRing_size(id) == MAX);
		assert(!IntRing_empty(id));
		assert(IntRing_full(id));
		/* pop_back */
		for (i = MAX-3; i >= 0; i--) {
			size = IntRing_size(id);
			assert(IntRing_pop_back(id) == piyo[i]);
			assert(IntRing_size(id) == size - 1);
		}
		assert(IntRing_pop_back(id) == piyo[MAX-1]);
		assert(IntRing_pop_back(id) == piyo[MAX-2]);

		assert(IntRing_size(id) == 0);
		assert(IntRing_empty(id));
		assert(!IntRing_full(id));
	}

	IntRing_delete(id);
}

void RingTest_test_2_4(void)
{
	int i, j;
	printf("***** test_2_4 *****\n");
	id = IntRing_new(MAX);
	for (j = 0; j < MAX+1; j++) {
		id->begin = id->end = j;
		assert(IntRing_size(id) == 0);
		/* insert */
		/* 空状態 */
		for (i = 0; i < MAX/2; i++) {
			assert(id_cmp(id, 0, piyo, i) == 0);
			assert(IntRing_insert(id, i, piyo[i]));
		}
		/* 先頭・前寄り */
		assert(IntRing_insert(id, 0, piyo[0]));
		assert(IntRing_insert(id, 1, piyo[1]));
		assert(IntRing_insert(id, 2, piyo[2]));
		assert(id_cmp(id, 0, piyo, 3) == 0);
		assert(id_cmp(id, 3, piyo, MAX/2) == 0);
		assert(IntRing_size(id) == MAX/2 + 3);

		/* erase */
		/* 先頭・前寄り */
		IntRing_erase(id, 2, 1);
		assert(id_cmp(id, 0, piyo, 2) == 0);
		assert(id_cmp(id, 2, piyo, MAX/2) == 0);
		assert(IntRing_size(id) == MAX/2 + 2);
		IntRing_erase(id, 1, 1);
		assert(id_cmp(id, 0, piyo, 1) == 0);
		assert(id_cmp(id, 1, piyo, MAX/2) == 0);
		assert(IntRing_size(id) == MAX/2 + 1);
		IntRing_erase(id, 0, 1);
		assert(id_cmp(id, 0, piyo, MAX/2) == 0);
		assert(IntRing_size(id) == MAX/2);

		/* insert */
		/* 末尾・後ろ寄り */
		assert(IntRing_insert(id, MAX/2, piyo[0]));
		assert(IntRing_insert(id, MAX/2, piyo[1]));
		assert(IntRing_insert(id, MAX/2, piyo[2]));
		assert(id_cmp(id, 0, piyo, MAX/2) == 0);
		assert(*IntRing_at(id, MAX/2 + 0) == piyo[2]);
		assert(*IntRing_at(id, MAX/2 + 1) == piyo[1]);
		assert(*IntRing_at(id, MAX/2 + 2) == piyo[0]);

		/* erase */
		/* 末尾・後ろ寄り */
		IntRing_erase(id, MAX/2, 1);
		assert(id_cmp(id, 0, piyo, MAX/2) == 0);
		assert(*IntRing_at(id, MAX/2 + 0) == piyo[1]);
		assert(*IntRing_at(id, MAX/2 + 1) == piyo[0]);
		assert(IntRing_size(id) == MAX/2 + 2);
		IntRing_erase(id, MAX/2, 1);
		assert(id_cmp(id, 0, piyo, MAX/2) == 0);
		assert(*IntRing_at(id, MAX/2 + 0) == piyo[0]);
		assert(IntRing_size(id) == MAX/2 + 1);
		IntRing_erase(id, MAX/2, 1);
		assert(id_cmp(id, 0, piyo, MAX/2) == 0);
		assert(IntRing_size(id) == MAX/2);

		/* 先頭から全削除 */
		for (i = 0; i < MAX/2; i++) {
			assert(id_cmp(id, 0, &piyo[i], MAX/2 - i) == 0);
			IntRing_erase(id, 0, 1);
		}
		assert(IntRing_size(id) == 0);

		for (i = 0; i < MAX/2; i++) {
			assert(id_cmp(id, 0, piyo, i) == 0);
			assert(IntRing_insert(id, i, piyo[i]));
		}
		assert(IntRing_size(id) == MAX/2);

		/* 末尾から全削除 */
		for (i = 0; i < MAX/2; i++) {
			assert(id_cmp(id, 0, piyo, MAX/2 - i) == 0);
			IntRing_erase(id, IntRing_size(id) -1, 1);
		}
		assert(IntRing_size(id) == 0);

	}

	IntRing_delete(id);
}

void RingTest_test_2_5(void)
{
	int i;
	printf("***** test_2_5 *****\n");
	id = IntRing_new(MAX);
	for (i = 0; i < MAX+1; i++) {
		id->begin = id->end = i;
		assert(IntRing_size(id) == 0);
		/* insert_array */
		/* 空状態 */
		assert(IntRing_insert_array(id, 0, piyo, MAX/2));
		assert(id_cmp(id, 0, piyo, MAX/2) == 0);
		assert(IntRing_size(id) == MAX/2);

		/* insert_array */
		/* 先頭 */
		assert(IntRing_insert_array(id, 0, piyo, 3));
		assert(id_cmp(id, 0, piyo, 3) == 0);
		assert(id_cmp(id, 3, piyo, MAX/2) == 0);
		assert(IntRing_size(id) == MAX/2 + 3);
		/* erase */
		IntRing_erase(id, 0, 3);
		assert(id_cmp(id, 0, piyo, MAX/2) == 0);
		assert(IntRing_size(id) == MAX/2);

		/* insert_array */
		/* 前寄り */
		assert(IntRing_insert_array(id, 2, piyo, 3));
		assert(id_cmp(id, 0, piyo, 2) == 0);
		assert(id_cmp(id, 2, piyo, 3) == 0);
		assert(id_cmp(id, 5, &piyo[2], MAX/2 -2) == 0);
		assert(IntRing_size(id) == MAX/2 + 3);
		/* 0個指定 */
		assert(IntRing_insert_array(id, 1, &piyo[MAX/2], 0));
		assert(id_cmp(id, 0, piyo, 2) == 0);
		assert(id_cmp(id, 2, piyo, 3) == 0);
		assert(id_cmp(id, 5, &piyo[2], MAX/2 -2) == 0);
		assert(IntRing_size(id) == MAX/2 + 3);
		/* 1個指定 */
		assert(IntRing_insert_array(id, 1, &piyo[MAX-1], 1));
		assert(id_cmp(id, 0, &piyo[0], 1) == 0);
		assert(id_cmp(id, 1, &piyo[MAX-1], 1) == 0);
		assert(id_cmp(id, 2, &piyo[1], 1) == 0);
		assert(id_cmp(id, 3, &piyo[0], 3) == 0);
		assert(id_cmp(id, 6, &piyo[2], MAX/2 -2) == 0);
		assert(IntRing_size(id) == MAX/2 + 4);
		/* erase */
		/* 0個指定 */
		IntRing_erase(id, 1, 0);
		assert(id_cmp(id, 0, &piyo[0], 1) == 0);
		assert(id_cmp(id, 1, &piyo[MAX-1], 1) == 0);
		assert(id_cmp(id, 2, &piyo[1], 1) == 0);
		assert(id_cmp(id, 3, &piyo[0], 3) == 0);
		assert(id_cmp(id, 6, &piyo[2], MAX/2 -2) == 0);
		assert(IntRing_size(id) == MAX/2 + 4);
		/* 1個指定 */
		IntRing_erase(id, 1, 1);
		assert(id_cmp(id, 0, piyo, 2) == 0);
		assert(id_cmp(id, 2, piyo, 3) == 0);
		assert(id_cmp(id, 5, &piyo[2], MAX/2 -2) == 0);
		assert(IntRing_size(id) == MAX/2 + 3);
		/* 前寄り */
		IntRing_erase(id, 2, 3);
		assert(id_cmp(id, 0, piyo, MAX/2) == 0);
		assert(IntRing_size(id) == MAX/2);

		/* insert_array */
		/* 末尾 */
		assert(IntRing_insert_array(id, IntRing_size(id), piyo, 3));
		assert(id_cmp(id, 0, piyo, MAX/2) == 0);
		assert(id_cmp(id, MAX/2, piyo, 3) == 0);
		assert(IntRing_size(id) == MAX/2 + 3);
		/* erase */
		IntRing_erase(id, IntRing_size(id) - 3, 3);
		assert(id_cmp(id, 0, piyo, MAX/2) == 0);
		assert(IntRing_size(id) == MAX/2);

		/* insert_array */
		/* 後ろ寄り */
		assert(IntRing_insert_array(id, IntRing_size(id) - 2, piyo, 3));
		assert(id_cmp(id, 0, piyo, MAX/2 -2) == 0);
		assert(id_cmp(id, MAX/2 -2, piyo, 3) == 0);
		assert(id_cmp(id, MAX/2 +1, &piyo[MAX/2 -2], 2) == 0);
		assert(IntRing_size(id) == MAX/2 + 3);
		/* 0個指定 */
		assert(IntRing_insert_array(id, IntRing_size(id) - 1, &piyo[MAX-1], 0));
		assert(id_cmp(id, 0, piyo, MAX/2 -2) == 0);
		assert(id_cmp(id, MAX/2 -2, piyo, 3) == 0);
		assert(id_cmp(id, MAX/2 +1, &piyo[MAX/2 -2], 2) == 0);
		assert(IntRing_size(id) == MAX/2 + 3);
		/* 1個指定 */
		assert(IntRing_insert_array(id, IntRing_size(id) - 1, &piyo[MAX-1], 1));
		assert(id_cmp(id, 0, piyo, MAX/2 -2) == 0);
		assert(id_cmp(id, MAX/2 -2, piyo, 3) == 0);
		assert(id_cmp(id, MAX/2 +1, &piyo[MAX/2 -2], 1) == 0);
		assert(id_cmp(id, MAX/2 +2, &piyo[MAX-1], 1) == 0);
		assert(id_cmp(id, MAX/2 +3, &piyo[MAX/2 -1], 1) == 0);
		assert(IntRing_size(id) == MAX/2 + 4);
		/* erase */
		/* 0個指定 */
		IntRing_erase(id, IntRing_size(id) - 2, 0);
		assert(id_cmp(id, 0, piyo, MAX/2 -2) == 0);
		assert(id_cmp(id, MAX/2 -2, piyo, 3) == 0);
		assert(id_cmp(id, MAX/2 +1, &piyo[MAX/2 -2], 1) == 0);
		assert(id_cmp(id, MAX/2 +2, &piyo[MAX-1], 1) == 0);
		assert(id_cmp(id, MAX/2 +3, &piyo[MAX/2 -1], 1) == 0);
		assert(IntRing_size(id) == MAX/2 + 4);
		/* 1個指定 */
		IntRing_erase(id, IntRing_size(id) - 2, 1);
		assert(id_cmp(id, 0, piyo, MAX/2 -2) == 0);
		assert(id_cmp(id, MAX/2 -2, piyo, 3) == 0);
		assert(id_cmp(id, MAX/2 +1, &piyo[MAX/2 -2], 2) == 0);
		assert(IntRing_size(id) == MAX/2 + 3);
		/* 後ろ寄り */
		IntRing_erase(id, IntRing_size(id) - 5, 3);
		assert(id_cmp(id, 0, piyo, MAX/2) == 0);
		assert(IntRing_size(id) == MAX/2);

		/* MAX以上で失敗 */
		assert(!IntRing_insert_array(id, 0, piyo, MAX - MAX/2 +1));
		assert(id_cmp(id, 0, piyo, MAX/2) == 0);
		assert(IntRing_size(id) == MAX/2);
		/* MAXになるまで */
		assert(IntRing_insert_array(id, 0, piyo, MAX - MAX/2));
		assert(IntRing_size(id) == MAX);
		assert(IntRing_full(id));
		/* 先頭から全削除 */
		IntRing_erase(id, 0, IntRing_size(id));
		assert(IntRing_size(id) == 0);
	}

	IntRing_delete(id);
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

