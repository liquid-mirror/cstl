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
CSTL_LIST_DEBUG_IMPLEMENT(IntList, int,)
CSTL_LIST_DEBUG_IMPLEMENT(HogeList, Hoge,)
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
	assert(cstl_size(hl) == 0);
	assert(cstl_empty(hl));
	assert(cstl_iter_eq(cstl_begin(hl), cstl_end(hl)));
	assert(cstl_iter_eq(cstl_rbegin(hl), cstl_rend(hl)));
	/* clear */
	hoge = hogetab[0];
	assert(cstl_push_back(hl, hoge));
	assert(cstl_push_back(hl, hoge));
	assert(cstl_push_back(hl, hoge));
	assert(HogeList_verify(hl));
	cstl_clear(hl);
	assert(HogeList_verify(hl));
	assert(cstl_size(hl) == 0);
	assert(cstl_empty(hl));
}

void ListTest_test_1_2(void)
{
	Hoge hoge;
	int i;
	printf("***** test_1_2 *****\n");
	/* push_back */
	for (i = 0; i < NELEMS(hogetab); i++) {
		assert(cstl_push_back(hl, hogetab[i]));
		assert(HogeList_verify(hl));
	}
	assert(cstl_size(hl) == NELEMS(hogetab));
	assert(!cstl_empty(hl));
	/* pop_front */
	for (i = 0; i < NELEMS(hogetab); i++) {
		/* front */
		hoge = *cstl_front(hl);
		assert(!strcmp(hoge.key, hogetab[i].key));
		assert(hoge.value == hogetab[i].value);
		/* back */
		hoge = *cstl_back(hl);
		assert(!strcmp(hoge.key, hogetab[NELEMS(hogetab)-1].key));
		assert(hoge.value == hogetab[NELEMS(hogetab)-1].value);
		/* pop_front */
		cstl_pop_front(hl);
		assert(HogeList_verify(hl));
	}
	assert(cstl_size(hl) == 0);
	assert(cstl_empty(hl));
	/* push_front */
	for (i = 0; i < NELEMS(hogetab); i++) {
		assert(cstl_push_front(hl, hogetab[i]));
		assert(HogeList_verify(hl));
	}
	assert(cstl_size(hl) == NELEMS(hogetab));
	assert(!cstl_empty(hl));
	/* pop_back */
	for (i = 0; i < NELEMS(hogetab); i++) {
		/* front */
		hoge = *cstl_front(hl);
		assert(!strcmp(hoge.key, hogetab[NELEMS(hogetab)-1].key));
		assert(hoge.value == hogetab[NELEMS(hogetab)-1].value);
		/* back */
		hoge = *cstl_back(hl);
		assert(!strcmp(hoge.key, hogetab[i].key));
		assert(hoge.value == hogetab[i].value);
		/* pop_back */
		cstl_pop_back(hl);
		assert(HogeList_verify(hl));
	}
	assert(cstl_size(hl) == 0);
	assert(cstl_empty(hl));
}

void ListTest_test_1_3(void)
{
	Hoge hoge;
	int c = 0;
	printf("***** test_1_3 *****\n");
	/* 大量にpush_back */
	hoge = hogetab[0];
	while (c < 1000000 && cstl_push_back(hl, hoge)) {
		c++;
	}
/*    printf("size: %d, c: %d\n", cstl_size(hl), c);*/
	assert(cstl_size(hl) == c);
	assert(!cstl_empty(hl));
	/* 大量にpush_front */
	cstl_clear(hl);
	assert(cstl_size(hl) == 0);
	assert(cstl_empty(hl));
	c = 0;
	while (c < 1000000 && cstl_push_front(hl, hoge)) {
		c++;
	}
/*    printf("size: %d, c: %d\n", cstl_size(hl), c);*/
	assert(cstl_size(hl) == c);
	assert(!cstl_empty(hl));
	cstl_clear(hl);
	assert(cstl_size(hl) == 0);
	assert(cstl_empty(hl));
}

