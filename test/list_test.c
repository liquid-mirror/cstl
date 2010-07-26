#include <stdio.h>
#include <string.h>
#include "../unittest/UnitTest.h"
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

/*#undef CSTL_ASSERT*/
/*#define CSTL_ASSERT	ASSERT_FATAL*/

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
	/* 初期状態 */
	ASSERT(cstl_size(hl) == 0);
	ASSERT(cstl_empty(hl));
	ASSERT(cstl_iter_eq(cstl_begin(hl), cstl_end(hl)));
	ASSERT(cstl_iter_eq(cstl_rbegin(hl), cstl_rend(hl)));
	/* clear */
	hoge = hogetab[0];
	ASSERT(cstl_push_back(hl, hoge));
	ASSERT(cstl_push_back(hl, hoge));
	ASSERT(cstl_push_back(hl, hoge));
	ASSERT(HogeList_verify(hl));
	cstl_clear(hl);
	ASSERT(HogeList_verify(hl));
	ASSERT(cstl_size(hl) == 0);
	ASSERT(cstl_empty(hl));
}

void ListTest_test_1_2(void)
{
	Hoge hoge;
	int i;
	/* push_back */
	for (i = 0; i < NELEMS(hogetab); i++) {
		ASSERT(cstl_push_back(hl, hogetab[i]));
		ASSERT(HogeList_verify(hl));
	}
	ASSERT(cstl_size(hl) == NELEMS(hogetab));
	ASSERT(!cstl_empty(hl));
	/* pop_front */
	for (i = 0; i < NELEMS(hogetab); i++) {
		/* front */
		hoge = *cstl_front(hl);
		ASSERT(!strcmp(hoge.key, hogetab[i].key));
		ASSERT(hoge.value == hogetab[i].value);
		/* back */
		hoge = *cstl_back(hl);
		ASSERT(!strcmp(hoge.key, hogetab[NELEMS(hogetab)-1].key));
		ASSERT(hoge.value == hogetab[NELEMS(hogetab)-1].value);
		/* pop_front */
		cstl_pop_front(hl);
		ASSERT(HogeList_verify(hl));
	}
	ASSERT(cstl_size(hl) == 0);
	ASSERT(cstl_empty(hl));
	/* push_front */
	for (i = 0; i < NELEMS(hogetab); i++) {
		ASSERT(cstl_push_front(hl, hogetab[i]));
		ASSERT(HogeList_verify(hl));
	}
	ASSERT(cstl_size(hl) == NELEMS(hogetab));
	ASSERT(!cstl_empty(hl));
	/* pop_back */
	for (i = 0; i < NELEMS(hogetab); i++) {
		/* front */
		hoge = *cstl_front(hl);
		ASSERT(!strcmp(hoge.key, hogetab[NELEMS(hogetab)-1].key));
		ASSERT(hoge.value == hogetab[NELEMS(hogetab)-1].value);
		/* back */
		hoge = *cstl_back(hl);
		ASSERT(!strcmp(hoge.key, hogetab[i].key));
		ASSERT(hoge.value == hogetab[i].value);
		/* pop_back */
		cstl_pop_back(hl);
		ASSERT(HogeList_verify(hl));
	}
	ASSERT(cstl_size(hl) == 0);
	ASSERT(cstl_empty(hl));
}

void ListTest_test_1_3(void)
{
	Hoge hoge;
	int c = 0;
	/* 大量にpush_back */
	hoge = hogetab[0];
	while (c < 1000000 && cstl_push_back(hl, hoge)) {
		c++;
	}
/*    printf("size: %d, c: %d\n", cstl_size(hl), c);*/
	ASSERT(cstl_size(hl) == c);
	ASSERT(!cstl_empty(hl));
	/* 大量にpush_front */
	cstl_clear(hl);
	ASSERT(cstl_size(hl) == 0);
	ASSERT(cstl_empty(hl));
	c = 0;
	while (c < 1000000 && cstl_push_front(hl, hoge)) {
		c++;
	}
/*    printf("size: %d, c: %d\n", cstl_size(hl), c);*/
	ASSERT(cstl_size(hl) == c);
	ASSERT(!cstl_empty(hl));
	cstl_clear(hl);
	ASSERT(cstl_size(hl) == 0);
	ASSERT(cstl_empty(hl));
}

