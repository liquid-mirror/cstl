#include "test.h"



DEQUE_IMPLEMENT(UCharDeque, unsigned char)
DEQUE_IMPLEMENT(IntDeque, int)

static UCharDeque *ud;
static IntDeque *id;

#define MAX		255

static unsigned char hoge[MAX];
static int piyo[MAX];

void deque_init_hoge(void)
{
	int i;
	for (i = 0; i < MAX; i++) {
		hoge[i] = i;
	}
}

void deque_init_piyo(void)
{
	int i;
	for (i = 0; i < MAX; i++) {
		piyo[i] = i;
	}
}

int ud_cmp(size_t idx, unsigned char *buf, size_t n)
{
	int i, j;
	for (i = idx, j = 0; i < idx + n; i++, j++) {
/*        printf("%d, %d\n", *UCharDeque_at(ud, i), buf[j]);*/
		if (*UCharDeque_at(ud, i) != buf[j]) return 1;
	}
	return 0;
}

int id_cmp(size_t idx, int *buf, size_t n)
{
	int i, j;
	for (i = idx, j = 0; i < idx + n; i++, j++) {
/*        printf("%d, %d\n", *IntDeque_at(id, i), buf[j]);*/
		if (*IntDeque_at(id, i) != buf[j]) return 1;
	}
	return 0;
}


void DequeTest_test_1_1(void)
{
	int i, j;
	printf("***** test_1_1 *****\n");
	ud = UCharDeque_new(MAX);
	/* 初期状態 */
	assert(UCharDeque_max_size(ud) == MAX);
	assert(UCharDeque_size(ud) == 0);
	assert(UCharDeque_empty(ud));
	assert(!UCharDeque_full(ud));
	for (j = 0; j < MAX+1; j++) {
		ud->begin = ud->end = j;
		assert(UCharDeque_size(ud) == 0);
		/* assign_array */
		assert(UCharDeque_assign_array(ud, hoge, MAX));
		assert(ud_cmp(0, hoge, MAX) == 0);
		assert(UCharDeque_size(ud) == MAX);
		assert(!UCharDeque_empty(ud));
		assert(UCharDeque_full(ud));
		/* at */
		for (i = 0; i < UCharDeque_size(ud); i++) {
			assert(*UCharDeque_at(ud, i) == hoge[i]);
			*UCharDeque_at(ud, i) = hoge[MAX - i -1];
			assert(*UCharDeque_at(ud, i) == hoge[MAX - i -1]);
		}
		/* clear */
		UCharDeque_clear(ud);
		assert(UCharDeque_size(ud) == 0);
		assert(UCharDeque_empty(ud));
		assert(!UCharDeque_full(ud));
		/* assign_array */
		assert(UCharDeque_assign_array(ud, hoge, MAX/2));
		assert(ud_cmp(0, hoge, MAX/2) == 0);
		assert(UCharDeque_size(ud) == MAX/2);
		assert(!UCharDeque_empty(ud));
		assert(!UCharDeque_full(ud));
		/* MAX以上で失敗 */
		assert(!UCharDeque_assign_array(ud, hoge, MAX+1));
		assert(ud_cmp(0, hoge, MAX/2) == 0);
		assert(UCharDeque_size(ud) == MAX/2);
		assert(!UCharDeque_empty(ud));
		assert(!UCharDeque_full(ud));
		/* clear */
		UCharDeque_clear(ud);
		assert(UCharDeque_size(ud) == 0);
	}

	UCharDeque_delete(ud);
}

void DequeTest_test_1_2(void)
{
	int i, j;
	size_t size;
	printf("***** test_1_2 *****\n");
	ud = UCharDeque_new(MAX);
	for (j = 0; j < MAX+1; j++) {
		ud->begin = ud->end = j;
		assert(UCharDeque_size(ud) == 0);
		/* push_back */
		/* MAXまで */
		for (i = 0; i < MAX; i++) {
			size = UCharDeque_size(ud);
			assert(UCharDeque_push_back(ud, hoge[i]));
			assert(UCharDeque_size(ud) == size + 1);
		}
		assert(ud_cmp(0, hoge, MAX) == 0);
		assert(UCharDeque_size(ud) == MAX);
		assert(!UCharDeque_empty(ud));
		assert(UCharDeque_full(ud));
		/* FULLで失敗 */
		assert(!UCharDeque_push_back(ud, hoge[0]));
		assert(UCharDeque_size(ud) == MAX);
		assert(!UCharDeque_empty(ud));
		assert(UCharDeque_full(ud));
		/* front */
		assert(UCharDeque_front(ud) == hoge[0]);
		/* back */
		assert(UCharDeque_back(ud) == hoge[MAX-1]);
		/* pop_front */
		size = UCharDeque_size(ud);
		assert(UCharDeque_pop_front(ud) == hoge[0]);
		assert(UCharDeque_size(ud) == size - 1);
		/* push_back */
		size = UCharDeque_size(ud);
		assert(UCharDeque_push_back(ud, hoge[0]));
		assert(UCharDeque_size(ud) == size + 1);
		assert(ud_cmp(0, &hoge[1], MAX-1) == 0);
		assert(ud_cmp(MAX-1, hoge, 1) == 0);
		assert(UCharDeque_size(ud) == MAX);
		assert(!UCharDeque_empty(ud));
		assert(UCharDeque_full(ud));
		/* pop_front */
		for (i = 0; i < MAX; i++) {
			size = UCharDeque_size(ud);
			if (i < MAX-1) {
				assert(UCharDeque_pop_front(ud) == hoge[i+1]);
			} else {
				assert(UCharDeque_pop_front(ud) == hoge[0]);
			}
			assert(UCharDeque_size(ud) == size - 1);
		}
		assert(UCharDeque_size(ud) == 0);
		assert(UCharDeque_empty(ud));
		assert(!UCharDeque_full(ud));
	}

	UCharDeque_delete(ud);
}