void ListTest_test_1_4(void)
{
	Hoge hoge;
	int i;
	HogeListIterator pos;
	printf("***** test_1_4 *****\n");
	/* push_back */
	for (i = 0; i < NELEMS(hogetab); i++) {
		assert(cstl_push_back(hl, hogetab[i]));
	}
	assert(cstl_size(hl) == NELEMS(hogetab));
	assert(!cstl_empty(hl));
	/* data */
	for (i = 0, pos = cstl_begin(hl); cstl_iter_ne(pos, cstl_end(hl)); cstl_iter_incr(&pos), i++) {
		hoge = *cstl_iter_data(pos);
		assert(!strcmp(hoge.key, hogetab[i].key));
		assert(hoge.value == hogetab[i].value);
	}
	assert(i == NELEMS(hogetab));
	for (i = 0, pos = cstl_begin(hl); cstl_iter_ne(pos, cstl_end(hl)); cstl_iter_incr(&pos), i++) {
		*cstl_iter_data(pos) = hogetab[0];
	}
	assert(i == NELEMS(hogetab));
	for (pos = cstl_begin(hl); cstl_iter_ne(pos, cstl_end(hl)); cstl_iter_incr(&pos)) {
		hoge = *cstl_iter_data(pos);
		assert(!strcmp(hoge.key, hogetab[0].key));
		assert(hoge.value == hogetab[0].value);
	}
	while (!cstl_empty(hl)) {
		cstl_pop_front(hl);
	}
	assert(cstl_size(hl) == 0);
}

void ListTest_test_1_5(void)
{
	Hoge hoge;
	int i;
	int flag = 0;
	HogeListIterator pos;
	HogeListIterator iter;
	printf("***** test_1_5 *****\n");
	/* beginの位置にinsert */
	for (i = 0; i < NELEMS(hogetab); i++) {
		assert(cstl_insert(hl, cstl_begin(hl), hogetab[i], &iter));
		assert(cstl_iter_eq(iter, cstl_begin(hl)));
		assert(HogeList_verify(hl));
	}
	cstl_clear(hl);
	/* endの位置にinsert */
	for (i = 0; i < NELEMS(hogetab); i++) {
		assert(cstl_insert(hl, cstl_end(hl), hogetab[i], &iter));
		cstl_iter_incr(&iter);
		assert(cstl_iter_eq(iter, cstl_end(hl)));
		assert(HogeList_verify(hl));
	}
	assert(cstl_size(hl) == NELEMS(hogetab));
	assert(!cstl_empty(hl));
	/* 真ん中の位置にinsert */
	for (i = 0, pos = cstl_begin(hl); cstl_iter_ne(pos, cstl_end(hl)); cstl_iter_incr(&pos), i++) {
		hoge = *cstl_iter_data(pos);
		assert(!strcmp(hoge.key, hogetab[i].key));
		assert(hoge.value == hogetab[i].value);
		if (i == NELEMS(hogetab)/2) {
			hoge = hogetab[0];
			assert(cstl_insert(hl, pos, hoge, &iter));
			assert(cstl_iter_eq(iter, cstl_iter_prev(pos)));
			assert(HogeList_verify(hl));
			break;
		}
	}
	assert(cstl_size(hl) == NELEMS(hogetab)+1);
	for (i = 0, pos = cstl_begin(hl); cstl_iter_ne(pos, cstl_end(hl)); cstl_iter_incr(&pos), i++) {
		hoge = *cstl_iter_data(pos);
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
	cstl_clear(hl);
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
		assert(cstl_push_back(hl, hogetab[i]));
	}
	assert(cstl_size(hl) == NELEMS(hogetab));
	assert(!cstl_empty(hl));
	/* beginの位置の要素をerase */
	size = cstl_size(hl);
	for (i = 0; i < NELEMS(hogetab); i++) {
		pos = cstl_begin(hl);
		assert(cstl_iter_eq(cstl_iter_next(pos), cstl_erase(hl, pos)));
		assert(cstl_size(hl) == --size);
		assert(HogeList_verify(hl));
	}
	assert(cstl_size(hl) == 0);
	assert(cstl_empty(hl));
	/* push_back */
	for (i = 0; i < NELEMS(hogetab); i++) {
		assert(cstl_push_back(hl, hogetab[i]));
	}
	assert(cstl_size(hl) == NELEMS(hogetab));
	assert(!cstl_empty(hl));
	/* endの前の位置の要素をerase */
	size = cstl_size(hl);
	for (i = 0; i < NELEMS(hogetab); i++) {
		HogeListReverseIterator iter = cstl_rbegin(hl);
		cstl_iter_incr(&iter);
		pos = cstl_riter_base(iter);
/*        pos = cstl_rbegin(hl);*/
		assert(cstl_iter_eq(cstl_iter_next(pos), cstl_erase(hl, pos)));
		assert(cstl_size(hl) == --size);
		assert(HogeList_verify(hl));
	}
	assert(cstl_size(hl) == 0);
	assert(cstl_empty(hl));
	/* push_back */
	for (i = 0; i < NELEMS(hogetab); i++) {
		assert(cstl_push_back(hl, hogetab[i]));
	}
	assert(cstl_size(hl) == NELEMS(hogetab));
	assert(!cstl_empty(hl));
	/* 真ん中の位置の要素をerase */
	for (i = 0, pos = cstl_begin(hl); cstl_iter_ne(pos, cstl_end(hl)); i++) {
		if (i == NELEMS(hogetab)/2) {
			pos = cstl_erase(hl, pos);
		} else {
			cstl_iter_incr(&pos);
		}
		assert(HogeList_verify(hl));
	}
	assert(cstl_size(hl) == NELEMS(hogetab)-1);
	for (i = 0, pos = cstl_begin(hl); cstl_iter_ne(pos, cstl_end(hl)); cstl_iter_incr(&pos), i++) {
		hoge = *cstl_iter_data(pos);
		if (i >= NELEMS(hogetab)/2) {
			assert(!strcmp(hoge.key, hogetab[i+1].key));
			assert(hoge.value == hogetab[i+1].value);
		} else {
			assert(!strcmp(hoge.key, hogetab[i].key));
			assert(hoge.value == hogetab[i].value);
		}
	}
	/* erase_range */
	assert(!cstl_empty(hl));
	assert(cstl_iter_eq(cstl_end(hl), cstl_erase_range(hl, cstl_begin(hl), cstl_end(hl))));
	assert(cstl_empty(hl));
	assert(HogeList_verify(hl));
}

