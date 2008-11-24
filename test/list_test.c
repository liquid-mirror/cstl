#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "../cstl/list.h"
#include "list_debug.h"
#include "Pool.h"
#ifdef MY_MALLOC
double buf[1024*1024/sizeof(double)];
Pool pool;
#define malloc(s)		Pool_malloc(&pool, s)
#define realloc(p, s)	Pool_realloc(&pool, p, s)
#define free(p)			Pool_free(&pool, p)
#endif


#include "hoge.h"

/* list */
/*typedef struct Hoge_t {*/
/*    char *key;*/
/*    int value;*/
/*} Hoge;*/

#ifdef CSTLGEN
#include "IntList.h"
#include "HogeList.h"
#else

CSTL_LIST_INTERFACE(IntList, int)
CSTL_LIST_INTERFACE(HogeList, Hoge)
CSTL_LIST_DEBUG_INTERFACE(IntList, int)
CSTL_LIST_DEBUG_INTERFACE(HogeList, Hoge)

CSTL_LIST_IMPLEMENT(IntList, int)
CSTL_LIST_IMPLEMENT(HogeList, Hoge)
CSTL_LIST_DEBUG_IMPLEMENT(IntList, int, %d)
CSTL_LIST_DEBUG_IMPLEMENT(HogeList, Hoge, )
#endif


static HogeList *hl;

#define NELEMS(array)	(sizeof(array) / sizeof(array[0]))

static Hoge hogetab[] = {
	{"000",  0}, 
	{"001",  1}, 
	{"002",  2}, 
	{"003",  3}, 
	{"004",  4}, 
	{"005",  5}, 
	{"006",  6}, 
	{"007",  7}, 
	{"008",  8}, 
	{"009",  9}, 
	{"010", 10}, 
	{"011", 11}, 
	{"012", 12}, 
	{"013", 13}, 
	{"014", 14}, 
	{"015", 15}, 
	{"016", 16}, 
	{"017", 17}, 
	{"018", 18}, 
};



void ListTest_test_1_1(void)
{
	Hoge hoge;
	printf("***** test_1_1 *****\n");
	/* 初期状態 */
	assert(HogeList_size(hl) == 0);
	assert(HogeList_empty(hl));
	assert(HogeList_begin(hl) == HogeList_end(hl));
	assert(HogeList_rbegin(hl) == HogeList_rend(hl));
	/* clear */
	hoge = hogetab[0];
	assert(HogeList_push_back(hl, hoge));
	assert(HogeList_push_back(hl, hoge));
	assert(HogeList_push_back(hl, hoge));
	assert(HogeList_verify(hl));
	HogeList_clear(hl);
	assert(HogeList_verify(hl));
	assert(HogeList_size(hl) == 0);
	assert(HogeList_empty(hl));
}

void ListTest_test_1_2(void)
{
	Hoge hoge;
	int i;
	printf("***** test_1_2 *****\n");
	/* push_back */
	for (i = 0; i < NELEMS(hogetab); i++) {
		assert(HogeList_push_back(hl, hogetab[i]));
		assert(HogeList_verify(hl));
	}
	assert(HogeList_size(hl) == NELEMS(hogetab));
	assert(!HogeList_empty(hl));
	/* pop_front */
	for (i = 0; i < NELEMS(hogetab); i++) {
		/* front */
		hoge = *HogeList_front(hl);
		assert(!strcmp(hoge.key, hogetab[i].key));
		assert(hoge.value == hogetab[i].value);
		/* back */
		hoge = *HogeList_back(hl);
		assert(!strcmp(hoge.key, hogetab[NELEMS(hogetab)-1].key));
		assert(hoge.value == hogetab[NELEMS(hogetab)-1].value);
		/* pop_front */
		HogeList_pop_front(hl);
		assert(HogeList_verify(hl));
	}
	assert(HogeList_size(hl) == 0);
	assert(HogeList_empty(hl));
	/* push_front */
	for (i = 0; i < NELEMS(hogetab); i++) {
		assert(HogeList_push_front(hl, hogetab[i]));
		assert(HogeList_verify(hl));
	}
	assert(HogeList_size(hl) == NELEMS(hogetab));
	assert(!HogeList_empty(hl));
	/* pop_back */
	for (i = 0; i < NELEMS(hogetab); i++) {
		/* front */
		hoge = *HogeList_front(hl);
		assert(!strcmp(hoge.key, hogetab[NELEMS(hogetab)-1].key));
		assert(hoge.value == hogetab[NELEMS(hogetab)-1].value);
		/* back */
		hoge = *HogeList_back(hl);
		assert(!strcmp(hoge.key, hogetab[i].key));
		assert(hoge.value == hogetab[i].value);
		/* pop_back */
		HogeList_pop_back(hl);
		assert(HogeList_verify(hl));
	}
	assert(HogeList_size(hl) == 0);
	assert(HogeList_empty(hl));
}

