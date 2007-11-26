#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "../cstl/vector.h"
#include "../cstl/algorithm.h"
#include <vector>
#include <algorithm>
#include <functional>
#include "heap.h"
#ifdef MY_MALLOC
double buf[10*1024*1024/sizeof(double)];
Heap heap;
#define malloc(s)		Heap_alloc(&heap, s)
#define realloc(p, s)	Heap_realloc(&heap, p, s)
#define free(p)			Heap_free(&heap, p)
#endif

#include "hoge.h"

#define NELEMS(array)	(sizeof(array) / sizeof(array[0]))

#ifdef CODEGEN
#include "IntVector.h"
#include "HogeVector.h"
#else

CSTL_VECTOR_INTERFACE(IntVector, int)
CSTL_VECTOR_INTERFACE(HogeVector, Hoge)

CSTL_VECTOR_IMPLEMENT(IntVector, int)
CSTL_VECTOR_IMPLEMENT(HogeVector, Hoge)
#endif

#define COUNT	(1000000)
using namespace std;


int int_less(const void *x, const void *y)
{
	if (*(int*)x < *(int*)y) {
		return -1;
	} else if (*(int*)x > *(int*)y) {
		return 1;
	} else {
		return 0;
	}
}

int int_greater(const void *x, const void *y)
{
	if (*(int*)x < *(int*)y) {
		return 1;
	} else if (*(int*)x > *(int*)y) {
		return -1;
	} else {
		return 0;
	}
}

int hoge_less(const void *p1, const void *p2)
{
	return strcmp(((Hoge*)p1)->key, ((Hoge*)p2)->key);
}

int hoge_greater(const void *p1, const void *p2)
{
	return strcmp(((Hoge*)p2)->key, ((Hoge*)p1)->key);
}

void AlgoTest_test_1_1(void)
{
	static int buf[COUNT];
	IntVector *x;
	vector<int> y;
	int i;
	printf("***** test_1_1 *****\n");
	x = IntVector_new(COUNT);
	assert(x);

	srand(time(0));
	/* sort */
	for (i = 0; i < COUNT; i++) {
		buf[i] = rand();
		IntVector_push_back(x, buf[i]);
		y.push_back(buf[i]);
	}
	IntVector_sort(x, 0, IntVector_size(x), int_less);
	sort(y.begin(), y.end(), less<int>());
	for (i = 0; i < COUNT; i++) {
		if (y[i] != *IntVector_at(x, i)) {
			printf("NG: x[%d]:%d, y[%d]:%d\n", i, *IntVector_at(x, i), i, y[i]);
			assert(0);
		}
	}
	/* sortçœÇ›Çsort */
	IntVector_sort(x, 0, IntVector_size(x), int_less);
	sort(y.begin(), y.end(), less<int>());
	for (i = 0; i < COUNT; i++) {
		if (y[i] != *IntVector_at(x, i)) {
			printf("NG: x[%d]:%d, y[%d]:%d\n", i, *IntVector_at(x, i), i, y[i]);
			assert(0);
		}
	}
	/* ãtèáÇ…sort */
	IntVector_sort(x, 0, IntVector_size(x), int_greater);
	sort(y.begin(), y.end(), greater<int>());
	for (i = 0; i < COUNT; i++) {
		if (y[i] != *IntVector_at(x, i)) {
			printf("NG: x[%d]:%d, y[%d]:%d\n", i, *IntVector_at(x, i), i, y[i]);
			assert(0);
		}
	}
	/* sortçœÇ›Çsort */
	IntVector_sort(x, 0, IntVector_size(x), int_greater);
	sort(y.begin(), y.end(), greater<int>());
	for (i = 0; i < COUNT; i++) {
		if (y[i] != *IntVector_at(x, i)) {
			printf("NG: x[%d]:%d, y[%d]:%d\n", i, *IntVector_at(x, i), i, y[i]);
			assert(0);
		}
	}

	IntVector_delete(x);
}