void ListTest_test_1_7(void)
{
	Hoge hoge;
	int i;
	HogeListReverseIterator pos;
	printf("***** test_1_7 *****\n");
	/* push_back */
	for (i = 0; i < NELEMS(hogetab); i++) {
		assert(cstl_push_back(hl, hogetab[i]));
	}
	assert(cstl_size(hl) == NELEMS(hogetab));
	assert(!cstl_empty(hl));
	/* rbegin/rend 逆順に走査 */
	for (i = 0, pos = cstl_rbegin(hl); cstl_iter_ne(pos, cstl_rend(hl)); cstl_iter_incr(&pos), i++) {
		hoge = *cstl_iter_data(pos);
		assert(!strcmp(hoge.key, hogetab[NELEMS(hogetab)-1 - i].key));
		assert(hoge.value == hogetab[NELEMS(hogetab)-1 - i].value);
	}
	assert(i == NELEMS(hogetab));
	/* TODO */
/*    assert(HogeList_next(cstl_rbegin(hl)) == cstl_end(hl));*/
/*    assert(HogeList_prev(cstl_begin(hl)) == cstl_rend(hl));*/
	cstl_clear(hl);
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
		assert(cstl_push_back(hl, hogetab2[i]));
	}
	pos = cstl_end(hl);
	hoge.key = "XXX"; hoge.value = 9999;
	printf("%s, %4d,	p[%p]\n", hoge.key, hoge.value, pos);
	for (pos = cstl_begin(hl); cstl_iter_ne(pos, cstl_end(hl)); cstl_iter_incr(&pos)) {
		hoge = *cstl_iter_data(pos);
/*        printf("%s, %4d,	p[%p], prev[%p], next[%p]\n", hoge.key, hoge.value, pos, HogeList_prev(pos), HogeList_next(pos));*/
	}
	/* sort less*/
	printf("size[%d]\n", cstl_size(hl));
	cstl_sort(hl, less);
	assert(HogeList_verify(hl));
	printf("size[%d]\n", cstl_size(hl));
	printf("after sort (less)\n");
	pos = cstl_end(hl);
	hoge.key = "XXX"; hoge.value = 9999;
	prev.key = "000"; prev.value = 0;
	printf("%s, %4d,	p[%p]\n", hoge.key, hoge.value, pos);
	for (pos = cstl_begin(hl); cstl_iter_ne(pos, cstl_end(hl)); cstl_iter_incr(&pos)) {
		hoge = *cstl_iter_data(pos);
/*        printf("%s, %4d,	p[%p], prev[%p], next[%p]\n", hoge.key, hoge.value, pos, HogeList_prev(pos), HogeList_next(pos));*/
		assert(strcmp(prev.key, hoge.key) <= 0);
		assert(prev.value <= hoge.value);
		prev = hoge;
	}
	/* sort済みをsort */
	printf("size[%d]\n", cstl_size(hl));
	cstl_sort(hl, less);
	assert(HogeList_verify(hl));
	printf("size[%d]\n", cstl_size(hl));
	printf("after sort (less)\n");
	pos = cstl_end(hl);
	hoge.key = "XXX"; hoge.value = 9999;
	prev.key = "000"; prev.value = 0;
	printf("%s, %4d,	p[%p]\n", hoge.key, hoge.value, pos);
	for (pos = cstl_begin(hl); cstl_iter_ne(pos, cstl_end(hl)); cstl_iter_incr(&pos)) {
		hoge = *cstl_iter_data(pos);
/*        printf("%s, %4d,	p[%p], prev[%p], next[%p]\n", hoge.key, hoge.value, pos, HogeList_prev(pos), HogeList_next(pos));*/
		assert(strcmp(prev.key, hoge.key) <= 0);
		assert(prev.value <= hoge.value);
		prev = hoge;
	}
	/* sort greater*/
	printf("size[%d]\n", cstl_size(hl));
	cstl_sort(hl, greater);
	assert(HogeList_verify(hl));
	printf("size[%d]\n", cstl_size(hl));
	printf("after sort (greater)\n");
	pos = cstl_end(hl);
	hoge.key = "XXX"; hoge.value = 9999;
	prev.key = "999"; prev.value = 999;
	printf("%s, %4d,	p[%p]\n", hoge.key, hoge.value, pos);
	for (pos = cstl_begin(hl); cstl_iter_ne(pos, cstl_end(hl)); cstl_iter_incr(&pos)) {
		hoge = *cstl_iter_data(pos);
/*        printf("%s, %4d,	p[%p], prev[%p], next[%p]\n", hoge.key, hoge.value, pos, HogeList_prev(pos), HogeList_next(pos));*/
		assert(strcmp(prev.key, hoge.key) >= 0);
		if (strcmp(prev.key, hoge.key) == 0) {
			assert(prev.value <= hoge.value);
		} else {
			assert(prev.value >= hoge.value);
		}
		prev = hoge;
	}
	/* sort済みをsort */
	printf("size[%d]\n", cstl_size(hl));
	cstl_sort(hl, greater);
	assert(HogeList_verify(hl));
	printf("size[%d]\n", cstl_size(hl));
	printf("after sort (greater)\n");
	pos = cstl_end(hl);
	hoge.key = "XXX"; hoge.value = 9999;
	prev.key = "999"; prev.value = 999;
	printf("%s, %4d,	p[%p]\n", hoge.key, hoge.value, pos);
	for (pos = cstl_begin(hl); cstl_iter_ne(pos, cstl_end(hl)); cstl_iter_incr(&pos)) {
		hoge = *cstl_iter_data(pos);
/*        printf("%s, %4d,	p[%p], prev[%p], next[%p]\n", hoge.key, hoge.value, pos, HogeList_prev(pos), HogeList_next(pos));*/
		assert(strcmp(prev.key, hoge.key) >= 0);
		if (strcmp(prev.key, hoge.key) == 0) {
			assert(prev.value <= hoge.value);
		} else {
			assert(prev.value >= hoge.value);
		}
		prev = hoge;
	}
	/* reverse */
	cstl_reverse(hl);
	assert(HogeList_verify(hl));
	printf("size[%d]\n", cstl_size(hl));
	printf("reverse\n");
	pos = cstl_end(hl);
	hoge.key = "XXX"; hoge.value = 9999;
	printf("%s, %4d,	p[%p]\n", hoge.key, hoge.value, pos);
	for (pos = cstl_begin(hl); cstl_iter_ne(pos, cstl_end(hl)); cstl_iter_incr(&pos)) {
		hoge = *cstl_iter_data(pos);
/*        printf("%s, %4d,	p[%p], prev[%p], next[%p]\n", hoge.key, hoge.value, pos, HogeList_prev(pos), HogeList_next(pos));*/
	}
	/* merge */
	printf("\n");
	printf("before merge 1\n");
	cstl_clear(hl);
	for (i = 0; i < NELEMS(hogetab2); i += 2) {
		assert(cstl_push_back(hl, hogetab2[i]));
	}
	cstl_sort(hl, less);
	pos = cstl_end(hl);
	hoge.key = "XXX"; hoge.value = 9999;
	printf("%s, %4d,	p[%p]\n", hoge.key, hoge.value, pos);
	for (pos = cstl_begin(hl); cstl_iter_ne(pos, cstl_end(hl)); cstl_iter_incr(&pos)) {
		hoge = *cstl_iter_data(pos);
/*        printf("%s, %4d,	p[%p], prev[%p], next[%p]\n", hoge.key, hoge.value, pos, HogeList_prev(pos), HogeList_next(pos));*/
	}

	printf("before merge 2\n");
	x = HogeList_new();
	for (i = 1; i < NELEMS(hogetab2); i += 2) {
		assert(cstl_push_back(x, hogetab2[i]));
	}
	cstl_sort(x, less);
	pos = cstl_end(x);
	hoge.key = "XXX"; hoge.value = 9999;
	printf("%s, %4d,	p[%p]\n", hoge.key, hoge.value, pos);
	for (pos = cstl_begin(x); cstl_iter_ne(pos, cstl_end(x)); cstl_iter_incr(&pos)) {
		hoge = *cstl_iter_data(pos);
/*        printf("%s, %4d,	p[%p], prev[%p], next[%p]\n", hoge.key, hoge.value, pos, HogeList_prev(pos), HogeList_next(pos));*/
	}

	printf("after merge\n");
	cstl_merge(hl, x, less);
	assert(HogeList_verify(hl));
	assert(HogeList_verify(x));
	pos = cstl_end(hl);
	hoge.key = "XXX"; hoge.value = 9999;
	printf("%s, %4d,	p[%p]\n", hoge.key, hoge.value, pos);
	for (pos = cstl_begin(hl); cstl_iter_ne(pos, cstl_end(hl)); cstl_iter_incr(&pos)) {
		hoge = *cstl_iter_data(pos);
/*        printf("%s, %4d,	p[%p], prev[%p], next[%p]\n", hoge.key, hoge.value, pos, HogeList_prev(pos), HogeList_next(pos));*/
	}
	assert(cstl_empty(x));
	assert(cstl_size(hl) == NELEMS(hogetab2));


	cstl_delete(x);
	cstl_clear(hl);
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
	assert(cstl_size(il) == 0);
	cstl_clear(il);
	assert(cstl_insert_array(il, cstl_begin(il), buf, 32));
	assert(IntList_verify(il));
	assert(cstl_size(il) == 32);
	for (pos = cstl_begin(il), i = 0; cstl_iter_ne(pos, cstl_end(il)); cstl_iter_incr(&pos), i++) {
		assert(*cstl_iter_data(pos) == i);
	}
	/* resize */
	assert(cstl_resize(il, 64, 100));
	assert(IntList_verify(il));
	assert(cstl_size(il) == 64);
	for (pos = cstl_begin(il), i = 0; cstl_iter_ne(pos, cstl_end(il)); cstl_iter_incr(&pos), i++) {
		if (i < 32) {
			assert(*cstl_iter_data(pos) == i);
		} else {
			assert(*cstl_iter_data(pos) == 100);
		}
	}
	assert(cstl_resize(il, 16, 99));
	assert(IntList_verify(il));
	assert(cstl_size(il) == 16);
	for (pos = cstl_begin(il), i = 0; cstl_iter_ne(pos, cstl_end(il)); cstl_iter_incr(&pos), i++) {
		assert(*cstl_iter_data(pos) == i);
	}
	assert(cstl_resize(il, 0, 99));
	assert(IntList_verify(il));
	assert(cstl_size(il) == 0);
	assert(cstl_resize(il, 100, 99));
	/* swap */
	x = IntList_new();
	cstl_clear(il);
	assert(cstl_insert_array(il, cstl_begin(il), buf, 32));
	assert(cstl_insert_array(x, cstl_begin(x), b, sizeof b / sizeof b[0]));
	assert(cstl_size(il) == 32);
	for (pos = cstl_begin(il), i = 0; cstl_iter_ne(pos, cstl_end(il)); cstl_iter_incr(&pos), i++) {
		assert(*cstl_iter_data(pos) == i);
	}
	assert(cstl_size(x) == sizeof b / sizeof b[0]);
	for (pos = cstl_begin(x), i = 0; cstl_iter_ne(pos, cstl_end(x)); cstl_iter_incr(&pos), i++) {
		assert(*cstl_iter_data(pos) == b[i]);
	}
	cstl_swap(il, x);
	assert(IntList_verify(il));
	assert(IntList_verify(x));
	assert(cstl_size(x) == 32);
	printf("x:\n");
	for (pos = cstl_begin(x), i = 0; cstl_iter_ne(pos, cstl_end(x)); cstl_iter_incr(&pos), i++) {
		assert(*cstl_iter_data(pos) == i);
		printf("%2d, ", *cstl_iter_data(pos));
	}
	printf("\n");
	printf("il:\n");
	assert(cstl_size(il) == sizeof b / sizeof b[0]);
	for (pos = cstl_begin(il), i = 0; cstl_iter_ne(pos, cstl_end(il)); cstl_iter_incr(&pos), i++) {
		assert(*cstl_iter_data(pos) == b[i]);
		printf("%2d, ", *cstl_iter_data(pos));
	}
	printf("\n");
	/* splice */
	cstl_splice(x, cstl_begin(x), il, cstl_begin(il), cstl_end(il));
