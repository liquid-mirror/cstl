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
#define malloc(s)		Heap_malloc(&heap, s)
#define realloc(p, s)	Heap_realloc(&heap, p, s)
#define free(p)			Heap_free(&heap, p)
#endif

#include "hoge.h"

#define NELEMS(array)	(sizeof(array) / sizeof(array[0]))

#ifdef CSTLGEN
#include "IntVector.h"
#include "HogeVector.h"
#else

CSTL_VECTOR_INTERFACE(IntVector, int)
CSTL_VECTOR_INTERFACE(HogeVector, Hoge)

CSTL_VECTOR_IMPLEMENT(IntVector, int)
CSTL_VECTOR_IMPLEMENT(HogeVector, Hoge)
#endif

#define SORT_COUNT	(1000000)
#define COUNT	(1000)
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
	static int buf[SORT_COUNT];
	IntVector *x;
	vector<int> y;
	int i;
	printf("***** test_1_1 *****\n");
	x = IntVector_new(SORT_COUNT);
	assert(x);

	srand(time(0));
	/* sort */
	for (i = 0; i < SORT_COUNT; i++) {
		buf[i] = rand();
		IntVector_push_back(x, buf[i]);
		y.push_back(buf[i]);
	}
	IntVector_sort(x, 0, IntVector_size(x), int_less);
	sort(y.begin(), y.end(), less<int>());
	for (i = 0; i < SORT_COUNT; i++) {
		if (y[i] != *IntVector_at(x, i)) {
			printf("NG: x[%d]:%d, y[%d]:%d\n", i, *IntVector_at(x, i), i, y[i]);
			assert(0);
		}
	}
	/* sort済みをsort */
	IntVector_sort(x, 0, IntVector_size(x), int_less);
	sort(y.begin(), y.end(), less<int>());
	for (i = 0; i < SORT_COUNT; i++) {
		if (y[i] != *IntVector_at(x, i)) {
			printf("NG: x[%d]:%d, y[%d]:%d\n", i, *IntVector_at(x, i), i, y[i]);
			assert(0);
		}
	}
	/* 逆順にsort */
	IntVector_sort(x, 0, IntVector_size(x), int_greater);
	sort(y.begin(), y.end(), greater<int>());
	for (i = 0; i < SORT_COUNT; i++) {
		if (y[i] != *IntVector_at(x, i)) {
			printf("NG: x[%d]:%d, y[%d]:%d\n", i, *IntVector_at(x, i), i, y[i]);
			assert(0);
		}
	}
	/* sort済みをsort */
	IntVector_sort(x, 0, IntVector_size(x), int_greater);
	sort(y.begin(), y.end(), greater<int>());
	for (i = 0; i < SORT_COUNT; i++) {
		if (y[i] != *IntVector_at(x, i)) {
			printf("NG: x[%d]:%d, y[%d]:%d\n", i, *IntVector_at(x, i), i, y[i]);
			assert(0);
		}
	}

	IntVector_delete(x);
}

