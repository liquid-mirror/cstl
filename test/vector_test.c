#include "test.h"


VECTOR_IMPLEMENT(UCharVector, unsigned char)
VECTOR_IMPLEMENT(IntVector, int)

static UCharVector *uv;
static IntVector *iv;

#define SIZE	(VECTOR_INIT_SIZE-1)


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
	uv = UCharVector_new();
	vector_init_hoge();
	/* 初期状態 */
	assert(UCharVector_capacity(uv) == SIZE);
	assert(UCharVector_size(uv) == 0);
	assert(UCharVector_empty(uv));
	/* assign */
	assert(UCharVector_assign(uv, hoge, SIZE));
	assert(memcmp(UCharVector_at(uv, 0), hoge, SIZE) == 0);
	assert(UCharVector_capacity(uv) == SIZE);
	assert(UCharVector_size(uv) == SIZE);
	assert(!UCharVector_empty(uv));
	/* clear */
	UCharVector_clear(uv);
	assert(UCharVector_size(uv) == 0);
	assert(UCharVector_empty(uv));
	/* assign */
	assert(UCharVector_assign(uv, &hoge[1], 255));
	assert(memcmp(UCharVector_at(uv, 0), &hoge[1], 255) == 0);
	a = UCharVector_capacity(uv);
	assert(a > SIZE);
	assert(UCharVector_size(uv) == 255);
	assert(!UCharVector_empty(uv));
	/* clear */
	UCharVector_clear(uv);
	assert(UCharVector_size(uv) == 0);
	assert(UCharVector_empty(uv));
	/* assign */
	assert(UCharVector_assign(uv, hoge, SIZE));
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

	UCharVector_delete(uv);
}

void VectorTest_test_1_2(void)
{
	int i;
	size_t a;
	printf("***** test_1_2 *****\n");
	uv = UCharVector_new();
	vector_init_hoge();
	/* push_back */
	for (i = 0; i < SIZE; i++) {
		assert(UCharVector_push_back(uv, hoge[i]));
		assert(UCharVector_size(uv) == i+1);
		assert(UCharVector_front(uv) == hoge[0]);
		assert(UCharVector_back(uv) == hoge[i]);
	}
	assert(memcmp(UCharVector_at(uv, 0), hoge, SIZE) == 0);
	assert(UCharVector_capacity(uv) == SIZE);
	assert(UCharVector_size(uv) == SIZE);
	assert(!UCharVector_empty(uv));
	for (i = i; i < 256; i++) {
		assert(UCharVector_push_back(uv, hoge[i]));
		assert(UCharVector_size(uv) == i+1);
		assert(UCharVector_front(uv) == hoge[0]);
		assert(UCharVector_back(uv) == hoge[i]);
	}
	assert(memcmp(UCharVector_at(uv, 0), hoge, 256) == 0);
	a = UCharVector_capacity(uv);
	assert(a > SIZE);
	assert(UCharVector_size(uv) == 256);
	assert(!UCharVector_empty(uv));
	/* front, back */
	assert(UCharVector_front(uv) == hoge[0]);
	assert(UCharVector_back(uv) == hoge[255]);
	/* pop_back */
	for (i = 255; i >= 0; i--) {
		assert(UCharVector_front(uv) == hoge[0]);
		assert(UCharVector_back(uv) == hoge[i]);
		assert(UCharVector_pop_back(uv) == hoge[i]);
		assert(UCharVector_size(uv) == i);
	}
	assert(UCharVector_size(uv) == 0);
	assert(UCharVector_empty(uv));
	assert(UCharVector_capacity(uv) == a);

	UCharVector_delete(uv);
}

void VectorTest_test_1_3(void)
{
	int i;
	printf("***** test_1_3 *****\n");
	uv = UCharVector_new();
	vector_init_hoge();
	/* assign */
	assert(UCharVector_assign(uv, hoge, SIZE));
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

	UCharVector_delete(uv);
}