void ListTest_test_1_4(void)
{
	Hoge hoge;
	int i;
	HogeListIterator pos;
	/* push_back */
	for (i = 0; i < NELEMS(hogetab); i++) {
		ASSERT(cstl_push_back(hl, hogetab[i]));
	}
	ASSERT(cstl_size(hl) == NELEMS(hogetab));
	ASSERT(!cstl_empty(hl));
	/* data */
	for (i = 0, pos = cstl_begin(hl); cstl_iter_ne(pos, cstl_end(hl)); cstl_iter_inc(&pos), i++) {
		hoge = *cstl_iter_data(pos);
		ASSERT(!strcmp(hoge.key, hogetab[i].key));
		ASSERT(hoge.value == hogetab[i].value);
	}
	ASSERT(i == NELEMS(hogetab));
	for (i = 0, pos = cstl_begin(hl); cstl_iter_ne(pos, cstl_end(hl)); cstl_iter_inc(&pos), i++) {
		*cstl_iter_data(pos) = hogetab[0];
	}
	ASSERT(i == NELEMS(hogetab));
	for (pos = cstl_begin(hl); cstl_iter_ne(pos, cstl_end(hl)); cstl_iter_inc(&pos)) {
		hoge = *cstl_iter_data(pos);
		ASSERT(!strcmp(hoge.key, hogetab[0].key));
		ASSERT(hoge.value == hogetab[0].value);
	}
	while (!cstl_empty(hl)) {
		cstl_pop_front(hl);
	}
	ASSERT(cstl_size(hl) == 0);
}