void DequeTest_test_1_3(void)
{
	int i, j;
	size_t size;
	printf("***** test_1_3 *****\n");
	ud = UCharDeque_new(MAX);
	for (j = 0; j < MAX+1; j++) {
		ud->begin = ud->end = j;
		assert(UCharDeque_size(ud) == 0);
		/* push_front */
		/* MAXまで */
		for (i = 0; i < MAX; i++) {
			size = UCharDeque_size(ud);
			assert(UCharDeque_push_front(ud, hoge[MAX-i-1]));
			assert(UCharDeque_size(ud) == size + 1);
		}
		assert(ud_cmp(0, hoge, MAX) == 0);
		assert(UCharDeque_size(ud) == MAX);
		assert(!UCharDeque_empty(ud));
		assert(UCharDeque_full(ud));
		/* FULLで失敗 */
		assert(!UCharDeque_push_front(ud, hoge[0]));
		assert(UCharDeque_size(ud) == MAX);
		assert(!UCharDeque_empty(ud));
		assert(UCharDeque_full(ud));
		/* front */
		assert(UCharDeque_front(ud) == hoge[0]);
		/* back */
		assert(UCharDeque_back(ud) == hoge[MAX-1]);
		/* pop_back */
		size = UCharDeque_size(ud);
		assert(UCharDeque_pop_back(ud) == hoge[MAX-1]);
		assert(UCharDeque_pop_back(ud) == hoge[MAX-2]);
		assert(UCharDeque_size(ud) == size - 2);
		/* push_front */
		size = UCharDeque_size(ud);
		assert(UCharDeque_push_front(ud, hoge[MAX-1]));
		assert(UCharDeque_push_front(ud, hoge[MAX-2]));
		assert(UCharDeque_size(ud) == size + 2);
		assert(ud_cmp(0, &hoge[MAX-2], 2) == 0);
		assert(ud_cmp(2, hoge, MAX-2) == 0);
		assert(UCharDeque_size(ud) == MAX);
		assert(!UCharDeque_empty(ud));
		assert(UCharDeque_full(ud));
		/* pop_back */
		for (i = MAX-3; i >= 0; i--) {
			size = UCharDeque_size(ud);
			assert(UCharDeque_pop_back(ud) == hoge[i]);
			assert(UCharDeque_size(ud) == size - 1);
		}
		assert(UCharDeque_pop_back(ud) == hoge[MAX-1]);
		assert(UCharDeque_pop_back(ud) == hoge[MAX-2]);

		assert(UCharDeque_size(ud) == 0);
		assert(UCharDeque_empty(ud));
		assert(!UCharDeque_full(ud));
	}

	UCharDeque_delete(ud);
}