void ListTest_test_1_3(void)
{
	Hoge hoge;
	int c = 0;
	printf("***** test_1_3 *****\n");
	/* 大量にpush_back */
	hoge = hogetab[0];
	while (c < 1000000 && HogeList_push_back(hl, hoge)) {
		c++;
	}
/*    printf("size: %d, c: %d\n", HogeList_size(hl), c);*/
	assert(HogeList_size(hl) == c);
	assert(!HogeList_empty(hl));
	/* 大量にpush_front */
	HogeList_clear(hl);
	assert(HogeList_size(hl) == 0);
	assert(HogeList_empty(hl));
	c = 0;
	while (c < 1000000 && HogeList_push_front(hl, hoge)) {
		c++;
	}
/*    printf("size: %d, c: %d\n", HogeList_size(hl), c);*/
	assert(HogeList_size(hl) == c);
	assert(!HogeList_empty(hl));
	HogeList_clear(hl);
	assert(HogeList_size(hl) == 0);
	assert(HogeList_empty(hl));
}

void ListTest_test_1_4(void)
{
	Hoge hoge;
	int i;
	HogeListIterator pos;
	printf("***** test_1_4 *****\n");
	/* push_back */
	for (i = 0; i < NELEMS(hogetab); i++) {
		assert(HogeList_push_back(hl, hogetab[i]));
	}
	assert(HogeList_size(hl) == NELEMS(hogetab));
	assert(!HogeList_empty(hl));
	/* at */
	for (i = 0, pos = HogeList_begin(hl); pos != HogeList_end(hl); pos = HogeList_next(pos), i++) {
		hoge = *HogeList_at(pos);
		assert(!strcmp(hoge.key, hogetab[i].key));
		assert(hoge.value == hogetab[i].value);
	}
	assert(i == NELEMS(hogetab));
	for (i = 0, pos = HogeList_begin(hl); pos != HogeList_end(hl); pos = HogeList_next(pos), i++) {
		*HogeList_at(pos) = hogetab[0];
	}
	assert(i == NELEMS(hogetab));
	for (pos = HogeList_begin(hl); pos != HogeList_end(hl); pos = HogeList_next(pos)) {
		hoge = *HogeList_at(pos);
		assert(!strcmp(hoge.key, hogetab[0].key));
		assert(hoge.value == hogetab[0].value);
	}
	while (!HogeList_empty(hl)) {
		HogeList_pop_front(hl);
	}
	assert(HogeList_size(hl) == 0);
}