/*    cstl_splice(x, cstl_begin(x), il, cstl_begin(il), cstl_begin(il));*/
/*    cstl_splice(x, cstl_begin(x), il, cstl_begin(il), IntList_next(cstl_begin(il)));*/
/*    cstl_splice(x, cstl_begin(x), x, IntList_next(cstl_begin(x)), cstl_end(x));*/
/*    cstl_splice(x, cstl_end(x), x, IntList_next(cstl_begin(x)), cstl_end(x));*/
/*    cstl_splice(x, cstl_rbegin(x), x, IntList_next(cstl_begin(x)), IntList_prev(cstl_rbegin(x)));*/
	assert(IntList_verify(il));
	assert(IntList_verify(x));

	assert(cstl_size(il) == 0);
	assert(cstl_size(x) == 32 + sizeof b / sizeof b[0]);
	printf("x:\n");
	for (pos = cstl_begin(x), i = 0; cstl_iter_ne(pos, cstl_end(x)); cstl_iter_incr(&pos), i++) {
		printf("%2d, ", *cstl_iter_data(pos));
	}
	printf("\n");
	printf("il:\n");
	for (pos = cstl_begin(il), i = 0; cstl_iter_ne(pos, cstl_end(il)); cstl_iter_incr(&pos), i++) {
		printf("%2d, ", *cstl_iter_data(pos));
	}
	/* insert_range */
	cstl_clear(il);
	cstl_clear(x);
	assert(cstl_insert_array(il, cstl_begin(il), buf, 32));
	assert(cstl_insert_array(x, cstl_begin(x), b, sizeof b / sizeof b[0]));
	assert(cstl_insert_range(il, cstl_begin(il), cstl_begin(x), cstl_end(x)));
	printf("insert_range, il:\n");
	for (pos = cstl_begin(il), i = 0; cstl_iter_ne(pos, cstl_end(il)); cstl_iter_incr(&pos), i++) {
		printf("%2d, ", *cstl_iter_data(pos));
	}
	assert(IntList_verify(il));
	assert(IntList_verify(x));
	printf("\n");

	POOL_DUMP_OVERFLOW(&pool);
	cstl_delete(il);
	cstl_delete(x);
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
	cstl_delete(hl);
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