void DequeTest_test_1_4(void)
{
	int i, j;
	printf("***** test_1_4 *****\n");
	ud = UCharDeque_new(MAX);
	for (j = 0; j < MAX+1; j++) {
		ud->begin = ud->end = j;
		assert(UCharDeque_size(ud) == 0);
		/* insert */
		/* 空状態 */
		for (i = 0; i < MAX/2; i++) {
			assert(ud_cmp(0, hoge, i) == 0);
			assert(UCharDeque_insert(ud, i, hoge[i]));
		}
		/* 先頭・前寄り */
		assert(UCharDeque_insert(ud, 0, hoge[0]));
		assert(UCharDeque_insert(ud, 1, hoge[1]));
		assert(UCharDeque_insert(ud, 2, hoge[2]));
		assert(ud_cmp(0, hoge, 3) == 0);
		assert(ud_cmp(3, hoge, MAX/2) == 0);
		assert(UCharDeque_size(ud) == MAX/2 + 3);

		/* erase */
		/* 先頭・前寄り */
		UCharDeque_erase(ud, 2, 1);
		assert(ud_cmp(0, hoge, 2) == 0);
		assert(ud_cmp(2, hoge, MAX/2) == 0);
		assert(UCharDeque_size(ud) == MAX/2 + 2);
		UCharDeque_erase(ud, 1, 1);
		assert(ud_cmp(0, hoge, 1) == 0);
		assert(ud_cmp(1, hoge, MAX/2) == 0);
		assert(UCharDeque_size(ud) == MAX/2 + 1);
		UCharDeque_erase(ud, 0, 1);
		assert(ud_cmp(0, hoge, MAX/2) == 0);
		assert(UCharDeque_size(ud) == MAX/2);

		/* insert */
		/* 末尾・後ろ寄り */
		assert(UCharDeque_insert(ud, MAX/2, hoge[0]));
		assert(UCharDeque_insert(ud, MAX/2, hoge[1]));
		assert(UCharDeque_insert(ud, MAX/2, hoge[2]));
		assert(ud_cmp(0, hoge, MAX/2) == 0);
		assert(*UCharDeque_at(ud, MAX/2 + 0) == hoge[2]);
		assert(*UCharDeque_at(ud, MAX/2 + 1) == hoge[1]);
		assert(*UCharDeque_at(ud, MAX/2 + 2) == hoge[0]);

		/* erase */
		/* 末尾・後ろ寄り */
		UCharDeque_erase(ud, MAX/2, 1);
		assert(ud_cmp(0, hoge, MAX/2) == 0);
		assert(*UCharDeque_at(ud, MAX/2 + 0) == hoge[1]);
		assert(*UCharDeque_at(ud, MAX/2 + 1) == hoge[0]);
		assert(UCharDeque_size(ud) == MAX/2 + 2);
		UCharDeque_erase(ud, MAX/2, 1);
		assert(ud_cmp(0, hoge, MAX/2) == 0);
		assert(*UCharDeque_at(ud, MAX/2 + 0) == hoge[0]);
		assert(UCharDeque_size(ud) == MAX/2 + 1);
		UCharDeque_erase(ud, MAX/2, 1);
		assert(ud_cmp(0, hoge, MAX/2) == 0);
		assert(UCharDeque_size(ud) == MAX/2);

		/* 先頭から全削除 */
		for (i = 0; i < MAX/2; i++) {
			assert(ud_cmp(0, &hoge[i], MAX/2 - i) == 0);
			UCharDeque_erase(ud, 0, 1);
		}
		assert(UCharDeque_size(ud) == 0);

		for (i = 0; i < MAX/2; i++) {
			assert(ud_cmp(0, hoge, i) == 0);
			assert(UCharDeque_insert(ud, i, hoge[i]));
		}
		assert(UCharDeque_size(ud) == MAX/2);

		/* 末尾から全削除 */
		for (i = 0; i < MAX/2; i++) {
			assert(ud_cmp(0, hoge, MAX/2 - i) == 0);
			UCharDeque_erase(ud, UCharDeque_size(ud) -1, 1);
		}
		assert(UCharDeque_size(ud) == 0);

		/* MAX以上で失敗 */
		assert(UCharDeque_assign_array(ud, hoge, MAX));
		assert(ud_cmp(0, hoge, MAX) == 0);
		assert(UCharDeque_size(ud) == MAX);
		assert(UCharDeque_full(ud));
		assert(!UCharDeque_insert(ud, 0, hoge[0]));
		assert(ud_cmp(0, hoge, MAX) == 0);
		assert(UCharDeque_size(ud) == MAX);
		assert(UCharDeque_full(ud));
	}

	UCharDeque_delete(ud);
}