void VectorTest_test_1_4(void)
{
	int i;
	unsigned char buf[256];
	printf("***** test_1_4 *****\n");
	uv = UCharVector_new();
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
	UCharVector_erase(uv, SIZE);
	assert(memcmp(UCharVector_at(uv, 0), buf, SIZE) == 0);
	assert(memcmp(&UCharVector_at(uv, 0)[SIZE], &buf[SIZE], 256-SIZE) == 0);
	for (i = SIZE; i < 256; i++) {
		UCharVector_erase(uv, UCharVector_size(uv)-1);
	}
	assert(memcmp(UCharVector_at(uv, 0), buf, SIZE) == 0);
	assert(UCharVector_size(uv) == SIZE);
	for (i = 0; i < SIZE; i++) {
		assert(memcmp(UCharVector_at(uv, 0), &buf[i], SIZE-i) == 0);
		UCharVector_erase(uv, 0);
		assert(UCharVector_size(uv) == SIZE-i-1);
	}
	assert(UCharVector_size(uv) == 0);

	UCharVector_delete(uv);
}

void VectorTest_test_1_5(void)
{
	printf("***** test_1_5 *****\n");
	uv = UCharVector_new();
	vector_init_hoge();
	/* insert_n */
	assert(UCharVector_insert_n(uv, 0, hoge, SIZE));
	assert(memcmp(UCharVector_at(uv, 0), hoge, SIZE) == 0);
	assert(UCharVector_capacity(uv) == SIZE);
	assert(UCharVector_size(uv) == SIZE);

	assert(UCharVector_insert_n(uv, UCharVector_size(uv), &hoge[SIZE], 256-SIZE));
	assert(memcmp(UCharVector_at(uv, 0), hoge, 256) == 0);
	assert(UCharVector_capacity(uv) > SIZE);
	assert(UCharVector_size(uv) == 256);

	assert(UCharVector_insert_n(uv, SIZE, hoge, SIZE));
	assert(memcmp(UCharVector_at(uv, 0), hoge, SIZE) == 0);
	assert(memcmp(&UCharVector_at(uv, 0)[SIZE], hoge, 256) == 0);
	assert(UCharVector_size(uv) == 256+SIZE);

	assert(UCharVector_insert_n(uv, 0, hoge, 1));
	assert(*UCharVector_at(uv, 0) == hoge[0]);
	assert(memcmp(&UCharVector_at(uv, 0)[1], hoge, SIZE) == 0);
	assert(memcmp(&UCharVector_at(uv, 0)[SIZE+1], hoge, 256) == 0);
	assert(UCharVector_size(uv) == 257+SIZE);

	assert(UCharVector_insert_n(uv, 0, hoge, 0));
	assert(*UCharVector_at(uv, 0) == hoge[0]);
	assert(memcmp(&UCharVector_at(uv, 0)[1], hoge, SIZE) == 0);
	assert(memcmp(&UCharVector_at(uv, 0)[SIZE+1], hoge, 256) == 0);
	assert(UCharVector_size(uv) == 257+SIZE);
	/* erase_n */
	UCharVector_erase_n(uv, 0, 1);
	assert(memcmp(UCharVector_at(uv, 0), hoge, SIZE) == 0);
	assert(memcmp(&UCharVector_at(uv, 0)[SIZE], hoge, 256) == 0);
	assert(UCharVector_size(uv) == 256+SIZE);

	UCharVector_erase_n(uv, SIZE, SIZE);
	assert(memcmp(UCharVector_at(uv, 0), hoge, 256) == 0);
	assert(UCharVector_size(uv) == 256);

	UCharVector_erase_n(uv, SIZE, 256-SIZE);
	assert(memcmp(UCharVector_at(uv, 0), hoge, SIZE) == 0);
	assert(UCharVector_size(uv) == SIZE);

	UCharVector_erase_n(uv, 0, SIZE);
	assert(UCharVector_size(uv) == 0);

	UCharVector_delete(uv);
}