void AlgoTest_test_1_2(void)
{
	static int buf[COUNT];
	IntVector *x;
	vector<int> y;
	int i;
	printf("***** test_1_2 *****\n");
	x = IntVector_new(COUNT);

	srand(time(0));
	/* stable_sort */
	for (i = 0; i < COUNT; i++) {
		buf[i] = rand();
		IntVector_push_back(x, buf[i]);
		y.push_back(buf[i]);
	}
	IntVector_stable_sort(x, 0, IntVector_size(x), int_less);
	stable_sort(y.begin(), y.end(), less<int>());
	for (i = 0; i < COUNT; i++) {
		if (y[i] != *IntVector_at(x, i)) {
			printf("NG: x[%d]:%d, y[%d]:%d\n", i, *IntVector_at(x, i), i, y[i]);
			assert(0);
		}
	}
	/* stable_sortçœÇ›Çstable_sort */
	IntVector_stable_sort(x, 0, IntVector_size(x), int_less);
	stable_sort(y.begin(), y.end(), less<int>());
	for (i = 0; i < COUNT; i++) {
		if (y[i] != *IntVector_at(x, i)) {
			printf("NG: x[%d]:%d, y[%d]:%d\n", i, *IntVector_at(x, i), i, y[i]);
			assert(0);
		}
	}
	/* ãtèáÇ…stable_sort */
	IntVector_stable_sort(x, 0, IntVector_size(x), int_greater);
	stable_sort(y.begin(), y.end(), greater<int>());
	for (i = 0; i < COUNT; i++) {
		if (y[i] != *IntVector_at(x, i)) {
			printf("NG: x[%d]:%d, y[%d]:%d\n", i, *IntVector_at(x, i), i, y[i]);
			assert(0);
		}
	}
	/* stable_sortçœÇ›Çstable_sort */
	IntVector_stable_sort(x, 0, IntVector_size(x), int_greater);
	stable_sort(y.begin(), y.end(), greater<int>());
	for (i = 0; i < COUNT; i++) {
		if (y[i] != *IntVector_at(x, i)) {
			printf("NG: x[%d]:%d, y[%d]:%d\n", i, *IntVector_at(x, i), i, y[i]);
			assert(0);
		}
	}

	IntVector_delete(x);
}

void AlgoTest_test_1_3(void)
{
#ifdef MY_MALLOC
	static int buf[COUNT];
	IntVector *x;
	vector<int> y;
	int i;
	printf("***** test_1_3 *****\n");
	x = IntVector_new(COUNT);

	srand(time(0));
	/* stable_sort */
	/* merge_without_buffer */
	for (i = 0; i < COUNT; i++) {
		buf[i] = rand();
		IntVector_push_back(x, buf[i]);
		y.push_back(buf[i]);
	}
	HEAP_SET_FAIL_COUNT(&heap, 0);
	IntVector_stable_sort(x, 0, IntVector_size(x), int_less);
	stable_sort(y.begin(), y.end(), less<int>());
	for (i = 0; i < COUNT; i++) {
		if (y[i] != *IntVector_at(x, i)) {
			printf("NG: x[%d]:%d, y[%d]:%d\n", i, *IntVector_at(x, i), i, y[i]);
			assert(0);
		}
	}
	/* stable_sortçœÇ›Çstable_sort */
	IntVector_stable_sort(x, 0, IntVector_size(x), int_less);
	stable_sort(y.begin(), y.end(), less<int>());
	for (i = 0; i < COUNT; i++) {
		if (y[i] != *IntVector_at(x, i)) {
			printf("NG: x[%d]:%d, y[%d]:%d\n", i, *IntVector_at(x, i), i, y[i]);
			assert(0);
		}
	}
	/* ãtèáÇ…stable_sort */
	IntVector_stable_sort(x, 0, IntVector_size(x), int_greater);
	stable_sort(y.begin(), y.end(), greater<int>());
	for (i = 0; i < COUNT; i++) {
		if (y[i] != *IntVector_at(x, i)) {
			printf("NG: x[%d]:%d, y[%d]:%d\n", i, *IntVector_at(x, i), i, y[i]);
			assert(0);
		}
	}
	/* stable_sortçœÇ›Çstable_sort */
	IntVector_stable_sort(x, 0, IntVector_size(x), int_greater);
	stable_sort(y.begin(), y.end(), greater<int>());
	for (i = 0; i < COUNT; i++) {
		if (y[i] != *IntVector_at(x, i)) {
			printf("NG: x[%d]:%d, y[%d]:%d\n", i, *IntVector_at(x, i), i, y[i]);
			assert(0);
		}
	}

	HEAP_RESET_FAIL_COUNT(&heap);
	IntVector_delete(x);
#endif
}