void DequeTest_test_1_5(void)
{
	int i;
	printf("***** test_1_5 *****\n");
	ud = UCharDeque_new(MAX);
	for (i = 0; i < MAX+1; i++) {
		ud->begin = ud->end = i;
		assert(UCharDeque_size(ud) == 0);
		/* insert_n */
		/* 空状態 */
		assert(UCharDeque_insert_n(ud, 0, hoge, MAX/2));
		assert(ud_cmp(0, hoge, MAX/2) == 0);
		assert(UCharDeque_size(ud) == MAX/2);

		/* insert_n */
		/* 先頭 */
		assert(UCharDeque_insert_n(ud, 0, hoge, 3));
		assert(ud_cmp(0, hoge, 3) == 0);
		assert(ud_cmp(3, hoge, MAX/2) == 0);
		assert(UCharDeque_size(ud) == MAX/2 + 3);
		/* erase */
		UCharDeque_erase(ud, 0, 3);
		assert(ud_cmp(0, hoge, MAX/2) == 0);
		assert(UCharDeque_size(ud) == MAX/2);

		/* insert_n */
		/* 前寄り */
		assert(UCharDeque_insert_n(ud, 2, hoge, 3));
		assert(ud_cmp(0, hoge, 2) == 0);
		assert(ud_cmp(2, hoge, 3) == 0);
		assert(ud_cmp(5, &hoge[2], MAX/2 -2) == 0);
		assert(UCharDeque_size(ud) == MAX/2 + 3);
		/* 0個指定 */
		assert(UCharDeque_insert_n(ud, 1, &hoge[MAX/2], 0));
		assert(ud_cmp(0, hoge, 2) == 0);
		assert(ud_cmp(2, hoge, 3) == 0);
		assert(ud_cmp(5, &hoge[2], MAX/2 -2) == 0);
		assert(UCharDeque_size(ud) == MAX/2 + 3);
		/* 1個指定 */
		assert(UCharDeque_insert_n(ud, 1, &hoge[MAX-1], 1));
		assert(ud_cmp(0, &hoge[0], 1) == 0);
		assert(ud_cmp(1, &hoge[MAX-1], 1) == 0);
		assert(ud_cmp(2, &hoge[1], 1) == 0);
		assert(ud_cmp(3, &hoge[0], 3) == 0);
		assert(ud_cmp(6, &hoge[2], MAX/2 -2) == 0);
		assert(UCharDeque_size(ud) == MAX/2 + 4);
		/* erase */
		/* 0個指定 */
		UCharDeque_erase(ud, 1, 0);
		assert(ud_cmp(0, &hoge[0], 1) == 0);
		assert(ud_cmp(1, &hoge[MAX-1], 1) == 0);
		assert(ud_cmp(2, &hoge[1], 1) == 0);
		assert(ud_cmp(3, &hoge[0], 3) == 0);
		assert(ud_cmp(6, &hoge[2], MAX/2 -2) == 0);
		assert(UCharDeque_size(ud) == MAX/2 + 4);
		/* 1個指定 */
		UCharDeque_erase(ud, 1, 1);
		assert(ud_cmp(0, hoge, 2) == 0);
		assert(ud_cmp(2, hoge, 3) == 0);
		assert(ud_cmp(5, &hoge[2], MAX/2 -2) == 0);
		assert(UCharDeque_size(ud) == MAX/2 + 3);
		/* 前寄り */
		UCharDeque_erase(ud, 2, 3);
		assert(ud_cmp(0, hoge, MAX/2) == 0);
		assert(UCharDeque_size(ud) == MAX/2);

		/* insert_n */
		/* 末尾 */
		assert(UCharDeque_insert_n(ud, UCharDeque_size(ud), hoge, 3));
		assert(ud_cmp(0, hoge, MAX/2) == 0);
		assert(ud_cmp(MAX/2, hoge, 3) == 0);
		assert(UCharDeque_size(ud) == MAX/2 + 3);
		/* erase */
		UCharDeque_erase(ud, UCharDeque_size(ud) - 3, 3);
		assert(ud_cmp(0, hoge, MAX/2) == 0);
		assert(UCharDeque_size(ud) == MAX/2);

		/* insert_n */
		/* 後ろ寄り */
		assert(UCharDeque_insert_n(ud, UCharDeque_size(ud) - 2, hoge, 3));
		assert(ud_cmp(0, hoge, MAX/2 -2) == 0);
		assert(ud_cmp(MAX/2 -2, hoge, 3) == 0);
		assert(ud_cmp(MAX/2 +1, &hoge[MAX/2 -2], 2) == 0);
		assert(UCharDeque_size(ud) == MAX/2 + 3);
		/* 0個指定 */
		assert(UCharDeque_insert_n(ud, UCharDeque_size(ud) - 1, &hoge[MAX-1], 0));
		assert(ud_cmp(0, hoge, MAX/2 -2) == 0);
		assert(ud_cmp(MAX/2 -2, hoge, 3) == 0);
		assert(ud_cmp(MAX/2 +1, &hoge[MAX/2 -2], 2) == 0);
		assert(UCharDeque_size(ud) == MAX/2 + 3);
		/* 1個指定 */
		assert(UCharDeque_insert_n(ud, UCharDeque_size(ud) - 1, &hoge[MAX-1], 1));
		assert(ud_cmp(0, hoge, MAX/2 -2) == 0);
		assert(ud_cmp(MAX/2 -2, hoge, 3) == 0);
		assert(ud_cmp(MAX/2 +1, &hoge[MAX/2 -2], 1) == 0);
		assert(ud_cmp(MAX/2 +2, &hoge[MAX-1], 1) == 0);
		assert(ud_cmp(MAX/2 +3, &hoge[MAX/2 -1], 1) == 0);
		assert(UCharDeque_size(ud) == MAX/2 + 4);
		/* erase */
		/* 0個指定 */
		UCharDeque_erase(ud, UCharDeque_size(ud) - 2, 0);
		assert(ud_cmp(0, hoge, MAX/2 -2) == 0);
		assert(ud_cmp(MAX/2 -2, hoge, 3) == 0);
		assert(ud_cmp(MAX/2 +1, &hoge[MAX/2 -2], 1) == 0);
		assert(ud_cmp(MAX/2 +2, &hoge[MAX-1], 1) == 0);
		assert(ud_cmp(MAX/2 +3, &hoge[MAX/2 -1], 1) == 0);
		assert(UCharDeque_size(ud) == MAX/2 + 4);
		/* 1個指定 */
		UCharDeque_erase(ud, UCharDeque_size(ud) - 2, 1);
		assert(ud_cmp(0, hoge, MAX/2 -2) == 0);
		assert(ud_cmp(MAX/2 -2, hoge, 3) == 0);
		assert(ud_cmp(MAX/2 +1, &hoge[MAX/2 -2], 2) == 0);
		assert(UCharDeque_size(ud) == MAX/2 + 3);
		/* 後ろ寄り */
		UCharDeque_erase(ud, UCharDeque_size(ud) - 5, 3);
		assert(ud_cmp(0, hoge, MAX/2) == 0);
		assert(UCharDeque_size(ud) == MAX/2);

		/* MAX以上で失敗 */
		assert(!UCharDeque_insert_n(ud, 0, hoge, MAX - MAX/2 +1));
		assert(ud_cmp(0, hoge, MAX/2) == 0);
		assert(UCharDeque_size(ud) == MAX/2);
		/* MAXになるまで */
		assert(UCharDeque_insert_n(ud, 0, hoge, MAX - MAX/2));
		assert(UCharDeque_size(ud) == MAX);
		assert(UCharDeque_full(ud));
		/* 先頭から全削除 */
		UCharDeque_erase(ud, 0, UCharDeque_size(ud));
		assert(UCharDeque_size(ud) == 0);
	}

	UCharDeque_delete(ud);
}

