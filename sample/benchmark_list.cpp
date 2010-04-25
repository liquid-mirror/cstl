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
			cstl_push_back(x, i);
		} else {
			cstl_push_front(x, i);
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
	if (y.size() != cstl_size(x)) {
		printf("!!!NG!!!\n");
	}
	for (xpos = cstl_begin(x), ypos = y.begin(); ypos != y.end(); cstl_iter_incr(xpos), ++ypos) {
		if (*ypos != *cstl_iter_data(xpos)) {
			printf("!!!NG!!!\n");
		}
	}
	// pop_back/pop_front
	t = get_msec();
	for (i = 0; i < COUNT; i++) {
//        if (buf[i] % 2) {
		if (i % 2) {
			cstl_pop_back(x);
		} else {
			cstl_pop_front(x);
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
	if (!y.empty() || y.size() != cstl_size(x)) {
		printf("!!!NG!!!\n");
	}

	// push_back
	t = get_msec();
	for (i = 0; i < COUNT; i++) {
		cstl_push_back(x, i);
	}
	printf("cstl: push_back[%d]: %g ms\n", COUNT, get_msec() - t);

	t = get_msec();
	for (i = 0; i < COUNT; i++) {
		y.push_back(i);
	}
	printf("stl : push_back[%d]: %g ms\n", COUNT, get_msec() - t);
	if (y.size() != cstl_size(x)) {
		printf("!!!NG!!!\n");
	}
	for (xpos = cstl_begin(x), ypos = y.begin(); ypos != y.end(); cstl_iter_incr(xpos), ++ypos) {
		if (*ypos != *cstl_iter_data(xpos)) {
			printf("!!!NG!!!\n");
		}
	}
	// pop_back
	t = get_msec();
	for (i = 0; i < COUNT; i++) {
		cstl_pop_back(x);
	}
	printf("cstl: pop_back[%d]: %g ms\n", COUNT, get_msec() - t);

	t = get_msec();
	for (i = 0; i < COUNT; i++) {
		y.pop_back();
	}
	printf("stl : pop_back[%d]: %g ms\n", COUNT, get_msec() - t);
	if (!y.empty() || y.size() != cstl_size(x)) {
		printf("!!!NG!!!\n");
	}
	// push_front
	t = get_msec();
	for (i = 0; i < COUNT; i++) {
		cstl_push_front(x, i);
	}
	printf("cstl: push_front[%d]: %g ms\n", COUNT, get_msec() - t);

	t = get_msec();
	for (i = 0; i < COUNT; i++) {
		y.push_front(i);
	}
	printf("stl : push_front[%d]: %g ms\n", COUNT, get_msec() - t);
	if (y.size() != cstl_size(x)) {
		printf("!!!NG!!!\n");
	}
	for (xpos = cstl_begin(x), ypos = y.begin(); ypos != y.end(); cstl_iter_incr(xpos), ++ypos) {
		if (*ypos != *cstl_iter_data(xpos)) {
			printf("!!!NG!!!\n");
		}
	}
	// pop_front
	t = get_msec();
	for (i = 0; i < COUNT; i++) {
		cstl_pop_front(x);
	}
	printf("cstl: pop_front[%d]: %g ms\n", COUNT, get_msec() - t);

	t = get_msec();
	for (i = 0; i < COUNT; i++) {
		y.pop_front();
	}
	printf("stl : pop_front[%d]: %g ms\n", COUNT, get_msec() - t);
	if (!y.empty() || y.size() != cstl_size(x)) {
		printf("!!!NG!!!\n");
	}


	cstl_clear(x);
	y.clear();
	int hoge;
	srand(time(0));
	// sort
	for (i = 0; i < SORT_COUNT; i++) {
		hoge = rand();
		cstl_push_back(x, hoge);
		y.push_back(hoge);
	}
	t = get_msec();
	cstl_sort(x, comp);
	printf("cstl: sort[%d]: %g ms\n", SORT_COUNT, get_msec() - t);

	t = get_msec();
	y.sort();
	printf("stl : sort[%d]: %g ms\n", SORT_COUNT, get_msec() - t);

	for (xpos = cstl_begin(x), ypos = y.begin(); ypos != y.end(); cstl_iter_incr(xpos), ++ypos) {
		if (*ypos != *cstl_iter_data(xpos)) {
			printf("!!!NG!!! :stl[%d], cstl[%d]\n", *ypos, *cstl_iter_data(xpos));
		}
	}
	// sort 2
	t = get_msec();
	cstl_sort(x, comp);
	printf("cstl: sort2[%d]: %g ms\n", SORT_COUNT, get_msec() - t);

	t = get_msec();
	y.sort();
	printf("stl : sort2[%d]: %g ms\n", SORT_COUNT, get_msec() - t);

	for (xpos = cstl_begin(x), ypos = y.begin(); ypos != y.end(); cstl_iter_incr(xpos), ++ypos) {
		if (*ypos != *cstl_iter_data(xpos)) {
			printf("!!!NG!!! :stl[%d], cstl[%d]\n", *ypos, *cstl_iter_data(xpos));
		}
	}
	// sort 3
	t = get_msec();
	cstl_sort(x, greater_comp);
	printf("cstl: sort3[%d]: %g ms\n", SORT_COUNT, get_msec() - t);

	t = get_msec();
	y.sort(std::greater<int>());
	printf("stl : sort3[%d]: %g ms\n", SORT_COUNT, get_msec() - t);

	for (xpos = cstl_begin(x), ypos = y.begin(); ypos != y.end(); cstl_iter_incr(xpos), ++ypos) {
		if (*ypos != *cstl_iter_data(xpos)) {
			printf("!!!NG!!! :stl[%d], cstl[%d]\n", *ypos, *cstl_iter_data(xpos));
		}
	}
	// sort 4
	for (xpos = cstl_begin(x), ypos = y.begin(), i = 0; ypos != y.end();
			cstl_iter_incr(xpos), ++ypos, i++) {
		hoge = rand();
		*cstl_iter_data(xpos) = hoge;
		*ypos = hoge;
	}
	t = get_msec();
	cstl_sort(x, comp);
	printf("cstl: sort4[%d]: %g ms\n", SORT_COUNT, get_msec() - t);

	t = get_msec();
	y.sort();
	printf("stl : sort4[%d]: %g ms\n", SORT_COUNT, get_msec() - t);

	for (xpos = cstl_begin(x), ypos = y.begin(); ypos != y.end(); cstl_iter_incr(xpos), ++ypos) {
		if (*ypos != *cstl_iter_data(xpos)) {
			printf("!!!NG!!! :stl[%d], cstl[%d]\n", *ypos, *cstl_iter_data(xpos));
		}
	}
	// sort 5
	t = get_msec();
	cstl_sort(x, comp);
	printf("cstl: sort5[%d]: %g ms\n", SORT_COUNT, get_msec() - t);

	t = get_msec();
	y.sort();
	printf("stl : sort5[%d]: %g ms\n", SORT_COUNT, get_msec() - t);

	for (xpos = cstl_begin(x), ypos = y.begin(); ypos != y.end(); cstl_iter_incr(xpos), ++ypos) {
		if (*ypos != *cstl_iter_data(xpos)) {
			printf("!!!NG!!! :stl[%d], cstl[%d]\n", *ypos, *cstl_iter_data(xpos));
		}
	}
	// sort 6
	t = get_msec();
	cstl_sort(x, greater_comp);
	printf("cstl: sort6[%d]: %g ms\n", SORT_COUNT, get_msec() - t);

	t = get_msec();
	y.sort(std::greater<int>());
	printf("stl : sort6[%d]: %g ms\n", SORT_COUNT, get_msec() - t);

	for (xpos = cstl_begin(x), ypos = y.begin(); ypos != y.end(); cstl_iter_incr(xpos), ++ypos) {
		if (*ypos != *cstl_iter_data(xpos)) {
			printf("!!!NG!!! :stl[%d], cstl[%d]\n", *ypos, *cstl_iter_data(xpos));
		}
	}

	cstl_delete(x);
	return 0;
}