void ListTest_test_1_5(void)
{
	Hoge hoge;
	int i;
	int flag = 0;
	HogeListIterator pos;
	HogeListIterator iter;
	/* beginの位置にinsert */
	for (i = 0; i < NELEMS(hogetab); i++) {
		ASSERT(cstl_insert(hl, cstl_begin(hl), hogetab[i], &iter));
		ASSERT(cstl_iter_eq(iter, cstl_begin(hl)));
		ASSERT(HogeList_verify(hl));
	}
	cstl_clear(hl);
	/* endの位置にinsert */
	for (i = 0; i < NELEMS(hogetab); i++) {
		ASSERT(cstl_insert(hl, cstl_end(hl), hogetab[i], &iter));
		cstl_iter_inc(&iter);
		ASSERT(cstl_iter_eq(iter, cstl_end(hl)));
		ASSERT(HogeList_verify(hl));
	}
	ASSERT(cstl_size(hl) == NELEMS(hogetab));
	ASSERT(!cstl_empty(hl));
	/* 真ん中の位置にinsert */
	for (i = 0, pos = cstl_begin(hl); cstl_iter_ne(pos, cstl_end(hl)); cstl_iter_inc(&pos), i++) {
		hoge = *cstl_iter_data(pos);
		ASSERT(!strcmp(hoge.key, hogetab[i].key));
		ASSERT(hoge.value == hogetab[i].value);
		if (i == NELEMS(hogetab)/2) {
			hoge = hogetab[0];
			ASSERT(cstl_insert(hl, pos, hoge, &iter));
			ASSERT(cstl_iter_eq(iter, cstl_iter_prev(pos)));
			ASSERT(HogeList_verify(hl));
			break;
		}
	}
	ASSERT(cstl_size(hl) == NELEMS(hogetab)+1);
	for (i = 0, pos = cstl_begin(hl); cstl_iter_ne(pos, cstl_end(hl)); cstl_iter_inc(&pos), i++) {
		hoge = *cstl_iter_data(pos);
		if (!flag && i == NELEMS(hogetab)/2) {
			ASSERT(!strcmp(hoge.key, hogetab[0].key));
			ASSERT(hoge.value == hogetab[0].value);
			flag = 1;
			i--;
		} else {
			ASSERT(!strcmp(hoge.key, hogetab[i].key));
			ASSERT(hoge.value == hogetab[i].value);
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
	/* push_back */
	for (i = 0; i < NELEMS(hogetab); i++) {
		ASSERT(cstl_push_back(hl, hogetab[i]));
	}
	ASSERT(cstl_size(hl) == NELEMS(hogetab));
	ASSERT(!cstl_empty(hl));
	/* beginの位置の要素をerase */
	size = cstl_size(hl);
	for (i = 0; i < NELEMS(hogetab); i++) {
		HogeListIterator next;
		pos = cstl_begin(hl);
		next = cstl_iter_next(pos);
		ASSERT(cstl_iter_eq(next, cstl_erase(hl, pos)));
		ASSERT(cstl_size(hl) == --size);
		ASSERT(HogeList_verify(hl));
	}
	ASSERT(cstl_size(hl) == 0);
	ASSERT(cstl_empty(hl));
	/* push_back */
	for (i = 0; i < NELEMS(hogetab); i++) {
		ASSERT(cstl_push_back(hl, hogetab[i]));
	}
	ASSERT(cstl_size(hl) == NELEMS(hogetab));
	ASSERT(!cstl_empty(hl));
	/* endの前の位置の要素をerase */
	size = cstl_size(hl);
	for (i = 0; i < NELEMS(hogetab); i++) {
		pos = cstl_end(hl);
		cstl_iter_dec(&pos);
		ASSERT(cstl_iter_eq(cstl_end(hl), cstl_erase(hl, pos)));
		ASSERT(cstl_size(hl) == --size);
		ASSERT(HogeList_verify(hl));
	}
	ASSERT(cstl_size(hl) == 0);
	ASSERT(cstl_empty(hl));
	/* push_back */
	for (i = 0; i < NELEMS(hogetab); i++) {
		ASSERT(cstl_push_back(hl, hogetab[i]));
	}
	ASSERT(cstl_size(hl) == NELEMS(hogetab));
	ASSERT(!cstl_empty(hl));
	/* 真ん中の位置の要素をerase */
	for (i = 0, pos = cstl_begin(hl); cstl_iter_ne(pos, cstl_end(hl)); i++) {
		if (i == NELEMS(hogetab)/2) {
			pos = cstl_erase(hl, pos);
		} else {
			cstl_iter_inc(&pos);
		}
		ASSERT(HogeList_verify(hl));
	}
	ASSERT(cstl_size(hl) == NELEMS(hogetab)-1);
	for (i = 0, pos = cstl_begin(hl); cstl_iter_ne(pos, cstl_end(hl)); cstl_iter_inc(&pos), i++) {
		hoge = *cstl_iter_data(pos);
		if (i >= NELEMS(hogetab)/2) {
			ASSERT(!strcmp(hoge.key, hogetab[i+1].key));
			ASSERT(hoge.value == hogetab[i+1].value);
		} else {
			ASSERT(!strcmp(hoge.key, hogetab[i].key));
			ASSERT(hoge.value == hogetab[i].value);
		}
	}
	/* erase_range */
	ASSERT(!cstl_empty(hl));
	ASSERT(cstl_iter_eq(cstl_end(hl), cstl_erase_range(hl, cstl_begin(hl), cstl_end(hl))));
	ASSERT(cstl_empty(hl));
	ASSERT(HogeList_verify(hl));
}

void ListTest_test_1_7(void)
{
	Hoge hoge;
	int i;
	HogeListReverseIterator pos;
	/* push_back */
	for (i = 0; i < NELEMS(hogetab); i++) {
		ASSERT(cstl_push_back(hl, hogetab[i]));
	}
	ASSERT(cstl_size(hl) == NELEMS(hogetab));
	ASSERT(!cstl_empty(hl));
	/* rbegin/rend 逆順に走査 */
	for (i = 0, pos = cstl_rbegin(hl); cstl_iter_ne(pos, cstl_rend(hl)); cstl_iter_inc(&pos), i++) {
		hoge = *cstl_iter_data(pos);
		ASSERT(!strcmp(hoge.key, hogetab[NELEMS(hogetab)-1 - i].key));
		ASSERT(hoge.value == hogetab[NELEMS(hogetab)-1 - i].value);
	}
	ASSERT(i == NELEMS(hogetab));
	/* TODO */
/*    ASSERT(HogeList_next(cstl_rbegin(hl)) == cstl_end(hl));*/
/*    ASSERT(HogeList_prev(cstl_begin(hl)) == cstl_rend(hl));*/
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

	/* NOTE: テストのために未公開のメンバを使用する */

	printf("before sort\n");
	/* push_back */
	for (i = 0; i < NELEMS(hogetab2); i++) {
		ASSERT(cstl_push_back(hl, hogetab2[i]));
	}
	hoge.key = "XXX"; hoge.value = 9999;
	printf("%s, %4d,	prev[%p], p[%p], next[%p]\n", hoge.key, hoge.value, hl->prev, hl, hl->next);
	for (pos = cstl_begin(hl); cstl_iter_ne(pos, cstl_end(hl)); cstl_iter_inc(&pos)) {
		HogeList *node = (HogeList*)pos.internal.data.data1;
		hoge = *cstl_iter_data(pos);
		printf("%s, %4d,	prev[%p], p[%p], next[%p]\n", hoge.key, hoge.value, node->prev, node, node->next);
	}
	/* sort less*/
	printf("size[%d]\n", cstl_size(hl));
	cstl_sort(hl, less);
	ASSERT(HogeList_verify(hl));
	printf("size[%d]\n", cstl_size(hl));
	printf("after sort (less)\n");
	hoge.key = "XXX"; hoge.value = 9999;
	prev.key = "000"; prev.value = 0;
	printf("%s, %4d,	prev[%p], p[%p], next[%p]\n", hoge.key, hoge.value, hl->prev, hl, hl->next);
	for (pos = cstl_begin(hl); cstl_iter_ne(pos, cstl_end(hl)); cstl_iter_inc(&pos)) {
		HogeList *node = (HogeList*)pos.internal.data.data1;
		hoge = *cstl_iter_data(pos);
		printf("%s, %4d,	prev[%p], p[%p], next[%p]\n", hoge.key, hoge.value, node->prev, node, node->next);
		ASSERT(strcmp(prev.key, hoge.key) <= 0);
		ASSERT(prev.value <= hoge.value);
		prev = hoge;
	}
	/* sort済みをsort */
	printf("size[%d]\n", cstl_size(hl));
	cstl_sort(hl, less);
	ASSERT(HogeList_verify(hl));
	printf("size[%d]\n", cstl_size(hl));
	printf("after sort (less) again\n");
	hoge.key = "XXX"; hoge.value = 9999;
	prev.key = "000"; prev.value = 0;
	printf("%s, %4d,	prev[%p], p[%p], next[%p]\n", hoge.key, hoge.value, hl->prev, hl, hl->next);
	for (pos = cstl_begin(hl); cstl_iter_ne(pos, cstl_end(hl)); cstl_iter_inc(&pos)) {
		HogeList *node = (HogeList*)pos.internal.data.data1;
		hoge = *cstl_iter_data(pos);
		printf("%s, %4d,	prev[%p], p[%p], next[%p]\n", hoge.key, hoge.value, node->prev, node, node->next);
		ASSERT(strcmp(prev.key, hoge.key) <= 0);
		ASSERT(prev.value <= hoge.value);
		prev = hoge;
	}
	/* sort greater*/
	printf("size[%d]\n", cstl_size(hl));
	cstl_sort(hl, greater);
	ASSERT(HogeList_verify(hl));
	printf("size[%d]\n", cstl_size(hl));
	printf("after sort (greater)\n");
	hoge.key = "XXX"; hoge.value = 9999;
	prev.key = "999"; prev.value = 999;
	printf("%s, %4d,	prev[%p], p[%p], next[%p]\n", hoge.key, hoge.value, hl->prev, hl, hl->next);
	for (pos = cstl_begin(hl); cstl_iter_ne(pos, cstl_end(hl)); cstl_iter_inc(&pos)) {
		HogeList *node = (HogeList*)pos.internal.data.data1;
		hoge = *cstl_iter_data(pos);
		printf("%s, %4d,	prev[%p], p[%p], next[%p]\n", hoge.key, hoge.value, node->prev, node, node->next);
		ASSERT(strcmp(prev.key, hoge.key) >= 0);
		if (strcmp(prev.key, hoge.key) == 0) {
			ASSERT(prev.value <= hoge.value);
		} else {
			ASSERT(prev.value >= hoge.value);
		}
		prev = hoge;
	}
	/* sort済みをsort */
	printf("size[%d]\n", cstl_size(hl));
	cstl_sort(hl, greater);
	ASSERT(HogeList_verify(hl));
	printf("size[%d]\n", cstl_size(hl));
	printf("after sort (greater) again\n");
	hoge.key = "XXX"; hoge.value = 9999;
	prev.key = "999"; prev.value = 999;
	printf("%s, %4d,	prev[%p], p[%p], next[%p]\n", hoge.key, hoge.value, hl->prev, hl, hl->next);
	for (pos = cstl_begin(hl); cstl_iter_ne(pos, cstl_end(hl)); cstl_iter_inc(&pos)) {
		HogeList *node = (HogeList*)pos.internal.data.data1;
		hoge = *cstl_iter_data(pos);
		printf("%s, %4d,	prev[%p], p[%p], next[%p]\n", hoge.key, hoge.value, node->prev, node, node->next);
		ASSERT(strcmp(prev.key, hoge.key) >= 0);
		if (strcmp(prev.key, hoge.key) == 0) {
			ASSERT(prev.value <= hoge.value);
		} else {
			ASSERT(prev.value >= hoge.value);
		}
		prev = hoge;
	}
	/* reverse */
	cstl_reverse(hl);
	ASSERT(HogeList_verify(hl));
	printf("size[%d]\n", cstl_size(hl));
	printf("reverse\n");
	hoge.key = "XXX"; hoge.value = 9999;
	printf("%s, %4d,	prev[%p], p[%p], next[%p]\n", hoge.key, hoge.value, hl->prev, hl, hl->next);
	for (pos = cstl_begin(hl); cstl_iter_ne(pos, cstl_end(hl)); cstl_iter_inc(&pos)) {
		HogeList *node = (HogeList*)pos.internal.data.data1;
		hoge = *cstl_iter_data(pos);
		printf("%s, %4d,	prev[%p], p[%p], next[%p]\n", hoge.key, hoge.value, node->prev, node, node->next);
	}
	/* merge */
	printf("\n");
	printf("before merge 1\n");
	cstl_clear(hl);
	for (i = 0; i < NELEMS(hogetab2); i += 2) {
		ASSERT(cstl_push_back(hl, hogetab2[i]));
	}
	cstl_sort(hl, less);
	hoge.key = "XXX"; hoge.value = 9999;
	printf("%s, %4d,	prev[%p], p[%p], next[%p]\n", hoge.key, hoge.value, hl->prev, hl, hl->next);
	for (pos = cstl_begin(hl); cstl_iter_ne(pos, cstl_end(hl)); cstl_iter_inc(&pos)) {
		HogeList *node = (HogeList*)pos.internal.data.data1;
		hoge = *cstl_iter_data(pos);
		printf("%s, %4d,	prev[%p], p[%p], next[%p]\n", hoge.key, hoge.value, node->prev, node, node->next);
	}

	printf("before merge 2\n");
	x = HogeList_new();
	for (i = 1; i < NELEMS(hogetab2); i += 2) {
		ASSERT(cstl_push_back(x, hogetab2[i]));
	}
	cstl_sort(x, less);
	hoge.key = "XXX"; hoge.value = 9999;
	printf("%s, %4d,	prev[%p], p[%p], next[%p]\n", hoge.key, hoge.value, x->prev, x, x->next);
	for (pos = cstl_begin(x); cstl_iter_ne(pos, cstl_end(x)); cstl_iter_inc(&pos)) {
		HogeList *node = (HogeList*)pos.internal.data.data1;
		hoge = *cstl_iter_data(pos);
		printf("%s, %4d,	prev[%p], p[%p], next[%p]\n", hoge.key, hoge.value, node->prev, node, node->next);
	}

	printf("after merge\n");
	cstl_merge(hl, x, less);
	ASSERT(HogeList_verify(hl));
	ASSERT(HogeList_verify(x));
	hoge.key = "XXX"; hoge.value = 9999;
	printf("%s, %4d,	prev[%p], p[%p], next[%p]\n", hoge.key, hoge.value, hl->prev, hl, hl->next);
	for (pos = cstl_begin(hl); cstl_iter_ne(pos, cstl_end(hl)); cstl_iter_inc(&pos)) {
		HogeList *node = (HogeList*)pos.internal.data.data1;
		hoge = *cstl_iter_data(pos);
		printf("%s, %4d,	prev[%p], p[%p], next[%p]\n", hoge.key, hoge.value, node->prev, node, node->next);
	}
	ASSERT(cstl_empty(x));
	ASSERT(cstl_size(hl) == NELEMS(hogetab2));

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
	for (i = 0; i < 32; i++) buf[i] = i;
	/* insert_array */
	ASSERT(cstl_size(il) == 0);
	cstl_clear(il);
	ASSERT(cstl_insert_array(il, cstl_begin(il), buf, 32));
	ASSERT(IntList_verify(il));
	ASSERT(cstl_size(il) == 32);
	for (pos = cstl_begin(il), i = 0; cstl_iter_ne(pos, cstl_end(il)); cstl_iter_inc(&pos), i++) {
		ASSERT(*cstl_iter_data(pos) == i);
	}
	/* resize */
	ASSERT(cstl_resize(il, 64, 100));
	ASSERT(IntList_verify(il));
	ASSERT(cstl_size(il) == 64);
	for (pos = cstl_begin(il), i = 0; cstl_iter_ne(pos, cstl_end(il)); cstl_iter_inc(&pos), i++) {
		if (i < 32) {
			ASSERT(*cstl_iter_data(pos) == i);
		} else {
			ASSERT(*cstl_iter_data(pos) == 100);
		}
	}
	ASSERT(cstl_resize(il, 16, 99));
	ASSERT(IntList_verify(il));
	ASSERT(cstl_size(il) == 16);
	for (pos = cstl_begin(il), i = 0; cstl_iter_ne(pos, cstl_end(il)); cstl_iter_inc(&pos), i++) {
		ASSERT(*cstl_iter_data(pos) == i);
	}
	ASSERT(cstl_resize(il, 0, 99));
	ASSERT(IntList_verify(il));
	ASSERT(cstl_size(il) == 0);
	ASSERT(cstl_resize(il, 100, 99));
	/* swap */
	x = IntList_new();
	cstl_clear(il);
	ASSERT(cstl_insert_array(il, cstl_begin(il), buf, 32));
	ASSERT(cstl_insert_array(x, cstl_begin(x), b, sizeof b / sizeof b[0]));
	ASSERT(cstl_size(il) == 32);
	for (pos = cstl_begin(il), i = 0; cstl_iter_ne(pos, cstl_end(il)); cstl_iter_inc(&pos), i++) {
		ASSERT(*cstl_iter_data(pos) == i);
	}
	ASSERT(cstl_size(x) == sizeof b / sizeof b[0]);
	for (pos = cstl_begin(x), i = 0; cstl_iter_ne(pos, cstl_end(x)); cstl_iter_inc(&pos), i++) {
		ASSERT(*cstl_iter_data(pos) == b[i]);
	}
	cstl_swap(il, x);
	ASSERT(IntList_verify(il));
	ASSERT(IntList_verify(x));
	ASSERT(cstl_size(x) == 32);
	printf("splice\n");
	printf("x:\n");
	for (pos = cstl_begin(x), i = 0; cstl_iter_ne(pos, cstl_end(x)); cstl_iter_inc(&pos), i++) {
		ASSERT(*cstl_iter_data(pos) == i);
		printf("%2d, ", *cstl_iter_data(pos));
	}
	printf("\n");
	printf("il:\n");
	ASSERT(cstl_size(il) == sizeof b / sizeof b[0]);
	for (pos = cstl_begin(il), i = 0; cstl_iter_ne(pos, cstl_end(il)); cstl_iter_inc(&pos), i++) {
		ASSERT(*cstl_iter_data(pos) == b[i]);
		printf("%2d, ", *cstl_iter_data(pos));
	}
	printf("\n");
	/* splice */
	cstl_splice(x, cstl_begin(x), il, cstl_begin(il), cstl_end(il));
/*    cstl_splice(x, cstl_begin(x), il, cstl_begin(il), cstl_begin(il));*/
/*    cstl_splice(x, cstl_begin(x), il, cstl_begin(il), cstl_iter_next(cstl_begin(il)));*/
/*    cstl_splice(x, cstl_begin(x), x, cstl_iter_next(cstl_begin(x)), cstl_end(x));*/
/*    cstl_splice(x, cstl_end(x), x, cstl_iter_next(cstl_begin(x)), cstl_end(x));*/
/*    pos = cstl_iter_prev(cstl_end(x));*/
/*    cstl_splice(x, pos, x, cstl_iter_next(cstl_begin(x)), cstl_iter_prev(pos));*/
	ASSERT(IntList_verify(il));
	ASSERT(IntList_verify(x));

	ASSERT(cstl_size(il) == 0);
	ASSERT(cstl_size(x) == 32 + sizeof b / sizeof b[0]);
	printf("x:\n");
	for (pos = cstl_begin(x), i = 0; cstl_iter_ne(pos, cstl_end(x)); cstl_iter_inc(&pos), i++) {
		printf("%2d, ", *cstl_iter_data(pos));
	}
	printf("\n");
	printf("il:\n");
	for (pos = cstl_begin(il), i = 0; cstl_iter_ne(pos, cstl_end(il)); cstl_iter_inc(&pos), i++) {
		printf("%2d, ", *cstl_iter_data(pos));
	}
	printf("\n");
	printf("\n");
	/* insert_range */
	cstl_clear(il);
	cstl_clear(x);
	ASSERT(cstl_insert_array(il, cstl_begin(il), buf, 32));
	ASSERT(cstl_insert_array(x, cstl_begin(x), b, sizeof b / sizeof b[0]));
	ASSERT(cstl_insert_range(il, cstl_begin(il), cstl_begin(x), cstl_end(x)));
	printf("insert_range, il:\n");
	for (pos = cstl_begin(il), i = 0; cstl_iter_ne(pos, cstl_end(il)); cstl_iter_inc(&pos), i++) {
		printf("%2d, ", *cstl_iter_data(pos));
	}
	ASSERT(IntList_verify(il));
	ASSERT(IntList_verify(x));
	printf("\n");

	POOL_DUMP_OVERFLOW(&pool);
	cstl_delete(il);
	cstl_delete(x);
}



void ListTest_run(void)
{
	printf("\n===== list test =====\n");
	hl = HogeList_new();
	ASSERT(hl);
	ASSERT(HogeList_verify(hl));

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



static TestCase list_tests[] = {
	{ "ListTest_test_1_1", ListTest_test_1_1 },
	{ "ListTest_test_1_2", ListTest_test_1_2 },
	{ "ListTest_test_1_3", ListTest_test_1_3 },
	{ "ListTest_test_1_4", ListTest_test_1_4 },
	{ "ListTest_test_1_5", ListTest_test_1_5 },
	{ "ListTest_test_1_6", ListTest_test_1_6 },
	{ "ListTest_test_1_7", ListTest_test_1_7 },
	{ "ListTest_test_1_8", ListTest_test_1_8 },
	{ "ListTest_test_2_1", ListTest_test_2_1 },
	TEST_CASE_NULL,
};

static int setup(void)
{
	hl = HogeList_new();
	return hl == 0;
}

static int teardown(void)
{
	POOL_DUMP_OVERFLOW(&pool);
	cstl_delete(hl);
	return 0;
}

static TestSuite suites[] = {
	{ "list_test", setup, teardown, list_tests },
	TEST_SUITE_NULL,
};

int main(int argc, char *argv[])
{
#ifdef MY_MALLOC
	Pool_init(&pool, buf, sizeof buf, sizeof buf[0]);
#endif
	if (argc < 2) {
		unittest_run_all(suites);
	} else {
		unittest_run_interactive(suites);
	}
#ifdef MY_MALLOC
	POOL_DUMP_LEAK(&pool, 0);
#endif
	return 0;
}