void DequeTest_test_1_6(void)
{
	int i;
	UCharDeque *x;
	unsigned char b[] = {1, 2, 4, 43, 2, 54, 1, 0, 2, 24};
	printf("***** test_1_6 *****\n");
	ud = UCharDeque_new(MAX);
	for (i = 0; i < MAX; i++) {
		assert(UCharDeque_push_back(ud, hoge[i]));
	}
	/* assign */
	assert(UCharDeque_size(ud) == MAX);
	x = UCharDeque_new(MAX);
	assert(UCharDeque_assign(x, ud, 0, UCharDeque_size(ud)));
	assert(UCharDeque_size(ud) == UCharDeque_size(x));
	assert(UCharDeque_max_size(ud) == UCharDeque_max_size(x));
	for (i = 0; i < UCharDeque_size(ud); i++) {
		assert(*UCharDeque_at(x, i) == *UCharDeque_at(ud, i));
	}
	/* resize */
	UCharDeque_clear(ud);
	assert(UCharDeque_resize(ud, 64, 100));
	assert(UCharDeque_size(ud) == 64);
	for (i = 0; i < UCharDeque_size(ud); i++) {
		assert(*UCharDeque_at(ud, i) == 100);
	}
	assert(UCharDeque_resize(ud, 16, 99));
	assert(UCharDeque_size(ud) == 16);
	for (i = 0; i < UCharDeque_size(ud); i++) {
		assert(*UCharDeque_at(ud, i) == 100);
	}
	assert(UCharDeque_resize(ud, 32, 99));
	for (i = 0; i < UCharDeque_size(ud); i++) {
		if (i < 16) {
			assert(*UCharDeque_at(ud, i) == 100);
		} else {
			assert(*UCharDeque_at(ud, i) == 99);
		}
	}
	assert(UCharDeque_size(ud) == 32);
	assert(UCharDeque_resize(ud, MAX+1, 99) == 0);
	assert(UCharDeque_resize(ud, 0, 100));
	assert(UCharDeque_size(ud) == 0);
	/* swap */
	UCharDeque_clear(ud);
	assert(UCharDeque_assign_array(ud, hoge, MAX));
	assert(memcmp(UCharDeque_at(ud, 0), hoge, MAX) == 0);
	assert(UCharDeque_size(ud) == MAX);
	UCharDeque_clear(x);
	assert(UCharDeque_assign_array(x, b, sizeof b));
	assert(memcmp(UCharDeque_at(x, 0), b, sizeof b) == 0);
	assert(UCharDeque_size(x) == sizeof b);

	UCharDeque_swap(ud, x);

	assert(memcmp(UCharDeque_at(ud, 0), b, sizeof b) == 0);
	assert(UCharDeque_size(ud) == sizeof b);
	assert(memcmp(UCharDeque_at(x, 0), hoge, MAX) == 0);
	assert(UCharDeque_size(x) == MAX);


	UCharDeque_delete(ud);
	UCharDeque_delete(x);
}



void DequeTest_test_2_1(void)
{
	int i, j;
	printf("***** test_2_1 *****\n");
	id = IntDeque_new(MAX);
	/* 初期状態 */
	assert(IntDeque_max_size(id) == MAX);
	assert(IntDeque_size(id) == 0);
	assert(IntDeque_empty(id));
	assert(!IntDeque_full(id));
	for (j = 0; j < MAX+1; j++) {
		id->begin = id->end = j;
		assert(IntDeque_size(id) == 0);
		/* assign_array */
		assert(IntDeque_assign_array(id, piyo, MAX));
		assert(id_cmp(0, piyo, MAX) == 0);
		assert(IntDeque_size(id) == MAX);
		assert(!IntDeque_empty(id));
		assert(IntDeque_full(id));
		/* at */
		for (i = 0; i < IntDeque_size(id); i++) {
			assert(*IntDeque_at(id, i) == hoge[i]);
			*IntDeque_at(id, i) = hoge[MAX - i -1];
			assert(*IntDeque_at(id, i) == hoge[MAX - i -1]);
		}
		/* clear */
		IntDeque_clear(id);
		assert(IntDeque_size(id) == 0);
		assert(IntDeque_empty(id));
		assert(!IntDeque_full(id));
		/* assign_array */
		assert(IntDeque_assign_array(id, piyo, MAX/2));
		assert(id_cmp(0, piyo, MAX/2) == 0);
		assert(IntDeque_size(id) == MAX/2);
		assert(!IntDeque_empty(id));
		assert(!IntDeque_full(id));
		/* MAX以上で失敗 */
		assert(!IntDeque_assign_array(id, piyo, MAX+1));
		assert(id_cmp(0, piyo, MAX/2) == 0);
		assert(IntDeque_size(id) == MAX/2);
		assert(!IntDeque_empty(id));
		assert(!IntDeque_full(id));
		/* clear */
		IntDeque_clear(id);
		assert(IntDeque_size(id) == 0);
	}

	IntDeque_delete(id);
}