void VectorTest_test_1_6(void)
{
	UCharVector *x;
	size_t i;
	printf("***** test_1_6 *****\n");
	uv = UCharVector_new();
	vector_init_hoge();

	assert(UCharVector_assign(uv, hoge, SIZE));
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
	/* new_copy */
	x = UCharVector_new_copy(uv);
	assert(UCharVector_capacity(x) == 10);
	assert(UCharVector_size(x) == 10);
	for (i = 0; i < UCharVector_size(uv); i++) {
		assert(*UCharVector_at(x, i) == *UCharVector_at(uv, i));
	}

	UCharVector_delete(uv);
	UCharVector_delete(x);
}


void VectorTest_test_2_1(void)
{
	size_t a;
	printf("***** test_2_1 *****\n");
	iv = IntVector_new();
	vector_init_piyo();
	/* 初期状態 */
	assert(IntVector_capacity(iv) == SIZE);
	assert(IntVector_size(iv) == 0);
	assert(IntVector_empty(iv));
	/* assign */
	assert(IntVector_assign(iv, piyo, SIZE));
	assert(memcmp(IntVector_at(iv, 0), piyo, SIZE) == 0);
	assert(IntVector_capacity(iv) == SIZE);
	assert(IntVector_size(iv) == SIZE);
	assert(!IntVector_empty(iv));
	/* clear */
	IntVector_clear(iv);
	assert(IntVector_size(iv) == 0);
	assert(IntVector_empty(iv));
	/* assign */
	assert(IntVector_assign(iv, &piyo[1], 255));
	assert(memcmp(IntVector_at(iv, 0), &piyo[1], 255) == 0);
	a = IntVector_capacity(iv);
	assert(a > SIZE);
	assert(IntVector_size(iv) == 255);
	assert(!IntVector_empty(iv));
	/* clear */
	IntVector_clear(iv);
	assert(IntVector_size(iv) == 0);
	assert(IntVector_empty(iv));
	/* assign */
	assert(IntVector_assign(iv, piyo, SIZE));
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

	IntVector_delete(iv);
}

void VectorTest_test_2_2(void)
{
	int i;
	size_t a;
	printf("***** test_2_2 *****\n");
	iv = IntVector_new();
	vector_init_piyo();
	/* push_back */
	for (i = 0; i < SIZE; i++) {
		assert(IntVector_push_back(iv, piyo[i]));
		assert(IntVector_size(iv) == i+1);
		assert(IntVector_front(iv) == piyo[0]);
		assert(IntVector_back(iv) == piyo[i]);
	}
	assert(memcmp(IntVector_at(iv, 0), piyo, SIZE) == 0);
	assert(IntVector_capacity(iv) == SIZE);
	assert(IntVector_size(iv) == SIZE);
	assert(!IntVector_empty(iv));
	for (i = i; i < 256; i++) {
		assert(IntVector_push_back(iv, piyo[i]));
		assert(IntVector_size(iv) == i+1);
		assert(IntVector_front(iv) == piyo[0]);
		assert(IntVector_back(iv) == piyo[i]);
	}
	assert(memcmp(IntVector_at(iv, 0), piyo, 256) == 0);
	a = IntVector_capacity(iv);
	assert(a > SIZE);
	assert(IntVector_size(iv) == 256);
	assert(!IntVector_empty(iv));
	/* front, back */
	assert(IntVector_front(iv) == piyo[0]);
	assert(IntVector_back(iv) == piyo[255]);
	/* pop_back */
	for (i = 255; i >= 0; i--) {
		assert(IntVector_front(iv) == piyo[0]);
		assert(IntVector_back(iv) == piyo[i]);
		assert(IntVector_pop_back(iv) == piyo[i]);
		assert(IntVector_size(iv) == i);
	}
	assert(IntVector_size(iv) == 0);
	assert(IntVector_empty(iv));
	assert(IntVector_capacity(iv) == a);

	IntVector_delete(iv);
}