static Hoge hogetab[] = {
	{"001",  10}, 
	{"009",  90}, 
	{"010", 100}, 
	{"002",  20}, 
	{"012", 120}, 
	{"003",  30}, 
	{"012", 121}, 
	{"007",  70}, 
	{"008",  80}, 
	{"018", 180}, 
	{"003",  31}, 
	{"000",  00}, 
	{"006",  60}, 
	{"004",  40}, 
	{"009",  91}, 
	{"012", 122}, 
	{"005",  50}, 
	{"015", 150}, 
	{"011", 110}, 
	{"013", 130}, 
	{"012", 123}, 
	{"015", 151}, 
	{"014", 140}, 
	{"003",  32}, 
	{"017", 170}, 
	{"016", 160}, 
};

void AlgoTest_test_1_4(void)
{
	Hoge hoge;
	Hoge prev;
	HogeVector *x;
	size_t i;
	printf("***** test_1_4 *****\n");
	x = HogeVector_new(COUNT);

	srand(time(0));
	/* stable_sort */
	for (i = 0; i < NELEMS(hogetab); i++) {
		assert(HogeVector_push_back(x, hogetab[i]));
	}
	HogeVector_stable_sort(x, 0, HogeVector_size(x), hoge_less);
	prev.key = "000", prev.value = 0;
	for (i = 0; i < NELEMS(hogetab); i++) {
		hoge = *HogeVector_at(x, i);
		printf("%s, %4d\n", hoge.key, hoge.value);
		assert(strcmp(prev.key, hoge.key) <= 0);
		assert(prev.value <= hoge.value);
		prev = hoge;
	}
	printf("\n");
	/* stable_sortçœÇ›Çstable_sort */
	HogeVector_stable_sort(x, 0, HogeVector_size(x), hoge_less);
	prev.key = "000", prev.value = 0;
	for (i = 0; i < NELEMS(hogetab); i++) {
		hoge = *HogeVector_at(x, i);
		printf("%s, %4d\n", hoge.key, hoge.value);
		assert(strcmp(prev.key, hoge.key) <= 0);
		assert(prev.value <= hoge.value);
		prev = hoge;
	}
	printf("\n");
	/* ãtèáÇ…stable_sort */
	HogeVector_clear(x);
	for (i = 0; i < NELEMS(hogetab); i++) {
		assert(HogeVector_push_back(x, hogetab[i]));
	}
	HogeVector_stable_sort(x, 0, HogeVector_size(x), hoge_greater);
	prev.key = "999", prev.value = 999;
	for (i = 0; i < NELEMS(hogetab); i++) {
		hoge = *HogeVector_at(x, i);
		printf("%s, %4d\n", hoge.key, hoge.value);
		assert(strcmp(prev.key, hoge.key) >= 0);
		if (strcmp(prev.key, hoge.key) == 0) {
			assert(prev.value <= hoge.value);
		} else {
			assert(prev.value >= hoge.value);
		}
		prev = hoge;
	}
	printf("\n");
	/* stable_sortçœÇ›Çstable_sort */
	HogeVector_stable_sort(x, 0, HogeVector_size(x), hoge_greater);
	prev.key = "999", prev.value = 999;
	for (i = 0; i < NELEMS(hogetab); i++) {
		hoge = *HogeVector_at(x, i);
		printf("%s, %4d\n", hoge.key, hoge.value);
		assert(strcmp(prev.key, hoge.key) >= 0);
		if (strcmp(prev.key, hoge.key) == 0) {
			assert(prev.value <= hoge.value);
		} else {
			assert(prev.value >= hoge.value);
		}
		prev = hoge;
	}

	HogeVector_delete(x);
}