void DequeTest_test_2_2(void)
{
	int i, j;
	size_t size;
	printf("***** test_2_2 *****\n");
	id = IntDeque_new(MAX);
	for (j = 0; j < MAX+1; j++) {
		id->begin = id->end = j;
		assert(IntDeque_size(id) == 0);
		/* push_back */
		/* MAXまで */
		for (i = 0; i < MAX; i++) {
			size = IntDeque_size(id);
			assert(IntDeque_push_back(id, piyo[i]));
			assert(IntDeque_size(id) == size + 1);
		}
		assert(id_cmp(0, piyo, MAX) == 0);
		assert(IntDeque_size(id) == MAX);
		assert(!IntDeque_empty(id));
		assert(IntDeque_full(id));
		/* FULLで失敗 */
		assert(!IntDeque_push_back(id, piyo[0]));
		assert(IntDeque_size(id) == MAX);
		assert(!IntDeque_empty(id));
		assert(IntDeque_full(id));
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
		assert(id_cmp(0, &piyo[1], MAX-1) == 0);
		assert(id_cmp(MAX-1, piyo, 1) == 0);
		assert(IntDeque_size(id) == MAX);
		assert(!IntDeque_empty(id));
		assert(IntDeque_full(id));
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
		assert(!IntDeque_full(id));
	}

	IntDeque_delete(id);
}

void DequeTest_test_2_3(void)
{
	int i, j;
	size_t size;
	printf("***** test_2_3 *****\n");
	id = IntDeque_new(MAX);
	for (j = 0; j < MAX+1; j++) {
		id->begin = id->end = j;
		assert(IntDeque_size(id) == 0);
		/* push_front */
		/* MAXまで */
		for (i = 0; i < MAX; i++) {
			size = IntDeque_size(id);
			assert(IntDeque_push_front(id, piyo[MAX-i-1]));
			assert(IntDeque_size(id) == size + 1);
		}
		assert(id_cmp(0, piyo, MAX) == 0);
		assert(IntDeque_size(id) == MAX);
		assert(!IntDeque_empty(id));
		assert(IntDeque_full(id));
		/* FULLで失敗 */
		assert(!IntDeque_push_front(id, piyo[0]));
		assert(IntDeque_size(id) == MAX);
		assert(!IntDeque_empty(id));
		assert(IntDeque_full(id));
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
		assert(id_cmp(0, &piyo[MAX-2], 2) == 0);
		assert(id_cmp(2, piyo, MAX-2) == 0);
		assert(IntDeque_size(id) == MAX);
		assert(!IntDeque_empty(id));
		assert(IntDeque_full(id));
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
		assert(!IntDeque_full(id));
	}

	IntDeque_delete(id);
}

