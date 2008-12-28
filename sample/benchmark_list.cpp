#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif
#include <cstl/list.h>
#include <list>
#include <functional>

//#define malloc(s) ::operator new(s)
//#define free(p) ::operator delete(p)

CSTL_LIST_INTERFACE(IntList, int)
CSTL_LIST_IMPLEMENT(IntList, int)


using namespace std;


double get_msec(void)
{
#ifdef _WIN32
	return (double) GetTickCount();
#else
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000.0 + tv.tv_usec / 1000.0;
#endif
}

#define COUNT			(1000000)
#define INSERT_COUNT	(10000)
#define SORT_COUNT		(1000000)

int comp(const void *x, const void *y)
{
	if (*(int*)x < *(int*)y) {
		return -1;
	} else if (*(int*)x > *(int*)y) {
		return 1;
	} else {
		return 0;
	}
}

int greater_comp(const void *x, const void *y)
{
	if (*(int*)x < *(int*)y) {
		return 1;
	} else if (*(int*)x > *(int*)y) {
		return -1;
	} else {
		return 0;
	}
}

int main(void)
{
	int i;
	double t;
	IntList *x;
	list<int> y;
	IntListIterator xpos;
	list<int>::iterator ypos;

	x = IntList_new();

	printf("*** benchmark list<int> ***\n");
	static int buf[COUNT];
	srand(time(0));
	for (i = 0; i < COUNT; i++) {
		buf[i] = rand();
	}

	// push_back/push_front
	t = get_msec();
	for (i = 0; i < COUNT; i++) {
//        if (buf[i] % 2) {
		if (i % 2) {
			IntList_push_back(x, i);
		} else {
			IntList_push_front(x, i);
		}
	}
	printf("cstl: push_back/push_front[%d]: %g ms\n", COUNT, get_msec() - t);

	t = get_msec();
	for (i = 0; i < COUNT; i++) {
//        if (buf[i] % 2) {
		if (i % 2) {
			y.push_back(i);
		} else {
			y.push_front(i);
		}
	}
	printf("stl : push_back/push_front[%d]: %g ms\n", COUNT, get_msec() - t);
	if (y.size() != IntList_size(x)) {
		printf("!!!NG!!!\n");
	}
	for (xpos = IntList_begin(x), ypos = y.begin(); ypos != y.end(); xpos = IntListIterator_next(xpos), ++ypos) {
		if (*ypos != *IntListIterator_ref(xpos)) {
			printf("!!!NG!!!\n");
		}
	}
	// pop_back/pop_front
	t = get_msec();
	for (i = 0; i < COUNT; i++) {
//        if (buf[i] % 2) {
		if (i % 2) {
			IntList_pop_back(x);
		} else {
			IntList_pop_front(x);
		}
	}
	printf("cstl: pop_back/pop_front[%d]: %g ms\n", COUNT, get_msec() - t);

	t = get_msec();
	for (i = 0; i < COUNT; i++) {
//        if (buf[i] % 2) {
		if (i % 2) {
			y.pop_back();
		} else {
			y.pop_front();
		}
	}
	printf("stl : pop_back/pop_front[%d]: %g ms\n", COUNT, get_msec() - t);
	if (!y.empty() || y.size() != IntList_size(x)) {
		printf("!!!NG!!!\n");
	}

	// push_back
	t = get_msec();
	for (i = 0; i < COUNT; i++) {
		IntList_push_back(x, i);
	}
	printf("cstl: push_back[%d]: %g ms\n", COUNT, get_msec() - t);

	t = get_msec();
	for (i = 0; i < COUNT; i++) {
		y.push_back(i);
	}
	printf("stl : push_back[%d]: %g ms\n", COUNT, get_msec() - t);
	if (y.size() != IntList_size(x)) {
		printf("!!!NG!!!\n");
	}
	for (xpos = IntList_begin(x), ypos = y.begin(); ypos != y.end(); xpos = IntListIterator_next(xpos), ++ypos) {
		if (*ypos != *IntListIterator_ref(xpos)) {
			printf("!!!NG!!!\n");
		}
	}
	// pop_back
	t = get_msec();
	for (i = 0; i < COUNT; i++) {
		IntList_pop_back(x);
	}
	printf("cstl: pop_back[%d]: %g ms\n", COUNT, get_msec() - t);

	t = get_msec();
	for (i = 0; i < COUNT; i++) {
		y.pop_back();
	}
	printf("stl : pop_back[%d]: %g ms\n", COUNT, get_msec() - t);
	if (!y.empty() || y.size() != IntList_size(x)) {
		printf("!!!NG!!!\n");
	}
	// push_front
	t = get_msec();
	for (i = 0; i < COUNT; i++) {
		IntList_push_front(x, i);
	}
	printf("cstl: push_front[%d]: %g ms\n", COUNT, get_msec() - t);

	t = get_msec();
	for (i = 0; i < COUNT; i++) {
		y.push_front(i);
	}
	printf("stl : push_front[%d]: %g ms\n", COUNT, get_msec() - t);
	if (y.size() != IntList_size(x)) {
		printf("!!!NG!!!\n");
	}
	for (xpos = IntList_begin(x), ypos = y.begin(); ypos != y.end(); xpos = IntListIterator_next(xpos), ++ypos) {
		if (*ypos != *IntListIterator_ref(xpos)) {
			printf("!!!NG!!!\n");
		}
	}
	// pop_front
	t = get_msec();
	for (i = 0; i < COUNT; i++) {
		IntList_pop_front(x);
	}
	printf("cstl: pop_front[%d]: %g ms\n", COUNT, get_msec() - t);

	t = get_msec();
	for (i = 0; i < COUNT; i++) {
		y.pop_front();
	}
	printf("stl : pop_front[%d]: %g ms\n", COUNT, get_msec() - t);
	if (!y.empty() || y.size() != IntList_size(x)) {
		printf("!!!NG!!!\n");
	}


	IntList_clear(x);
	y.clear();
	int hoge;
	srand(time(0));
	// sort
	for (i = 0; i < SORT_COUNT; i++) {
		hoge = rand();
		IntList_push_back(x, hoge);
		y.push_back(hoge);
	}
	t = get_msec();
	IntList_sort(x, comp);
	printf("cstl: sort[%d]: %g ms\n", SORT_COUNT, get_msec() - t);

	t = get_msec();
	y.sort();
	printf("stl : sort[%d]: %g ms\n", SORT_COUNT, get_msec() - t);

	for (xpos = IntList_begin(x), ypos = y.begin(); ypos != y.end(); xpos = IntListIterator_next(xpos), ++ypos) {
		if (*ypos != *IntListIterator_ref(xpos)) {
			printf("!!!NG!!! :stl[%d], cstl[%d]\n", *ypos, *IntListIterator_ref(xpos));
		}
	}
	// sort 2
	t = get_msec();
	IntList_sort(x, comp);
	printf("cstl: sort2[%d]: %g ms\n", SORT_COUNT, get_msec() - t);

	t = get_msec();
	y.sort();
	printf("stl : sort2[%d]: %g ms\n", SORT_COUNT, get_msec() - t);

	for (xpos = IntList_begin(x), ypos = y.begin(); ypos != y.end(); xpos = IntListIterator_next(xpos), ++ypos) {
		if (*ypos != *IntListIterator_ref(xpos)) {
			printf("!!!NG!!! :stl[%d], cstl[%d]\n", *ypos, *IntListIterator_ref(xpos));
		}
	}
	// sort 3
	t = get_msec();
	IntList_sort(x, greater_comp);
	printf("cstl: sort3[%d]: %g ms\n", SORT_COUNT, get_msec() - t);

	t = get_msec();
	y.sort(std::greater<int>());
	printf("stl : sort3[%d]: %g ms\n", SORT_COUNT, get_msec() - t);

	for (xpos = IntList_begin(x), ypos = y.begin(); ypos != y.end(); xpos = IntListIterator_next(xpos), ++ypos) {
		if (*ypos != *IntListIterator_ref(xpos)) {
			printf("!!!NG!!! :stl[%d], cstl[%d]\n", *ypos, *IntListIterator_ref(xpos));
		}
	}

	IntList_delete(x);
	return 0;
}