void AlgoTest_test_1_5(void)
{
#ifdef MY_MALLOC
	Hoge hoge;
	Hoge prev;
	HogeVector *x;
	size_t i;
	printf("***** test_1_5 *****\n");
	x = HogeVector_new(COUNT);

	srand(time(0));
	/* stable_sort */
	/* merge_without_buffer */
	for (i = 0; i < NELEMS(hogetab); i++) {
		assert(HogeVector_push_back(x, hogetab[i]));
	}
	HEAP_SET_FAIL_COUNT(&heap, 0);
	HogeVector_stable_sort(x, 0, HogeVector_size(x), hoge_less);
	prev.key = "000", prev.value = 0;
	for (i = 0; i < NELEMS(hogetab); i++) {
		hoge = *HogeVector_at(x, i);
		printf("%s, %4d\n", hoge.key, hoge.value);
		assert(strcmp(prev.key, hoge.key) <= 0);
		assert(prev.value <= hoge.value);
		prev = hoge;
	}
	printf("\n");
	/* stable_sortçœÇ›Çstable_sort */
	HogeVector_stable_sort(x, 0, HogeVector_size(x), hoge_less);
	prev.key = "000", prev.value = 0;
	for (i = 0; i < NELEMS(hogetab); i++) {
		hoge = *HogeVector_at(x, i);
		printf("%s, %4d\n", hoge.key, hoge.value);
		assert(strcmp(prev.key, hoge.key) <= 0);
		assert(prev.value <= hoge.value);
		prev = hoge;
	}
	printf("\n");
	/* ãtèáÇ…stable_sort */
	HogeVector_clear(x);
	for (i = 0; i < NELEMS(hogetab); i++) {
		assert(HogeVector_push_back(x, hogetab[i]));
	}
	HogeVector_stable_sort(x, 0, HogeVector_size(x), hoge_greater);
	prev.key = "999", prev.value = 999;
	for (i = 0; i < NELEMS(hogetab); i++) {
		hoge = *HogeVector_at(x, i);
		printf("%s, %4d\n", hoge.key, hoge.value);
		assert(strcmp(prev.key, hoge.key) >= 0);
		if (strcmp(prev.key, hoge.key) == 0) {
			assert(prev.value <= hoge.value);
		} else {
			assert(prev.value >= hoge.value);
		}
		prev = hoge;
	}
	printf("\n");
	/* stable_sortçœÇ›Çstable_sort */
	HogeVector_stable_sort(x, 0, HogeVector_size(x), hoge_greater);
	prev.key = "999", prev.value = 999;
	for (i = 0; i < NELEMS(hogetab); i++) {
		hoge = *HogeVector_at(x, i);
		printf("%s, %4d\n", hoge.key, hoge.value);
		assert(strcmp(prev.key, hoge.key) >= 0);
		if (strcmp(prev.key, hoge.key) == 0) {
			assert(prev.value <= hoge.value);
		} else {
			assert(prev.value >= hoge.value);
		}
		prev = hoge;
	}

	HEAP_RESET_FAIL_COUNT(&heap);
	HogeVector_delete(x);
#endif
}

void AlgoTest_run(void)
{
	printf("\n===== algorithm test =====\n");
	AlgoTest_test_1_1();
	AlgoTest_test_1_2();
	AlgoTest_test_1_3();
	AlgoTest_test_1_4();
	AlgoTest_test_1_5();
}


int main(void)
{
#ifdef MY_MALLOC
	Heap_init(&heap, buf, sizeof buf, sizeof buf[0]);
#endif
	AlgoTest_run();
#ifdef MY_MALLOC
	HEAP_DUMP_LEAK(&heap, 0);
#endif
	return 0;
}