void DequeTest_test_2_4(void)
{
	int i, j;
	printf("***** test_2_4 *****\n");
	id = IntDeque_new(MAX);
	for (j = 0; j < MAX+1; j++) {
		id->begin = id->end = j;
		assert(IntDeque_size(id) == 0);
		/* insert */
		/* 空状態 */
		for (i = 0; i < MAX/2; i++) {
			assert(id_cmp(0, piyo, i) == 0);
			assert(IntDeque_insert(id, i, piyo[i]));
		}
		/* 先頭・前寄り */
		assert(IntDeque_insert(id, 0, piyo[0]));
		assert(IntDeque_insert(id, 1, piyo[1]));
		assert(IntDeque_insert(id, 2, piyo[2]));
		assert(id_cmp(0, piyo, 3) == 0);
		assert(id_cmp(3, piyo, MAX/2) == 0);
		assert(IntDeque_size(id) == MAX/2 + 3);

		/* erase */
		/* 先頭・前寄り */
		IntDeque_erase(id, 2, 1);
		assert(id_cmp(0, piyo, 2) == 0);
		assert(id_cmp(2, piyo, MAX/2) == 0);
		assert(IntDeque_size(id) == MAX/2 + 2);
		IntDeque_erase(id, 1, 1);
		assert(id_cmp(0, piyo, 1) == 0);
		assert(id_cmp(1, piyo, MAX/2) == 0);
		assert(IntDeque_size(id) == MAX/2 + 1);
		IntDeque_erase(id, 0, 1);
		assert(id_cmp(0, piyo, MAX/2) == 0);
		assert(IntDeque_size(id) == MAX/2);

		/* insert */
		/* 末尾・後ろ寄り */
		assert(IntDeque_insert(id, MAX/2, piyo[0]));
		assert(IntDeque_insert(id, MAX/2, piyo[1]));
		assert(IntDeque_insert(id, MAX/2, piyo[2]));
		assert(id_cmp(0, piyo, MAX/2) == 0);
		assert(*IntDeque_at(id, MAX/2 + 0) == piyo[2]);
		assert(*IntDeque_at(id, MAX/2 + 1) == piyo[1]);
		assert(*IntDeque_at(id, MAX/2 + 2) == piyo[0]);

		/* erase */
		/* 末尾・後ろ寄り */
		IntDeque_erase(id, MAX/2, 1);
		assert(id_cmp(0, piyo, MAX/2) == 0);
		assert(*IntDeque_at(id, MAX/2 + 0) == piyo[1]);
		assert(*IntDeque_at(id, MAX/2 + 1) == piyo[0]);
		assert(IntDeque_size(id) == MAX/2 + 2);
		IntDeque_erase(id, MAX/2, 1);
		assert(id_cmp(0, piyo, MAX/2) == 0);
		assert(*IntDeque_at(id, MAX/2 + 0) == piyo[0]);
		assert(IntDeque_size(id) == MAX/2 + 1);
		IntDeque_erase(id, MAX/2, 1);
		assert(id_cmp(0, piyo, MAX/2) == 0);
		assert(IntDeque_size(id) == MAX/2);

		/* 先頭から全削除 */
		for (i = 0; i < MAX/2; i++) {
			assert(id_cmp(0, &piyo[i], MAX/2 - i) == 0);
			IntDeque_erase(id, 0, 1);
		}
		assert(IntDeque_size(id) == 0);

		for (i = 0; i < MAX/2; i++) {
			assert(id_cmp(0, piyo, i) == 0);
			assert(IntDeque_insert(id, i, piyo[i]));
		}
		assert(IntDeque_size(id) == MAX/2);

		/* 末尾から全削除 */
		for (i = 0; i < MAX/2; i++) {
			assert(id_cmp(0, piyo, MAX/2 - i) == 0);
			IntDeque_erase(id, IntDeque_size(id) -1, 1);
		}
		assert(IntDeque_size(id) == 0);

		/* MAX以上で失敗 */
		assert(IntDeque_assign_array(id, piyo, MAX));
		assert(id_cmp(0, piyo, MAX) == 0);
		assert(IntDeque_size(id) == MAX);
		assert(IntDeque_full(id));
		assert(!IntDeque_insert(id, 0, piyo[0]));
		assert(id_cmp(0, piyo, MAX) == 0);
		assert(IntDeque_size(id) == MAX);
		assert(IntDeque_full(id));
	}

	IntDeque_delete(id);
}