void AlgoTest_test_1_2(void)
{
	static int buf[SORT_COUNT];
	IntVector *x;
	vector<int> y;
	int i;
	printf("***** test_1_2 *****\n");
	x = IntVector_new(SORT_COUNT);

	srand(time(0));
	/* stable_sort */
	for (i = 0; i < SORT_COUNT; i++) {
		buf[i] = rand();
		IntVector_push_back(x, buf[i]);
		y.push_back(buf[i]);
	}
	IntVector_stable_sort(x, 0, IntVector_size(x), int_less);
	stable_sort(y.begin(), y.end(), less<int>());
	for (i = 0; i < SORT_COUNT; i++) {
		if (y[i] != *IntVector_at(x, i)) {
			printf("NG: x[%d]:%d, y[%d]:%d\n", i, *IntVector_at(x, i), i, y[i]);
			assert(0);
		}
	}
	/* stable_sort済みをstable_sort */
	IntVector_stable_sort(x, 0, IntVector_size(x), int_less);
	stable_sort(y.begin(), y.end(), less<int>());
	for (i = 0; i < SORT_COUNT; i++) {
		if (y[i] != *IntVector_at(x, i)) {
			printf("NG: x[%d]:%d, y[%d]:%d\n", i, *IntVector_at(x, i), i, y[i]);
			assert(0);
		}
	}
	/* 逆順にstable_sort */
	IntVector_stable_sort(x, 0, IntVector_size(x), int_greater);
	stable_sort(y.begin(), y.end(), greater<int>());
	for (i = 0; i < SORT_COUNT; i++) {
		if (y[i] != *IntVector_at(x, i)) {
			printf("NG: x[%d]:%d, y[%d]:%d\n", i, *IntVector_at(x, i), i, y[i]);
			assert(0);
		}
	}
	/* stable_sort済みをstable_sort */
	IntVector_stable_sort(x, 0, IntVector_size(x), int_greater);
	stable_sort(y.begin(), y.end(), greater<int>());
	for (i = 0; i < SORT_COUNT; i++) {
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
	static int buf[SORT_COUNT];
	IntVector *x;
	vector<int> y;
	int i;
	printf("***** test_1_3 *****\n");
	x = IntVector_new(SORT_COUNT);

	srand(time(0));
	/* stable_sort */
	/* merge_without_buffer */
	for (i = 0; i < SORT_COUNT; i++) {
		buf[i] = rand();
		IntVector_push_back(x, buf[i]);
		y.push_back(buf[i]);
	}
	HEAP_SET_FAIL_COUNT(&heap, 0);
	IntVector_stable_sort(x, 0, IntVector_size(x), int_less);
	stable_sort(y.begin(), y.end(), less<int>());
	for (i = 0; i < SORT_COUNT; i++) {
		if (y[i] != *IntVector_at(x, i)) {
			printf("NG: x[%d]:%d, y[%d]:%d\n", i, *IntVector_at(x, i), i, y[i]);
			assert(0);
		}
	}
	/* stable_sort済みをstable_sort */
	IntVector_stable_sort(x, 0, IntVector_size(x), int_less);
	stable_sort(y.begin(), y.end(), less<int>());
	for (i = 0; i < SORT_COUNT; i++) {
		if (y[i] != *IntVector_at(x, i)) {
			printf("NG: x[%d]:%d, y[%d]:%d\n", i, *IntVector_at(x, i), i, y[i]);
			assert(0);
		}
	}
	/* 逆順にstable_sort */
	IntVector_stable_sort(x, 0, IntVector_size(x), int_greater);
	stable_sort(y.begin(), y.end(), greater<int>());
	for (i = 0; i < SORT_COUNT; i++) {
		if (y[i] != *IntVector_at(x, i)) {
			printf("NG: x[%d]:%d, y[%d]:%d\n", i, *IntVector_at(x, i), i, y[i]);
			assert(0);
		}
	}
	/* stable_sort済みをstable_sort */
	IntVector_stable_sort(x, 0, IntVector_size(x), int_greater);
	stable_sort(y.begin(), y.end(), greater<int>());
	for (i = 0; i < SORT_COUNT; i++) {
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
	x = HogeVector_new(SORT_COUNT);

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
	/* stable_sort済みをstable_sort */
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
	/* 逆順にstable_sort */
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
	/* stable_sort済みをstable_sort */
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
	x = HogeVector_new(SORT_COUNT);

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
	/* stable_sort済みをstable_sort */
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
	/* 逆順にstable_sort */
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
	/* stable_sort済みをstable_sort */
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

void AlgoTest_test_1_6(void)
{
	static int buf[SORT_COUNT];
	IntVector *x;
	vector<int> y;
	int i;
	printf("***** test_1_6 *****\n");
	x = IntVector_new(SORT_COUNT);
	assert(x);

	srand(time(0));
	/* sort */
	/* 部分的にソート */
	for (i = 0; i < SORT_COUNT; i++) {
		buf[i] = rand();
		IntVector_push_back(x, buf[i]);
		y.push_back(buf[i]);
	}
	size_t half_size = IntVector_size(x) / 2;
	IntVector_sort(x, half_size, IntVector_size(x) - half_size - 1000, int_less);
	sort(y.begin() + half_size, y.end() - 1000, less<int>());
	for (i = 0; i < SORT_COUNT; i++) {
		if (y[i] != *IntVector_at(x, i)) {
			printf("NG: x[%d]:%d, y[%d]:%d\n", i, *IntVector_at(x, i), i, y[i]);
			assert(0);
		}
	}
	/* sort済みをsort */
	IntVector_sort(x, half_size, IntVector_size(x) - half_size - 1000, int_less);
	sort(y.begin() + half_size, y.end() - 1000, less<int>());
	for (i = 0; i < SORT_COUNT; i++) {
		if (y[i] != *IntVector_at(x, i)) {
			printf("NG: x[%d]:%d, y[%d]:%d\n", i, *IntVector_at(x, i), i, y[i]);
			assert(0);
		}
	}
	/* 逆順にsort */
	IntVector_sort(x, half_size, IntVector_size(x) - half_size - 1000, int_greater);
	sort(y.begin() + half_size, y.end() - 1000, greater<int>());
	for (i = 0; i < SORT_COUNT; i++) {
		if (y[i] != *IntVector_at(x, i)) {
			printf("NG: x[%d]:%d, y[%d]:%d\n", i, *IntVector_at(x, i), i, y[i]);
			assert(0);
		}
	}
	/* sort済みをsort */
	IntVector_sort(x, half_size, IntVector_size(x) - half_size - 1000, int_greater);
	sort(y.begin() + half_size, y.end() - 1000, greater<int>());
	for (i = 0; i < SORT_COUNT; i++) {
		if (y[i] != *IntVector_at(x, i)) {
			printf("NG: x[%d]:%d, y[%d]:%d\n", i, *IntVector_at(x, i), i, y[i]);
			assert(0);
		}
	}

	IntVector_delete(x);
}

void AlgoTest_test_1_7(void)
{
	static int buf[SORT_COUNT];
	IntVector *x;
	vector<int> y;
	int i;
	printf("***** test_1_7 *****\n");
	x = IntVector_new(SORT_COUNT);
	assert(x);

	srand(time(0));
	/* stable_sort */
	/* 部分的にソート */
	for (i = 0; i < SORT_COUNT; i++) {
		buf[i] = rand();
		IntVector_push_back(x, buf[i]);
		y.push_back(buf[i]);
	}
	size_t half_size = IntVector_size(x) / 2;
	IntVector_stable_sort(x, half_size, IntVector_size(x) - half_size - 1000, int_less);
	stable_sort(y.begin() + half_size, y.end() - 1000, less<int>());
	for (i = 0; i < SORT_COUNT; i++) {
		if (y[i] != *IntVector_at(x, i)) {
			printf("NG: x[%d]:%d, y[%d]:%d\n", i, *IntVector_at(x, i), i, y[i]);
			assert(0);
		}
	}
	/* stable_sort済みをstable_sort */
	IntVector_stable_sort(x, half_size, IntVector_size(x) - half_size - 1000, int_less);
	stable_sort(y.begin() + half_size, y.end() - 1000, less<int>());
	for (i = 0; i < SORT_COUNT; i++) {
		if (y[i] != *IntVector_at(x, i)) {
			printf("NG: x[%d]:%d, y[%d]:%d\n", i, *IntVector_at(x, i), i, y[i]);
			assert(0);
		}
	}
	/* 逆順にstable_sort */
	IntVector_stable_sort(x, half_size, IntVector_size(x) - half_size - 1000, int_greater);
	stable_sort(y.begin() + half_size, y.end() - 1000, greater<int>());
	for (i = 0; i < SORT_COUNT; i++) {
		if (y[i] != *IntVector_at(x, i)) {
			printf("NG: x[%d]:%d, y[%d]:%d\n", i, *IntVector_at(x, i), i, y[i]);
			assert(0);
		}
	}
	/* stable_sort済みをstable_sort */
	IntVector_stable_sort(x, half_size, IntVector_size(x) - half_size - 1000, int_greater);
	stable_sort(y.begin() + half_size, y.end() - 1000, greater<int>());
	for (i = 0; i < SORT_COUNT; i++) {
		if (y[i] != *IntVector_at(x, i)) {
			printf("NG: x[%d]:%d, y[%d]:%d\n", i, *IntVector_at(x, i), i, y[i]);
			assert(0);
		}
	}

	IntVector_delete(x);
}

void AlgoTest_test_1_8(void)
{
#ifdef MY_MALLOC
	static int buf[SORT_COUNT];
	IntVector *x;
	vector<int> y;
	int i;
	printf("***** test_1_8 *****\n");
	x = IntVector_new(SORT_COUNT);
	assert(x);

	srand(time(0));
	/* stable_sort */
	/* 部分的にソート */
	for (i = 0; i < SORT_COUNT; i++) {
		buf[i] = rand();
		IntVector_push_back(x, buf[i]);
		y.push_back(buf[i]);
	}
	HEAP_SET_FAIL_COUNT(&heap, 0);
	size_t half_size = IntVector_size(x) / 2;
	IntVector_stable_sort(x, half_size, IntVector_size(x) - half_size - 1000, int_less);
	stable_sort(y.begin() + half_size, y.end() - 1000, less<int>());
	for (i = 0; i < SORT_COUNT; i++) {
		if (y[i] != *IntVector_at(x, i)) {
			printf("NG: x[%d]:%d, y[%d]:%d\n", i, *IntVector_at(x, i), i, y[i]);
			assert(0);
		}
	}
	/* stable_sort済みをstable_sort */
	IntVector_stable_sort(x, half_size, IntVector_size(x) - half_size - 1000, int_less);
	stable_sort(y.begin() + half_size, y.end() - 1000, less<int>());
	for (i = 0; i < SORT_COUNT; i++) {
		if (y[i] != *IntVector_at(x, i)) {
			printf("NG: x[%d]:%d, y[%d]:%d\n", i, *IntVector_at(x, i), i, y[i]);
			assert(0);
		}
	}
	/* 逆順にstable_sort */
	IntVector_stable_sort(x, half_size, IntVector_size(x) - half_size - 1000, int_greater);
	stable_sort(y.begin() + half_size, y.end() - 1000, greater<int>());
	for (i = 0; i < SORT_COUNT; i++) {
		if (y[i] != *IntVector_at(x, i)) {
			printf("NG: x[%d]:%d, y[%d]:%d\n", i, *IntVector_at(x, i), i, y[i]);
			assert(0);
		}
	}
	/* stable_sort済みをstable_sort */
	IntVector_stable_sort(x, half_size, IntVector_size(x) - half_size - 1000, int_greater);
	stable_sort(y.begin() + half_size, y.end() - 1000, greater<int>());
	for (i = 0; i < SORT_COUNT; i++) {
		if (y[i] != *IntVector_at(x, i)) {
			printf("NG: x[%d]:%d, y[%d]:%d\n", i, *IntVector_at(x, i), i, y[i]);
			assert(0);
		}
	}
	HEAP_RESET_FAIL_COUNT(&heap);

	IntVector_delete(x);
#endif
}

void AlgoTest_test_2_1(void)
{
	IntVector *x;
	int i;
	printf("***** test_2_1 *****\n");
	x = IntVector_new(COUNT);
	assert(x);
	for (i = 0; i < COUNT; i++) {
		assert(IntVector_push_back(x, i));
	}
	assert(IntVector_size(x) == COUNT);
	// binary_search
	// idx:0からsize()個
	assert(0 == IntVector_binary_search(x, 0, IntVector_size(x), 0, int_less));
	assert(COUNT - 1 == IntVector_binary_search(x, 0, IntVector_size(x), COUNT - 1, int_less));
	assert(COUNT / 2 == IntVector_binary_search(x, 0, IntVector_size(x), COUNT / 2, int_less));
	assert(COUNT / 3 == IntVector_binary_search(x, 0, IntVector_size(x), COUNT / 3, int_less));
	assert(COUNT == IntVector_binary_search(x, 0, IntVector_size(x), COUNT, int_less));
	assert(COUNT == IntVector_binary_search(x, 0, IntVector_size(x), COUNT + 1, int_less));
	// idx:1からsize() - 1個
	assert(COUNT == IntVector_binary_search(x, 1, IntVector_size(x) - 1, 0, int_less));
	assert(COUNT - 1 == IntVector_binary_search(x, 1, IntVector_size(x) - 1, COUNT - 1, int_less));
	assert(COUNT / 2 == IntVector_binary_search(x, 1, IntVector_size(x) - 1, COUNT / 2, int_less));
	assert(COUNT / 3 == IntVector_binary_search(x, 1, IntVector_size(x) - 1, COUNT / 3, int_less));
	assert(COUNT == IntVector_binary_search(x, 1, IntVector_size(x) - 1, COUNT, int_less));
	assert(COUNT == IntVector_binary_search(x, 1, IntVector_size(x) - 1, COUNT + 1, int_less));

	// lower_bound
	// idx:0からsize()個
	assert(0 == IntVector_lower_bound(x, 0, IntVector_size(x), 0, int_less));
	assert(COUNT - 1 == IntVector_lower_bound(x, 0, IntVector_size(x), COUNT - 1, int_less));
	assert(COUNT / 2 == IntVector_lower_bound(x, 0, IntVector_size(x), COUNT / 2, int_less));
	assert(COUNT / 3 == IntVector_lower_bound(x, 0, IntVector_size(x), COUNT / 3, int_less));
	assert(COUNT == IntVector_lower_bound(x, 0, IntVector_size(x), COUNT, int_less));
	assert(COUNT == IntVector_lower_bound(x, 0, IntVector_size(x), COUNT + 1, int_less));
	// idx:1からsize() - 1個
	assert(1 == IntVector_lower_bound(x, 1, IntVector_size(x) - 1, 0, int_less));
	assert(COUNT - 1 == IntVector_lower_bound(x, 1, IntVector_size(x) - 1, COUNT - 1, int_less));
	assert(COUNT / 2 == IntVector_lower_bound(x, 1, IntVector_size(x) - 1, COUNT / 2, int_less));
	assert(COUNT / 3 == IntVector_lower_bound(x, 1, IntVector_size(x) - 1, COUNT / 3, int_less));
	assert(COUNT == IntVector_lower_bound(x, 1, IntVector_size(x) - 1, COUNT, int_less));
	assert(COUNT == IntVector_lower_bound(x, 1, IntVector_size(x) - 1, COUNT + 1, int_less));

	// upper_bound
	// idx:0からsize()個
	assert(1 == IntVector_upper_bound(x, 0, IntVector_size(x), 0, int_less));
	assert(COUNT - 1 == IntVector_upper_bound(x, 0, IntVector_size(x), COUNT - 2, int_less));
	assert(COUNT == IntVector_upper_bound(x, 0, IntVector_size(x), COUNT - 1, int_less));
	assert(COUNT / 2 + 1 == IntVector_upper_bound(x, 0, IntVector_size(x), COUNT / 2, int_less));
	assert(COUNT / 3 + 1 == IntVector_upper_bound(x, 0, IntVector_size(x), COUNT / 3, int_less));
	assert(COUNT == IntVector_upper_bound(x, 0, IntVector_size(x), COUNT, int_less));
	assert(COUNT == IntVector_upper_bound(x, 0, IntVector_size(x), COUNT + 1, int_less));
	// idx:1からsize() - 1個
	assert(1 == IntVector_upper_bound(x, 1, IntVector_size(x) - 1, 0, int_less));
	assert(COUNT - 1 == IntVector_upper_bound(x, 1, IntVector_size(x) - 1, COUNT - 2, int_less));
	assert(COUNT == IntVector_upper_bound(x, 1, IntVector_size(x) - 1, COUNT - 1, int_less));
	assert(COUNT / 2 + 1 == IntVector_upper_bound(x, 1, IntVector_size(x) - 1, COUNT / 2, int_less));
	assert(COUNT / 3 + 1 == IntVector_upper_bound(x, 1, IntVector_size(x) - 1, COUNT / 3, int_less));
	assert(COUNT == IntVector_upper_bound(x, 1, IntVector_size(x) - 1, COUNT, int_less));
	assert(COUNT == IntVector_upper_bound(x, 1, IntVector_size(x) - 1, COUNT + 1, int_less));


	*IntVector_at(x, COUNT / 2) = *IntVector_at(x, COUNT / 2 + 1);

	// binary_search
	assert(COUNT == IntVector_binary_search(x, 0, IntVector_size(x), COUNT / 2, int_less));
	assert(COUNT / 2 == IntVector_binary_search(x, 0, IntVector_size(x), COUNT / 2 + 1, int_less));
	// lower_bound
	assert(COUNT / 2 == IntVector_lower_bound(x, 0, IntVector_size(x), COUNT / 2, int_less));
	assert(COUNT / 2 == IntVector_lower_bound(x, 0, IntVector_size(x), COUNT / 2 + 1, int_less));
	// upper_bound
	assert(COUNT / 2 == IntVector_upper_bound(x, 0, IntVector_size(x), COUNT / 2, int_less));
	assert(COUNT / 2 + 2 == IntVector_upper_bound(x, 0, IntVector_size(x), COUNT / 2 + 1, int_less));


	IntVector_delete(x);
}

void AlgoTest_test_3_1(void)
{
	IntVector *x;
	int i;
	printf("***** test_3_1 *****\n");
	x = IntVector_new(COUNT);
	assert(x);
	for (i = 0; i < COUNT; i++) {
		assert(IntVector_push_back(x, i));
	}
	assert(IntVector_size(x) == COUNT);
	// reverse
	IntVector_reverse(x, 0, IntVector_size(x));
	for (i = 0; i < COUNT; i++) {
		assert(*IntVector_at(x, i) == COUNT - 1 - i);
	}
	assert(IntVector_size(x) == COUNT);

	// reverse
	IntVector_reverse(x, 0, IntVector_size(x));
	for (i = 0; i < COUNT; i++) {
		assert(*IntVector_at(x, i) == i);
	}
	assert(IntVector_size(x) == COUNT);

	// 途中からreverse
	IntVector_reverse(x, COUNT / 2, IntVector_size(x) - COUNT / 2);
	for (i = 0; i < COUNT; i++) {
//        printf("i[%d], data[%d]\n", i,  *IntVector_at(x, i));
		if (i < COUNT / 2) {
			assert(*IntVector_at(x, i) == i);
		} else {
			assert(*IntVector_at(x, i) == COUNT - 1 - (i - COUNT / 2));
		}
	}
	assert(IntVector_size(x) == COUNT);

	IntVector_delete(x);
}

void AlgoTest_test_3_2(void)
{
	IntVector *x;
	int i;
	printf("***** test_3_2 *****\n");
	x = IntVector_new(COUNT);
	assert(x);
	for (i = 0; i < COUNT; i++) {
		assert(IntVector_push_back(x, i));
	}
	assert(IntVector_size(x) == COUNT);
	// rotate
	IntVector_rotate(x, 0, 0, 0);
	for (i = 0; i < COUNT; i++) {
		assert(*IntVector_at(x, i) == i);
	}
	assert(IntVector_size(x) == COUNT);
	IntVector_rotate(x, COUNT / 2, COUNT / 2, COUNT / 2);
	for (i = 0; i < COUNT; i++) {
		assert(*IntVector_at(x, i) == i);
	}
	assert(IntVector_size(x) == COUNT);
	IntVector_rotate(x, COUNT, COUNT, COUNT);
	for (i = 0; i < COUNT; i++) {
		assert(*IntVector_at(x, i) == i);
	}
	assert(IntVector_size(x) == COUNT);
	IntVector_rotate(x, 0, COUNT, COUNT);
	for (i = 0; i < COUNT; i++) {
		assert(*IntVector_at(x, i) == i);
	}
	assert(IntVector_size(x) == COUNT);
	IntVector_rotate(x, 0, 0, COUNT);
	for (i = 0; i < COUNT; i++) {
		assert(*IntVector_at(x, i) == i);
	}
	assert(IntVector_size(x) == COUNT);

	IntVector_rotate(x, 0, COUNT / 2, COUNT);
	for (i = 0; i < COUNT; i++) {
		if (i < COUNT / 2) {
			assert(*IntVector_at(x, i) == i + COUNT / 2);
		} else {
			assert(*IntVector_at(x, i) == i - COUNT / 2);
		}
	}
	assert(IntVector_size(x) == COUNT);

	IntVector_delete(x);
}

void AlgoTest_test_4_1(void)
{
	IntVector *x;
	IntVector *y;
	IntVector *z;
	int i;
	printf("***** test_4_1 *****\n");
	x = IntVector_new(COUNT);
	y = IntVector_new(COUNT);
	z = IntVector_new(COUNT);
	assert(x);
	assert(y);
	assert(z);
	for (i = 0; i < COUNT / 2; i++) {
		assert(IntVector_push_back(x, i * 2));
	}
	assert(IntVector_size(x) == COUNT / 2);
	for (i = 0; i < COUNT / 2; i++) {
		assert(IntVector_push_back(y, i * 2 + 1));
	}
	assert(IntVector_size(y) == COUNT / 2);

	// merge
	assert(IntVector_merge(z, 0, x, 0, IntVector_size(x), y, 0, IntVector_size(y), int_less));
	assert(IntVector_size(x) == COUNT / 2);
	assert(IntVector_size(y) == COUNT / 2);
	assert(IntVector_size(z) == COUNT);
	for (i = 0; i < COUNT; i++) {
		assert(*IntVector_at(z, i) == i);
	}

	assert(IntVector_merge(z, IntVector_size(z), x, 0, IntVector_size(x), y, 0, IntVector_size(y), int_less));
	assert(IntVector_size(z) == COUNT * 2);
	for (i = 0; i < COUNT; i++) {
		assert(*IntVector_at(z, i) == i);
	}
	for (i = 0; i < COUNT; i++) {
		assert(*IntVector_at(z, i + COUNT) == i);
	}

	for (i = 0; i < COUNT / 2; i++) {
		*IntVector_at(x, i) = *IntVector_at(x, i) * 2;
		*IntVector_at(y, i) = *IntVector_at(y, i) * 2;
	}
	assert(IntVector_merge(z, COUNT, x, 0, IntVector_size(x), y, 0, IntVector_size(y), int_less));
	assert(IntVector_size(z) == COUNT * 3);
	for (i = 0; i < COUNT; i++) {
		assert(*IntVector_at(z, i) == i);
	}
	for (i = 0; i < COUNT; i++) {
//        printf("i[%d], idx[%d], at[%d]\n", i, i + COUNT, *IntVector_at(z, i + COUNT));
		assert(*IntVector_at(z, i + COUNT) == i * 2);
	}
	for (i = 0; i < COUNT; i++) {
		assert(*IntVector_at(z, i + COUNT * 2) == i);
	}

#ifdef MY_MALLOC
	IntVector_shrink(z, 0);
	HEAP_SET_FAIL_COUNT(&heap, 0);
	assert(!IntVector_merge(z, COUNT, x, 0, IntVector_size(x), y, 0, IntVector_size(y), int_less));
	assert(IntVector_size(z) == COUNT * 3);
	for (i = 0; i < COUNT; i++) {
		assert(*IntVector_at(z, i) == i);
	}
	for (i = 0; i < COUNT; i++) {
//        printf("i[%d], idx[%d], at[%d]\n", i, i + COUNT, *IntVector_at(z, i + COUNT));
		assert(*IntVector_at(z, i + COUNT) == i * 2);
	}
	for (i = 0; i < COUNT; i++) {
		assert(*IntVector_at(z, i + COUNT * 2) == i);
	}


	HEAP_RESET_FAIL_COUNT(&heap);
#endif

	IntVector_delete(x);
	IntVector_delete(y);
	IntVector_delete(z);
}

void AlgoTest_test_5_1(void)
{
	IntVector *x;
	int i;
	printf("***** test_5_1 *****\n");
	x = IntVector_new(COUNT);
	assert(x);
	// inplace_merge
	for (i = 0; i < COUNT / 2; i++) {
		assert(IntVector_push_back(x, i * 2));
	}
	for (i = 0; i < COUNT / 2; i++) {
		assert(IntVector_push_back(x, i * 2 + 1));
	}
	assert(IntVector_size(x) == COUNT);

	IntVector_inplace_merge(x, 0, IntVector_size(x) / 2, IntVector_size(x), int_less);
	for (i = 0; i < COUNT; i++) {
		assert(*IntVector_at(x, i) == i);
	}
	assert(IntVector_size(x) == COUNT);

	IntVector_clear(x);
	srand(time(0));
	for (i = 0; i < COUNT; i++) {
		IntVector_push_back(x, rand() % COUNT);
	}
	assert(IntVector_size(x) == COUNT);

	IntVector_sort(x, 0, IntVector_size(x) / 2, int_less);
	IntVector_sort(x, IntVector_size(x) / 2, IntVector_size(x) / 2, int_less);
	int prev;

	prev = 0;
	for (i = 0; i < COUNT / 2; i++) {
//        printf("%d ", *IntVector_at(x, i));
		assert(*IntVector_at(x, i) >= prev);
		prev = *IntVector_at(x, i);
	}
	printf("\n");
	prev = 0;
	for (i = 0; i < COUNT / 2; i++) {
//        printf("%d ", *IntVector_at(x, i + COUNT / 2));
		assert(*IntVector_at(x, i + COUNT / 2) >= prev);
		prev = *IntVector_at(x, i + COUNT / 2);
	}
	printf("\n");
	assert(IntVector_size(x) == COUNT);

	IntVector_inplace_merge(x, 0, IntVector_size(x) / 2, IntVector_size(x), int_less);
	prev = 0;
	for (i = 0; i < COUNT; i++) {
//        printf("%d ", *IntVector_at(x, i));
		assert(*IntVector_at(x, i) >= prev);
		prev = *IntVector_at(x, i);
	}
	printf("\n");
	assert(IntVector_size(x) == COUNT);


	IntVector_delete(x);
}

void AlgoTest_test_5_2(void)
{
#ifdef MY_MALLOC
	IntVector *x;
	int i;
	printf("***** test_5_2 *****\n");
	x = IntVector_new(COUNT);
	assert(x);
	// inplace_merge
	// merge_without_buffer
	for (i = 0; i < COUNT / 2; i++) {
		assert(IntVector_push_back(x, i * 2));
	}
	for (i = 0; i < COUNT / 2; i++) {
		assert(IntVector_push_back(x, i * 2 + 1));
	}
	assert(IntVector_size(x) == COUNT);

	HEAP_SET_FAIL_COUNT(&heap, 0);
	IntVector_inplace_merge(x, 0, IntVector_size(x) / 2, IntVector_size(x), int_less);
	for (i = 0; i < COUNT; i++) {
		assert(*IntVector_at(x, i) == i);
	}
	assert(IntVector_size(x) == COUNT);

	IntVector_clear(x);
	srand(time(0));
	for (i = 0; i < COUNT; i++) {
		IntVector_push_back(x, rand() % COUNT);
	}
	assert(IntVector_size(x) == COUNT);

	IntVector_sort(x, 0, IntVector_size(x) / 2, int_less);
	IntVector_sort(x, IntVector_size(x) / 2, IntVector_size(x) / 2, int_less);
	int prev;

	prev = 0;
	for (i = 0; i < COUNT / 2; i++) {
//        printf("%d ", *IntVector_at(x, i));
		assert(*IntVector_at(x, i) >= prev);
		prev = *IntVector_at(x, i);
	}
	printf("\n");
	prev = 0;
	for (i = 0; i < COUNT / 2; i++) {
//        printf("%d ", *IntVector_at(x, i + COUNT / 2));
		assert(*IntVector_at(x, i + COUNT / 2) >= prev);
		prev = *IntVector_at(x, i + COUNT / 2);
	}
	printf("\n");
	assert(IntVector_size(x) == COUNT);

	IntVector_inplace_merge(x, 0, IntVector_size(x) / 2, IntVector_size(x), int_less);
	prev = 0;
	for (i = 0; i < COUNT; i++) {
//        printf("%d ", *IntVector_at(x, i));
		assert(*IntVector_at(x, i) >= prev);
		prev = *IntVector_at(x, i);
	}
	printf("\n");
	assert(IntVector_size(x) == COUNT);

	HEAP_RESET_FAIL_COUNT(&heap);

	IntVector_delete(x);
#endif
}

void AlgoTest_test_5_3(void)
{
	IntVector *x;
	int i;
	printf("***** test_5_3 *****\n");
	x = IntVector_new(COUNT);
	assert(x);
	// inplace_merge
	for (i = 0; i < COUNT; i++) {
		assert(IntVector_push_back(x, i));
	}
	assert(IntVector_size(x) == COUNT);

	IntVector_inplace_merge(x, 0, IntVector_size(x) / 2, IntVector_size(x), int_less);
	for (i = 0; i < COUNT; i++) {
		assert(*IntVector_at(x, i) == i);
	}
	assert(IntVector_size(x) == COUNT);

	IntVector_inplace_merge(x, 0, IntVector_size(x), IntVector_size(x), int_less);
	for (i = 0; i < COUNT; i++) {
		assert(*IntVector_at(x, i) == i);
	}
	assert(IntVector_size(x) == COUNT);

	IntVector_inplace_merge(x, 0, 0, IntVector_size(x), int_less);
	for (i = 0; i < COUNT; i++) {
		assert(*IntVector_at(x, i) == i);
	}
	assert(IntVector_size(x) == COUNT);

	IntVector_inplace_merge(x, 0, 0, 0, int_less);
	for (i = 0; i < COUNT; i++) {
		assert(*IntVector_at(x, i) == i);
	}
	assert(IntVector_size(x) == COUNT);

	IntVector_inplace_merge(x, IntVector_size(x), IntVector_size(x), IntVector_size(x), int_less);
	for (i = 0; i < COUNT; i++) {
		assert(*IntVector_at(x, i) == i);
	}
	assert(IntVector_size(x) == COUNT);

#ifdef MY_MALLOC
	HEAP_SET_FAIL_COUNT(&heap, 0);
	IntVector_inplace_merge(x, 0, IntVector_size(x) / 2, IntVector_size(x), int_less);
	for (i = 0; i < COUNT; i++) {
		assert(*IntVector_at(x, i) == i);
	}
	assert(IntVector_size(x) == COUNT);

	HEAP_RESET_FAIL_COUNT(&heap);
#endif

	IntVector_delete(x);
}

int IntVector_is_heap(IntVector *x, size_t idx, size_t n, int (*comp)(const void *, const void *))
{
	size_t hi_i;
	for (hi_i = n; hi_i > 1; hi_i--) {
		if (comp(IntVector_at(x, hi_i + idx - 1), IntVector_at(x, hi_i / 2 + idx - 1)) > 0) {
			return 0;
		}
	}
	return 1;
}

void AlgoTest_test_6_1(void)
{
	IntVector *x;
	int i;
	printf("***** test_6_1 *****\n");
	x = IntVector_new(COUNT);
	assert(x);
	for (i = 0; i < COUNT; i++) {
		assert(IntVector_push_back(x, i));
	}
	assert(IntVector_size(x) == COUNT);
	assert(!IntVector_is_heap(x, 0, IntVector_size(x), int_less));
	// make_heap
	IntVector_make_heap(x, 0, IntVector_size(x), int_less);
	assert(IntVector_is_heap(x, 0, IntVector_size(x), int_less));
	// sort_heap
	IntVector_sort_heap(x, 0, IntVector_size(x), int_less);
	for (i = 0; i < COUNT; i++) {
		assert(*IntVector_at(x, i) == i);
	}

	IntVector_sort(x, 0, IntVector_size(x), int_greater);
	assert(!IntVector_is_heap(x, 0, IntVector_size(x), int_greater));
	// make_heap
	IntVector_make_heap(x, 0, IntVector_size(x), int_greater);
	assert(IntVector_is_heap(x, 0, IntVector_size(x), int_greater));
	// sort_heap
	IntVector_sort_heap(x, 0, IntVector_size(x), int_greater);
	for (i = 0; i < COUNT; i++) {
		assert(*IntVector_at(x, i) == COUNT - 1 - i);
	}

	IntVector_sort(x, 0, IntVector_size(x), int_less);

	// 途中から
	size_t half_size = COUNT / 2;
	assert(!IntVector_is_heap(x, half_size, IntVector_size(x) - half_size, int_less));
	// make_heap
	IntVector_make_heap(x, half_size, IntVector_size(x) - half_size, int_less);
	assert(!IntVector_is_heap(x, 0, IntVector_size(x), int_less));
	assert(IntVector_is_heap(x, half_size, IntVector_size(x) - half_size, int_less));
	// sort_heap
	IntVector_sort_heap(x, half_size, IntVector_size(x) - half_size, int_less);
	for (i = 0; i < COUNT; i++) {
		assert(*IntVector_at(x, i) == i);
	}

	// ランダム
	int prev;
	IntVector_clear(x);
	srand(time(0));
	for (i = 0; i < COUNT; i++) {
		IntVector_push_back(x, rand() % COUNT);
	}
	assert(IntVector_size(x) == COUNT);
	assert(!IntVector_is_heap(x, 0, IntVector_size(x), int_less));
	// make_heap
	IntVector_make_heap(x, 0, IntVector_size(x), int_less);
	for (i = 0; i < COUNT; i++) {
//        printf("%d ", *IntVector_at(x, i));
	}
	printf("\n");
	assert(IntVector_is_heap(x, 0, IntVector_size(x), int_less));
	// sort_heap
	IntVector_sort_heap(x, 0, IntVector_size(x), int_less);
	prev = 0;
	for (i = 0; i < COUNT; i++) {
//        printf("%d ", *IntVector_at(x, i));
		assert(*IntVector_at(x, i) >= prev);
		prev = *IntVector_at(x, i);
	}
	printf("\n");

	IntVector_clear(x);
	for (i = 0; i < COUNT; i++) {
		IntVector_push_back(x, rand() % COUNT);
	}
	assert(IntVector_size(x) == COUNT);
	assert(!IntVector_is_heap(x, 0, IntVector_size(x), int_greater));
	// make_heap
	IntVector_make_heap(x, 0, IntVector_size(x), int_greater);
	for (i = 0; i < COUNT; i++) {
//        printf("%d ", *IntVector_at(x, i));
	}
	printf("\n");
	assert(IntVector_is_heap(x, 0, IntVector_size(x), int_greater));
	// sort_heap
	IntVector_sort_heap(x, 0, IntVector_size(x), int_greater);
	prev = COUNT;
	for (i = 0; i < COUNT; i++) {
//        printf("%d ", *IntVector_at(x, i));
		assert(*IntVector_at(x, i) <= prev);
		prev = *IntVector_at(x, i);
	}

	IntVector_delete(x);
}

void AlgoTest_test_6_2(void)
{
	IntVector *x;
	int i;
	printf("***** test_6_2 *****\n");
	x = IntVector_new(COUNT);
	assert(x);
	for (i = 0; i < COUNT; i++) {
		assert(IntVector_push_back(x, i));
	}
	assert(IntVector_size(x) == COUNT);

	// push_heap
	for (i = 1; i <= COUNT; i++) {
		IntVector_push_heap(x, 0, i, int_less);
		assert(IntVector_is_heap(x, 0, i, int_less));
	}
	assert(IntVector_is_heap(x, 0, IntVector_size(x), int_less));
	// pop_heap
	for (i = COUNT; i > 0; i--) {
		IntVector_pop_heap(x, 0, i, int_less);
		assert(IntVector_is_heap(x, 0, i - 1, int_less));
		assert(*IntVector_at(x, i - 1) == i - 1);
//        printf("%d\n", *IntVector_at(x, i - 1));
	}
	assert(!IntVector_is_heap(x, 0, IntVector_size(x), int_less));

	IntVector_sort(x, 0, IntVector_size(x), int_greater);
	assert(!IntVector_is_heap(x, 0, IntVector_size(x), int_greater));
	// push_heap
	for (i = 1; i <= COUNT; i++) {
		IntVector_push_heap(x, 0, i, int_greater);
		assert(IntVector_is_heap(x, 0, i, int_greater));
	}
	assert(IntVector_is_heap(x, 0, IntVector_size(x), int_greater));
	// pop_heap
	for (i = COUNT; i > 0; i--) {
		IntVector_pop_heap(x, 0, i, int_greater);
		assert(IntVector_is_heap(x, 0, i - 1, int_greater));
		assert(*IntVector_at(x, i - 1) == COUNT - i);
//        printf("%d\n", *IntVector_at(x, i - 1));
	}
	assert(!IntVector_is_heap(x, 0, IntVector_size(x), int_greater));

	// ランダム
	int prev;
	IntVector_clear(x);
	srand(time(0));
	for (i = 0; i < COUNT; i++) {
		IntVector_push_back(x, rand() % COUNT);
	}
	assert(IntVector_size(x) == COUNT);
	assert(!IntVector_is_heap(x, 0, IntVector_size(x), int_less));
	// push_heap
	for (i = 1; i <= COUNT; i++) {
		IntVector_push_heap(x, 0, i, int_less);
		assert(IntVector_is_heap(x, 0, i, int_less));
	}
	assert(IntVector_is_heap(x, 0, IntVector_size(x), int_less));
	// pop_heap
	prev = COUNT;
	for (i = COUNT; i > 0; i--) {
		IntVector_pop_heap(x, 0, i, int_less);
		assert(IntVector_is_heap(x, 0, i - 1, int_less));
		assert(*IntVector_at(x, i - 1) <= prev);
		prev = *IntVector_at(x, i - 1);
//        printf("%d\n", *IntVector_at(x, i - 1));
	}
	assert(!IntVector_is_heap(x, 0, IntVector_size(x), int_less));

	IntVector_clear(x);
	srand(time(0));
	for (i = 0; i < COUNT; i++) {
		IntVector_push_back(x, rand() % COUNT);
	}
	assert(IntVector_size(x) == COUNT);
	assert(!IntVector_is_heap(x, 0, IntVector_size(x), int_greater));
	// push_heap
	for (i = 1; i <= COUNT; i++) {
		IntVector_push_heap(x, 0, i, int_greater);
		assert(IntVector_is_heap(x, 0, i, int_greater));
	}
	assert(IntVector_is_heap(x, 0, IntVector_size(x), int_greater));
	// pop_heap
	prev = 0;
	for (i = COUNT; i > 0; i--) {
		IntVector_pop_heap(x, 0, i, int_greater);
		assert(IntVector_is_heap(x, 0, i - 1, int_greater));
		assert(*IntVector_at(x, i - 1) >= prev);
		prev = *IntVector_at(x, i - 1);
//        printf("%d\n", *IntVector_at(x, i - 1));
	}
	assert(!IntVector_is_heap(x, 0, IntVector_size(x), int_greater));

	IntVector_delete(x);
}


void AlgoTest_run(void)
{
	printf("\n===== algorithm test =====\n");
	AlgoTest_test_1_1();
	AlgoTest_test_1_2();
	AlgoTest_test_1_3();
	AlgoTest_test_1_4();
	AlgoTest_test_1_5();
	AlgoTest_test_1_6();
	AlgoTest_test_1_7();
	AlgoTest_test_1_8();
	AlgoTest_test_2_1();
	AlgoTest_test_3_1();
	AlgoTest_test_3_2();
	AlgoTest_test_4_1();
	AlgoTest_test_5_1();
	AlgoTest_test_5_2();
	AlgoTest_test_5_3();
	AlgoTest_test_6_1();
	AlgoTest_test_6_2();
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