void ListTest_test_1_5(void)
{
	Hoge hoge;
	int i;
	int flag = 0;
	HogeListIterator pos;
	printf("***** test_1_5 *****\n");
	/* beginの位置にinsert */
	for (i = 0; i < NELEMS(hogetab); i++) {
		assert(HogeList_insert(hl, HogeList_begin(hl), hogetab[i]) == HogeList_begin(hl));
		assert(HogeList_verify(hl));
	}
	HogeList_clear(hl);
	/* endの位置にinsert */
	for (i = 0; i < NELEMS(hogetab); i++) {
		assert(HogeList_insert(hl, HogeList_end(hl), hogetab[i]) == HogeList_rbegin(hl));
		assert(HogeList_verify(hl));
	}
	assert(HogeList_size(hl) == NELEMS(hogetab));
	assert(!HogeList_empty(hl));
	/* 真ん中の位置にinsert */
	for (i = 0, pos = HogeList_begin(hl); pos != HogeList_end(hl); pos = HogeList_next(pos), i++) {
		hoge = *HogeList_at(pos);
		assert(!strcmp(hoge.key, hogetab[i].key));
		assert(hoge.value == hogetab[i].value);
		if (i == NELEMS(hogetab)/2) {
			hoge = hogetab[0];
			assert(HogeList_insert(hl, pos, hoge) == HogeList_prev(pos));
			assert(HogeList_verify(hl));
			break;
		}
	}
	assert(HogeList_size(hl) == NELEMS(hogetab)+1);
	for (i = 0, pos = HogeList_begin(hl); pos != HogeList_end(hl); pos = HogeList_next(pos), i++) {
		hoge = *HogeList_at(pos);
		if (!flag && i == NELEMS(hogetab)/2) {
			assert(!strcmp(hoge.key, hogetab[0].key));
			assert(hoge.value == hogetab[0].value);
			flag = 1;
			i--;
		} else {
			assert(!strcmp(hoge.key, hogetab[i].key));
			assert(hoge.value == hogetab[i].value);
		}
	}
	HogeList_clear(hl);
}

void ListTest_test_1_6(void)
{
	Hoge hoge;
	int i;
	HogeListIterator pos;
	size_t size;
	printf("***** test_1_6 *****\n");
	/* push_back */
	for (i = 0; i < NELEMS(hogetab); i++) {
		assert(HogeList_push_back(hl, hogetab[i]));
	}
	assert(HogeList_size(hl) == NELEMS(hogetab));
	assert(!HogeList_empty(hl));
	/* beginの位置の要素をerase */
	size = HogeList_size(hl);
	for (i = 0; i < NELEMS(hogetab); i++) {
		pos = HogeList_begin(hl);
		assert(HogeList_next(pos) == HogeList_erase(hl, pos));
		assert(HogeList_size(hl) == --size);
		assert(HogeList_verify(hl));
	}
	assert(HogeList_size(hl) == 0);
	assert(HogeList_empty(hl));
	/* push_back */
	for (i = 0; i < NELEMS(hogetab); i++) {
		assert(HogeList_push_back(hl, hogetab[i]));
	}
	assert(HogeList_size(hl) == NELEMS(hogetab));
	assert(!HogeList_empty(hl));
	/* endの前の位置の要素をerase */
	size = HogeList_size(hl);
	for (i = 0; i < NELEMS(hogetab); i++) {
		pos = HogeList_rbegin(hl);
		assert(HogeList_next(pos) == HogeList_erase(hl, pos));
		assert(HogeList_size(hl) == --size);
		assert(HogeList_verify(hl));
	}
	assert(HogeList_size(hl) == 0);
	assert(HogeList_empty(hl));
	/* push_back */
	for (i = 0; i < NELEMS(hogetab); i++) {
		assert(HogeList_push_back(hl, hogetab[i]));
	}
	assert(HogeList_size(hl) == NELEMS(hogetab));
	assert(!HogeList_empty(hl));
	/* 真ん中の位置の要素をerase */
	for (i = 0, pos = HogeList_begin(hl); pos != HogeList_end(hl); i++) {
		if (i == NELEMS(hogetab)/2) {
			pos = HogeList_erase(hl, pos);
		} else {
			pos = HogeList_next(pos);
		}
		assert(HogeList_verify(hl));
	}
	assert(HogeList_size(hl) == NELEMS(hogetab)-1);
	for (i = 0, pos = HogeList_begin(hl); pos != HogeList_end(hl); pos = HogeList_next(pos), i++) {
		hoge = *HogeList_at(pos);
		if (i >= NELEMS(hogetab)/2) {
			assert(!strcmp(hoge.key, hogetab[i+1].key));
			assert(hoge.value == hogetab[i+1].value);
		} else {
			assert(!strcmp(hoge.key, hogetab[i].key));
			assert(hoge.value == hogetab[i].value);
		}
	}
	/* erase_range */
	assert(!HogeList_empty(hl));
	assert(HogeList_end(hl) == HogeList_erase_range(hl, HogeList_begin(hl), HogeList_end(hl)));
	assert(HogeList_empty(hl));
	assert(HogeList_verify(hl));
}