void VectorTest_test_2_3(void)
{
	int i;
	printf("***** test_2_3 *****\n");
	iv = IntVector_new();
	vector_init_piyo();
	/* assign */
	assert(IntVector_assign(iv, piyo, SIZE));
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

	IntVector_delete(iv);
}

void VectorTest_test_2_4(void)
{
	int i;
	int buf[256];
	printf("***** test_2_4 *****\n");
	iv = IntVector_new();
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
	IntVector_erase(iv, SIZE);
	assert(memcmp(IntVector_at(iv, 0), buf, SIZE) == 0);
	assert(memcmp(&IntVector_at(iv, 0)[SIZE], &buf[SIZE], 256-SIZE) == 0);
	for (i = SIZE; i < 256; i++) {
		IntVector_erase(iv, IntVector_size(iv)-1);
	}
	assert(memcmp(IntVector_at(iv, 0), buf, SIZE) == 0);
	assert(IntVector_size(iv) == SIZE);
	for (i = 0; i < SIZE; i++) {
		assert(memcmp(IntVector_at(iv, 0), &buf[i], SIZE-i) == 0);
		IntVector_erase(iv, 0);
		assert(IntVector_size(iv) == SIZE-i-1);
	}
	assert(IntVector_size(iv) == 0);

	IntVector_delete(iv);
}

void VectorTest_test_2_5(void)
{
	printf("***** test_2_5 *****\n");
	iv = IntVector_new();
	vector_init_piyo();
	/* insert_n */
	assert(IntVector_insert_n(iv, 0, piyo, SIZE));
	assert(memcmp(IntVector_at(iv, 0), piyo, SIZE) == 0);
	assert(IntVector_capacity(iv) == SIZE);
	assert(IntVector_size(iv) == SIZE);

	assert(IntVector_insert_n(iv, IntVector_size(iv), &piyo[SIZE], 256-SIZE));
	assert(memcmp(IntVector_at(iv, 0), piyo, 256) == 0);
	assert(IntVector_capacity(iv) > SIZE);
	assert(IntVector_size(iv) == 256);

	assert(IntVector_insert_n(iv, SIZE, piyo, SIZE));
	assert(memcmp(IntVector_at(iv, 0), piyo, SIZE) == 0);
	assert(memcmp(&IntVector_at(iv, 0)[SIZE], piyo, 256) == 0);
	assert(IntVector_size(iv) == 256+SIZE);

	assert(IntVector_insert_n(iv, 0, piyo, 1));
	assert(*IntVector_at(iv, 0) == piyo[0]);
	assert(memcmp(&IntVector_at(iv, 0)[1], piyo, SIZE) == 0);
	assert(memcmp(&IntVector_at(iv, 0)[SIZE+1], piyo, 256) == 0);
	assert(IntVector_size(iv) == 257+SIZE);
	/* erase_n */
	IntVector_erase_n(iv, 0, 1);
	assert(memcmp(IntVector_at(iv, 0), piyo, SIZE) == 0);
	assert(memcmp(&IntVector_at(iv, 0)[SIZE], piyo, 256) == 0);
	assert(IntVector_size(iv) == 256+SIZE);

	IntVector_erase_n(iv, SIZE, SIZE);
	assert(memcmp(IntVector_at(iv, 0), piyo, 256) == 0);
	assert(IntVector_size(iv) == 256);

	IntVector_erase_n(iv, SIZE, 256-SIZE);
	assert(memcmp(IntVector_at(iv, 0), piyo, SIZE) == 0);
	assert(IntVector_size(iv) == SIZE);

	IntVector_erase_n(iv, 0, SIZE);
	assert(IntVector_size(iv) == 0);

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