void DequeTest_test_2_5(void)
{
	int i;
	printf("***** test_2_5 *****\n");
	id = IntDeque_new(MAX);
	for (i = 0; i < MAX+1; i++) {
		id->begin = id->end = i;
		assert(IntDeque_size(id) == 0);
		/* insert_n */
		/* 空状態 */
		assert(IntDeque_insert_n(id, 0, piyo, MAX/2));
		assert(id_cmp(0, piyo, MAX/2) == 0);
		assert(IntDeque_size(id) == MAX/2);

		/* insert_n */
		/* 先頭 */
		assert(IntDeque_insert_n(id, 0, piyo, 3));
		assert(id_cmp(0, piyo, 3) == 0);
		assert(id_cmp(3, piyo, MAX/2) == 0);
		assert(IntDeque_size(id) == MAX/2 + 3);
		/* erase */
		IntDeque_erase(id, 0, 3);
		assert(id_cmp(0, piyo, MAX/2) == 0);
		assert(IntDeque_size(id) == MAX/2);

		/* insert_n */
		/* 前寄り */
		assert(IntDeque_insert_n(id, 2, piyo, 3));
		assert(id_cmp(0, piyo, 2) == 0);
		assert(id_cmp(2, piyo, 3) == 0);
		assert(id_cmp(5, &piyo[2], MAX/2 -2) == 0);
		assert(IntDeque_size(id) == MAX/2 + 3);
		/* 0個指定 */
		assert(IntDeque_insert_n(id, 1, &piyo[MAX/2], 0));
		assert(id_cmp(0, piyo, 2) == 0);
		assert(id_cmp(2, piyo, 3) == 0);
		assert(id_cmp(5, &piyo[2], MAX/2 -2) == 0);
		assert(IntDeque_size(id) == MAX/2 + 3);
		/* 1個指定 */
		assert(IntDeque_insert_n(id, 1, &piyo[MAX-1], 1));
		assert(id_cmp(0, &piyo[0], 1) == 0);
		assert(id_cmp(1, &piyo[MAX-1], 1) == 0);
		assert(id_cmp(2, &piyo[1], 1) == 0);
		assert(id_cmp(3, &piyo[0], 3) == 0);
		assert(id_cmp(6, &piyo[2], MAX/2 -2) == 0);
		assert(IntDeque_size(id) == MAX/2 + 4);
		/* erase */
		/* 0個指定 */
		IntDeque_erase(id, 1, 0);
		assert(id_cmp(0, &piyo[0], 1) == 0);
		assert(id_cmp(1, &piyo[MAX-1], 1) == 0);
		assert(id_cmp(2, &piyo[1], 1) == 0);
		assert(id_cmp(3, &piyo[0], 3) == 0);
		assert(id_cmp(6, &piyo[2], MAX/2 -2) == 0);
		assert(IntDeque_size(id) == MAX/2 + 4);
		/* 1個指定 */
		IntDeque_erase(id, 1, 1);
		assert(id_cmp(0, piyo, 2) == 0);
		assert(id_cmp(2, piyo, 3) == 0);
		assert(id_cmp(5, &piyo[2], MAX/2 -2) == 0);
		assert(IntDeque_size(id) == MAX/2 + 3);
		/* 前寄り */
		IntDeque_erase(id, 2, 3);
		assert(id_cmp(0, piyo, MAX/2) == 0);
		assert(IntDeque_size(id) == MAX/2);

		/* insert_n */
		/* 末尾 */
		assert(IntDeque_insert_n(id, IntDeque_size(id), piyo, 3));
		assert(id_cmp(0, piyo, MAX/2) == 0);
		assert(id_cmp(MAX/2, piyo, 3) == 0);
		assert(IntDeque_size(id) == MAX/2 + 3);
		/* erase */
		IntDeque_erase(id, IntDeque_size(id) - 3, 3);
		assert(id_cmp(0, piyo, MAX/2) == 0);
		assert(IntDeque_size(id) == MAX/2);

		/* insert_n */
		/* 後ろ寄り */
		assert(IntDeque_insert_n(id, IntDeque_size(id) - 2, piyo, 3));
		assert(id_cmp(0, piyo, MAX/2 -2) == 0);
		assert(id_cmp(MAX/2 -2, piyo, 3) == 0);
		assert(id_cmp(MAX/2 +1, &piyo[MAX/2 -2], 2) == 0);
		assert(IntDeque_size(id) == MAX/2 + 3);
		/* 0個指定 */
		assert(IntDeque_insert_n(id, IntDeque_size(id) - 1, &piyo[MAX-1], 0));
		assert(id_cmp(0, piyo, MAX/2 -2) == 0);
		assert(id_cmp(MAX/2 -2, piyo, 3) == 0);
		assert(id_cmp(MAX/2 +1, &piyo[MAX/2 -2], 2) == 0);
		assert(IntDeque_size(id) == MAX/2 + 3);
		/* 1個指定 */
		assert(IntDeque_insert_n(id, IntDeque_size(id) - 1, &piyo[MAX-1], 1));
		assert(id_cmp(0, piyo, MAX/2 -2) == 0);
		assert(id_cmp(MAX/2 -2, piyo, 3) == 0);
		assert(id_cmp(MAX/2 +1, &piyo[MAX/2 -2], 1) == 0);
		assert(id_cmp(MAX/2 +2, &piyo[MAX-1], 1) == 0);
		assert(id_cmp(MAX/2 +3, &piyo[MAX/2 -1], 1) == 0);
		assert(IntDeque_size(id) == MAX/2 + 4);
		/* erase */
		/* 0個指定 */
		IntDeque_erase(id, IntDeque_size(id) - 2, 0);
		assert(id_cmp(0, piyo, MAX/2 -2) == 0);
		assert(id_cmp(MAX/2 -2, piyo, 3) == 0);
		assert(id_cmp(MAX/2 +1, &piyo[MAX/2 -2], 1) == 0);
		assert(id_cmp(MAX/2 +2, &piyo[MAX-1], 1) == 0);
		assert(id_cmp(MAX/2 +3, &piyo[MAX/2 -1], 1) == 0);
		assert(IntDeque_size(id) == MAX/2 + 4);
		/* 1個指定 */
		IntDeque_erase(id, IntDeque_size(id) - 2, 1);
		assert(id_cmp(0, piyo, MAX/2 -2) == 0);
		assert(id_cmp(MAX/2 -2, piyo, 3) == 0);
		assert(id_cmp(MAX/2 +1, &piyo[MAX/2 -2], 2) == 0);
		assert(IntDeque_size(id) == MAX/2 + 3);
		/* 後ろ寄り */
		IntDeque_erase(id, IntDeque_size(id) - 5, 3);
		assert(id_cmp(0, piyo, MAX/2) == 0);
		assert(IntDeque_size(id) == MAX/2);

		/* MAX以上で失敗 */
		assert(!IntDeque_insert_n(id, 0, piyo, MAX - MAX/2 +1));
		assert(id_cmp(0, piyo, MAX/2) == 0);
		assert(IntDeque_size(id) == MAX/2);
		/* MAXになるまで */
		assert(IntDeque_insert_n(id, 0, piyo, MAX - MAX/2));
		assert(IntDeque_size(id) == MAX);
		assert(IntDeque_full(id));
		/* 先頭から全削除 */
		IntDeque_erase(id, 0, IntDeque_size(id));
		assert(IntDeque_size(id) == 0);
	}

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