void ListTest_test_1_7(void)
{
	Hoge hoge;
	int i;
	HogeListIterator pos;
	printf("***** test_1_7 *****\n");
	/* push_back */
	for (i = 0; i < NELEMS(hogetab); i++) {
		assert(HogeList_push_back(hl, hogetab[i]));
	}
	assert(HogeList_size(hl) == NELEMS(hogetab));
	assert(!HogeList_empty(hl));
	/* rbegin/rend 逆順に走査 */
	for (i = 0, pos = HogeList_rbegin(hl); pos != HogeList_rend(hl); pos = HogeList_prev(pos), i++) {
		hoge = *HogeList_at(pos);
		assert(!strcmp(hoge.key, hogetab[NELEMS(hogetab)-1 - i].key));
		assert(hoge.value == hogetab[NELEMS(hogetab)-1 - i].value);
	}
	assert(i == NELEMS(hogetab));
	assert(HogeList_next(HogeList_rbegin(hl)) == HogeList_end(hl));
	assert(HogeList_prev(HogeList_begin(hl)) == HogeList_rend(hl));
	HogeList_clear(hl);
}

static Hoge hogetab2[] = {
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
int less(const void *p1, const void *p2)
{
	return strcmp(((Hoge*)p1)->key, ((Hoge*)p2)->key);
}
int greater(const void *p1, const void *p2)
{
	return strcmp(((Hoge*)p2)->key, ((Hoge*)p1)->key);
}
void ListTest_test_1_8(void)
{
	Hoge hoge;
	Hoge prev;
	int i;
	HogeListIterator pos;
	HogeList *x;
	printf("***** test_1_8 *****\n");

	printf("before sort\n");
	/* push_back */
	for (i = 0; i < NELEMS(hogetab2); i++) {
		assert(HogeList_push_back(hl, hogetab2[i]));
	}
	pos = HogeList_end(hl);
	hoge.key = "XXX"; hoge.value = 9999;
	printf("%s, %4d,	p[%p]\n", hoge.key, hoge.value, pos);
	for (pos = HogeList_begin(hl); pos != HogeList_end(hl); pos = HogeList_next(pos)) {
		hoge = *HogeList_at(pos);
		printf("%s, %4d,	p[%p], prev[%p], next[%p]\n", hoge.key, hoge.value, pos, HogeList_prev(pos), HogeList_next(pos));
	}
	/* sort less*/
	printf("size[%d]\n", HogeList_size(hl));
	HogeList_sort(hl, less);
	assert(HogeList_verify(hl));
	printf("size[%d]\n", HogeList_size(hl));
	printf("after sort (less)\n");
	pos = HogeList_end(hl);
	hoge.key = "XXX"; hoge.value = 9999;
	prev.key = "000"; prev.value = 0;
	printf("%s, %4d,	p[%p]\n", hoge.key, hoge.value, pos);
	for (pos = HogeList_begin(hl); pos != HogeList_end(hl); pos = HogeList_next(pos)) {
		hoge = *HogeList_at(pos);
		printf("%s, %4d,	p[%p], prev[%p], next[%p]\n", hoge.key, hoge.value, pos, HogeList_prev(pos), HogeList_next(pos));
		assert(strcmp(prev.key, hoge.key) <= 0);
		assert(prev.value <= hoge.value);
		prev = hoge;
	}
	/* sort済みをsort */
	printf("size[%d]\n", HogeList_size(hl));
	HogeList_sort(hl, less);
	assert(HogeList_verify(hl));
	printf("size[%d]\n", HogeList_size(hl));
	printf("after sort (less)\n");
	pos = HogeList_end(hl);
	hoge.key = "XXX"; hoge.value = 9999;
	prev.key = "000"; prev.value = 0;
	printf("%s, %4d,	p[%p]\n", hoge.key, hoge.value, pos);
	for (pos = HogeList_begin(hl); pos != HogeList_end(hl); pos = HogeList_next(pos)) {
		hoge = *HogeList_at(pos);
		printf("%s, %4d,	p[%p], prev[%p], next[%p]\n", hoge.key, hoge.value, pos, HogeList_prev(pos), HogeList_next(pos));
		assert(strcmp(prev.key, hoge.key) <= 0);
		assert(prev.value <= hoge.value);
		prev = hoge;
	}
	/* sort greater*/
	printf("size[%d]\n", HogeList_size(hl));
	HogeList_sort(hl, greater);
	assert(HogeList_verify(hl));
	printf("size[%d]\n", HogeList_size(hl));
	printf("after sort (greater)\n");
	pos = HogeList_end(hl);
	hoge.key = "XXX"; hoge.value = 9999;
	prev.key = "999"; prev.value = 999;
	printf("%s, %4d,	p[%p]\n", hoge.key, hoge.value, pos);
	for (pos = HogeList_begin(hl); pos != HogeList_end(hl); pos = HogeList_next(pos)) {
		hoge = *HogeList_at(pos);
		printf("%s, %4d,	p[%p], prev[%p], next[%p]\n", hoge.key, hoge.value, pos, HogeList_prev(pos), HogeList_next(pos));
		assert(strcmp(prev.key, hoge.key) >= 0);
		if (strcmp(prev.key, hoge.key) == 0) {
			assert(prev.value <= hoge.value);
		} else {
			assert(prev.value >= hoge.value);
		}
		prev = hoge;
	}
	/* sort済みをsort */
	printf("size[%d]\n", HogeList_size(hl));
	HogeList_sort(hl, greater);
	assert(HogeList_verify(hl));
	printf("size[%d]\n", HogeList_size(hl));
	printf("after sort (greater)\n");
	pos = HogeList_end(hl);
	hoge.key = "XXX"; hoge.value = 9999;
	prev.key = "999"; prev.value = 999;
	printf("%s, %4d,	p[%p]\n", hoge.key, hoge.value, pos);
	for (pos = HogeList_begin(hl); pos != HogeList_end(hl); pos = HogeList_next(pos)) {
		hoge = *HogeList_at(pos);
		printf("%s, %4d,	p[%p], prev[%p], next[%p]\n", hoge.key, hoge.value, pos, HogeList_prev(pos), HogeList_next(pos));
		assert(strcmp(prev.key, hoge.key) >= 0);
		if (strcmp(prev.key, hoge.key) == 0) {
			assert(prev.value <= hoge.value);
		} else {
			assert(prev.value >= hoge.value);
		}
		prev = hoge;
	}
	/* reverse */
	HogeList_reverse(hl);
	assert(HogeList_verify(hl));
	printf("size[%d]\n", HogeList_size(hl));
	printf("reverse\n");
	pos = HogeList_end(hl);
	hoge.key = "XXX"; hoge.value = 9999;
	printf("%s, %4d,	p[%p]\n", hoge.key, hoge.value, pos);
	for (pos = HogeList_begin(hl); pos != HogeList_end(hl); pos = HogeList_next(pos)) {
		hoge = *HogeList_at(pos);
		printf("%s, %4d,	p[%p], prev[%p], next[%p]\n", hoge.key, hoge.value, pos, HogeList_prev(pos), HogeList_next(pos));
	}
	/* merge */
	printf("\n");
	printf("before merge 1\n");
	HogeList_clear(hl);
	for (i = 0; i < NELEMS(hogetab2); i += 2) {
		assert(HogeList_push_back(hl, hogetab2[i]));
	}
	HogeList_sort(hl, less);
	pos = HogeList_end(hl);
	hoge.key = "XXX"; hoge.value = 9999;
	printf("%s, %4d,	p[%p]\n", hoge.key, hoge.value, pos);
	for (pos = HogeList_begin(hl); pos != HogeList_end(hl); pos = HogeList_next(pos)) {
		hoge = *HogeList_at(pos);
		printf("%s, %4d,	p[%p], prev[%p], next[%p]\n", hoge.key, hoge.value, pos, HogeList_prev(pos), HogeList_next(pos));
	}

	printf("before merge 2\n");
	x = HogeList_new();
	for (i = 1; i < NELEMS(hogetab2); i += 2) {
		assert(HogeList_push_back(x, hogetab2[i]));
	}
	HogeList_sort(x, less);
	pos = HogeList_end(x);
	hoge.key = "XXX"; hoge.value = 9999;
	printf("%s, %4d,	p[%p]\n", hoge.key, hoge.value, pos);
	for (pos = HogeList_begin(x); pos != HogeList_end(x); pos = HogeList_next(pos)) {
		hoge = *HogeList_at(pos);
		printf("%s, %4d,	p[%p], prev[%p], next[%p]\n", hoge.key, hoge.value, pos, HogeList_prev(pos), HogeList_next(pos));
	}

	printf("after merge\n");
	HogeList_merge(hl, x, less);
	assert(HogeList_verify(hl));
	assert(HogeList_verify(x));
	pos = HogeList_end(hl);
	hoge.key = "XXX"; hoge.value = 9999;
	printf("%s, %4d,	p[%p]\n", hoge.key, hoge.value, pos);
	for (pos = HogeList_begin(hl); pos != HogeList_end(hl); pos = HogeList_next(pos)) {
		hoge = *HogeList_at(pos);
		printf("%s, %4d,	p[%p], prev[%p], next[%p]\n", hoge.key, hoge.value, pos, HogeList_prev(pos), HogeList_next(pos));
	}
	assert(HogeList_empty(x));
	assert(HogeList_size(hl) == NELEMS(hogetab2));


	HogeList_delete(x);
	HogeList_clear(hl);
}

void ListTest_test_2_1(void)
{
	IntList *il = IntList_new();
	IntList *x;
	IntListIterator pos;
	int buf[32];
	int i;
	int b[] = {1, 2, 4, 43, 2, 54, 1, 0, 2, 24};
	printf("***** test_2_1 *****\n");
	for (i = 0; i < 32; i++) buf[i] = i;
	/* insert_array */
	assert(IntList_size(il) == 0);
	IntList_clear(il);
	assert(IntList_insert_array(il, IntList_begin(il), buf, 32));
	assert(IntList_verify(il));
	assert(IntList_size(il) == 32);
	for (pos = IntList_begin(il), i = 0; pos != IntList_end(il); pos = IntList_next(pos), i++) {
		assert(*IntList_at(pos) == i);
	}
	/* resize */
	assert(IntList_resize(il, 64, 100));
	assert(IntList_verify(il));
	assert(IntList_size(il) == 64);
	for (pos = IntList_begin(il), i = 0; pos != IntList_end(il); pos = IntList_next(pos), i++) {
		if (i < 32) {
			assert(*IntList_at(pos) == i);
		} else {
			assert(*IntList_at(pos) == 100);
		}
	}
	assert(IntList_resize(il, 16, 99));
	assert(IntList_verify(il));
	assert(IntList_size(il) == 16);
	for (pos = IntList_begin(il), i = 0; pos != IntList_end(il); pos = IntList_next(pos), i++) {
		assert(*IntList_at(pos) == i);
	}
	assert(IntList_resize(il, 0, 99));
	assert(IntList_verify(il));
	assert(IntList_size(il) == 0);
	assert(IntList_resize(il, 100, 99));
	/* swap */
	x = IntList_new();
	IntList_clear(il);
	assert(IntList_insert_array(il, IntList_begin(il), buf, 32));
	assert(IntList_insert_array(x, IntList_begin(x), b, sizeof b / sizeof b[0]));
	assert(IntList_size(il) == 32);
	for (pos = IntList_begin(il), i = 0; pos != IntList_end(il); pos = IntList_next(pos), i++) {
		assert(*IntList_at(pos) == i);
	}
	assert(IntList_size(x) == sizeof b / sizeof b[0]);
	for (pos = IntList_begin(x), i = 0; pos != IntList_end(x); pos = IntList_next(pos), i++) {
		assert(*IntList_at(pos) == b[i]);
	}
	IntList_swap(il, x);
	assert(IntList_verify(il));
	assert(IntList_verify(x));
	assert(IntList_size(x) == 32);
	printf("x:\n");
	for (pos = IntList_begin(x), i = 0; pos != IntList_end(x); pos = IntList_next(pos), i++) {
		assert(*IntList_at(pos) == i);
		printf("%2d, ", *IntList_at(pos));
	}
	printf("\n");
	printf("il:\n");
	assert(IntList_size(il) == sizeof b / sizeof b[0]);
	for (pos = IntList_begin(il), i = 0; pos != IntList_end(il); pos = IntList_next(pos), i++) {
		assert(*IntList_at(pos) == b[i]);
		printf("%2d, ", *IntList_at(pos));
	}
	printf("\n");
	/* splice */
	IntList_splice(x, IntList_begin(x), il, IntList_begin(il), IntList_end(il));
/*    IntList_splice(x, IntList_begin(x), il, IntList_begin(il), IntList_begin(il));*/
/*    IntList_splice(x, IntList_begin(x), il, IntList_begin(il), IntList_next(IntList_begin(il)));*/
/*    IntList_splice(x, IntList_begin(x), x, IntList_next(IntList_begin(x)), IntList_end(x));*/
/*    IntList_splice(x, IntList_end(x), x, IntList_next(IntList_begin(x)), IntList_end(x));*/
/*    IntList_splice(x, IntList_rbegin(x), x, IntList_next(IntList_begin(x)), IntList_prev(IntList_rbegin(x)));*/
	assert(IntList_verify(il));
	assert(IntList_verify(x));

	assert(IntList_size(il) == 0);
	assert(IntList_size(x) == 32 + sizeof b / sizeof b[0]);
	printf("x:\n");
	for (pos = IntList_begin(x), i = 0; pos != IntList_end(x); pos = IntList_next(pos), i++) {
		printf("%2d, ", *IntList_at(pos));
	}
	printf("\n");
	printf("il:\n");
	for (pos = IntList_begin(il), i = 0; pos != IntList_end(il); pos = IntList_next(pos), i++) {
		printf("%2d, ", *IntList_at(pos));
	}
	/* insert_range */
	IntList_clear(il);
	IntList_clear(x);
	assert(IntList_insert_array(il, IntList_begin(il), buf, 32));
	assert(IntList_insert_array(x, IntList_begin(x), b, sizeof b / sizeof b[0]));
	assert(IntList_insert_range(il, IntList_begin(il), IntList_begin(x), IntList_end(x)));
	printf("insert_range, il:\n");
	for (pos = IntList_begin(il), i = 0; pos != IntList_end(il); pos = IntList_next(pos), i++) {
		printf("%2d, ", *IntList_at(pos));
	}
	assert(IntList_verify(il));
	assert(IntList_verify(x));
	printf("\n");

	POOL_DUMP_OVERFLOW(&pool);
	IntList_delete(il);
	IntList_delete(x);
}



void ListTest_run(void)
{
	printf("\n===== list test =====\n");
	hl = HogeList_new();
	assert(hl);
	assert(HogeList_verify(hl));

	ListTest_test_1_1();
	ListTest_test_1_2();
	ListTest_test_1_3();
	ListTest_test_1_4();
	ListTest_test_1_5();
	ListTest_test_1_6();
	ListTest_test_1_7();
	ListTest_test_1_8();
	ListTest_test_2_1();

	POOL_DUMP_OVERFLOW(&pool);
	HogeList_delete(hl);
}



int main(void)
{
#ifdef MY_MALLOC
	Pool_init(&pool, buf, sizeof buf, sizeof buf[0]);
#endif
	ListTest_run();
#ifdef MY_MALLOC
	POOL_DUMP_LEAK(&pool, 0);
#endif
	return 0;
}
