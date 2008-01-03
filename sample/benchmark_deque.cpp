#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif
#include <cstl/deque.h>
#include <cstl/algorithm.h>
#include <deque>
#include <algorithm>


CSTL_DEQUE_INTERFACE(IntDeque, int)
CSTL_DEQUE_IMPLEMENT(IntDeque, int)


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

int main(void)
{
	int i;
	double t;
	IntDeque *x;
	deque<int> y;

	x = IntDeque_new();

	printf("*** benchmark deque<int> ***\n");
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
			IntDeque_push_back(x, i);
		} else {
			IntDeque_push_front(x, i);
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
	if (y.size() != IntDeque_size(x)) {
		printf("!!!NG!!!\n");
	}
	for (i = 0; i < COUNT; i++) {
		if (y[i] != *IntDeque_at(x, i)) {
			printf("!!!NG!!!\n");
		}
	}
	// pop_back/pop_front
	t = get_msec();
	for (i = 0; i < COUNT; i++) {
//        if (buf[i] % 2) {
		if (i % 2) {
			IntDeque_pop_back(x);
		} else {
			IntDeque_pop_front(x);
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
	if (!y.empty() || y.size() != IntDeque_size(x)) {
		printf("!!!NG!!!\n");
	}

	// push_back
	t = get_msec();
	for (i = 0; i < COUNT; i++) {
		IntDeque_push_back(x, i);
	}
	printf("cstl: push_back[%d]: %g ms\n", COUNT, get_msec() - t);

	t = get_msec();
	for (i = 0; i < COUNT; i++) {
		y.push_back(i);
	}
	printf("stl : push_back[%d]: %g ms\n", COUNT, get_msec() - t);
	if (y.size() != IntDeque_size(x)) {
		printf("!!!NG!!!\n");
	}
	for (i = 0; i < COUNT; i++) {
		if (y[i] != *IntDeque_at(x, i)) {
			printf("!!!NG!!!\n");
		}
	}
	// pop_back
	t = get_msec();
	for (i = 0; i < COUNT; i++) {
		IntDeque_pop_back(x);
	}
	printf("cstl: pop_back[%d]: %g ms\n", COUNT, get_msec() - t);

	t = get_msec();
	for (i = 0; i < COUNT; i++) {
		y.pop_back();
	}
	printf("stl : pop_back[%d]: %g ms\n", COUNT, get_msec() - t);
	if (!y.empty() || y.size() != IntDeque_size(x)) {
		printf("!!!NG!!!\n");
	}
	// push_front
	t = get_msec();
	for (i = 0; i < COUNT; i++) {
		IntDeque_push_front(x, i);
	}
	printf("cstl: push_front[%d]: %g ms\n", COUNT, get_msec() - t);

	t = get_msec();
	for (i = 0; i < COUNT; i++) {
		y.push_front(i);
	}
	printf("stl : push_front[%d]: %g ms\n", COUNT, get_msec() - t);
	if (y.size() != IntDeque_size(x)) {
		printf("!!!NG!!!\n");
	}
	for (i = 0; i < COUNT; i++) {
		if (y[i] != *IntDeque_at(x, i)) {
			printf("!!!NG!!!\n");
		}
	}
	// pop_front
	t = get_msec();
	for (i = 0; i < COUNT; i++) {
		IntDeque_pop_front(x);
	}
	printf("cstl: pop_front[%d]: %g ms\n", COUNT, get_msec() - t);

	t = get_msec();
	for (i = 0; i < COUNT; i++) {
		y.pop_front();
	}
	printf("stl : pop_front[%d]: %g ms\n", COUNT, get_msec() - t);
	if (!y.empty() || y.size() != IntDeque_size(x)) {
		printf("!!!NG!!!\n");
	}
	// insert front
	t = get_msec();
	for (i = 0; i < COUNT; i++) {
		IntDeque_insert(x, 0, i);
	}
	printf("cstl: insert front[%d]: %g ms\n", COUNT, get_msec() - t);

	t = get_msec();
	for (i = 0; i < COUNT; i++) {
		y.insert(y.begin(), i);
	}
	printf("stl : insert front[%d]: %g ms\n", COUNT, get_msec() - t);
	for (i = 0; i < COUNT; i++) {
		if (y[i] != *IntDeque_at(x, i)) {
			printf("!!!NG!!!\n");
		}
	}
	// erase front
	t = get_msec();
	for (i = 0; i < COUNT; i++) {
		IntDeque_erase(x, 0, 1);
	}
	printf("cstl: erase front[%d]: %g ms\n", COUNT, get_msec() - t);

	t = get_msec();
	for (i = 0; i < COUNT; i++) {
		y.erase(y.begin());
	}
	printf("stl : erase front[%d]: %g ms\n", COUNT, get_msec() - t);
	if (!y.empty() || y.size() != IntDeque_size(x)) {
		printf("!!!NG!!!\n");
	}
	// insert center
	t = get_msec();
	for (i = 0; i < INSERT_COUNT; i++) {
		IntDeque_insert(x, IntDeque_size(x) / 2, i);
	}
	printf("cstl: insert center[%d]: %g ms\n", INSERT_COUNT, get_msec() - t);

	t = get_msec();
	for (i = 0; i < INSERT_COUNT; i++) {
		y.insert(y.begin() + y.size() / 2, i);
	}
	printf("stl : insert center[%d]: %g ms\n", INSERT_COUNT, get_msec() - t);
	for (i = 0; i < INSERT_COUNT; i++) {
		if (y[i] != *IntDeque_at(x, i)) {
			printf("!!!NG!!!\n");
		}
	}
	// erase center
	t = get_msec();
	for (i = 0; i < INSERT_COUNT; i++) {
		IntDeque_erase(x, IntDeque_size(x) / 2, 1);
	}
	printf("cstl: erase center[%d]: %g ms\n", INSERT_COUNT, get_msec() - t);

	t = get_msec();
	for (i = 0; i < INSERT_COUNT; i++) {
		y.erase(y.begin() + y.size() / 2);
	}
	printf("stl : erase center[%d]: %g ms\n", INSERT_COUNT, get_msec() - t);
	if (!y.empty() || y.size() != IntDeque_size(x)) {
		printf("!!!NG!!!\n");
	}
	// insert back
	t = get_msec();
	for (i = 0; i < COUNT; i++) {
		IntDeque_insert(x, IntDeque_size(x), i);
	}
	printf("cstl: insert back[%d]: %g ms\n", COUNT, get_msec() - t);

	t = get_msec();
	for (i = 0; i < COUNT; i++) {
		y.insert(y.end(), i);
	}
	printf("stl : insert back[%d]: %g ms\n", COUNT, get_msec() - t);
	for (i = 0; i < COUNT; i++) {
		if (y[i] != *IntDeque_at(x, i)) {
			printf("!!!NG!!!\n");
		}
	}
	// at
	volatile int a;
	t = get_msec();
	for (i = 0; i < COUNT; i++) {
		a = *IntDeque_at(x, i);
	}
	printf("cstl: at[%d]: %g ms\n", COUNT, get_msec() - t);

	t = get_msec();
	for (i = 0; i < COUNT; i++) {
		a = y[i];
	}
	printf("stl : at[%d]: %g ms\n", COUNT, get_msec() - t);
	// erase back
	t = get_msec();
	for (i = 0; i < COUNT; i++) {
		IntDeque_erase(x, IntDeque_size(x) - 1, 1);
	}
	printf("cstl: erase back[%d]: %g ms\n", COUNT, get_msec() - t);

	t = get_msec();
	for (i = 0; i < COUNT; i++) {
		y.erase(y.end() - 1);
	}
	printf("stl : erase back[%d]: %g ms\n", COUNT, get_msec() - t);
	if (!y.empty() || y.size() != IntDeque_size(x)) {
		printf("!!!NG!!!\n");
	}


	int hoge;
	srand(time(0));
	// sort
	for (i = 0; i < SORT_COUNT; i++) {
		hoge = rand();
		IntDeque_push_back(x, hoge);
		y.push_back(hoge);
	}
	t = get_msec();
	IntDeque_sort(x, 0, IntDeque_size(x), comp);
	printf("cstl: sort[%d]: %g ms\n", SORT_COUNT, get_msec() - t);

	t = get_msec();
	sort(y.begin(), y.end());
	printf("stl : sort[%d]: %g ms\n", SORT_COUNT, get_msec() - t);

	for (i = 0; i < SORT_COUNT; i++) {
		if (y[i] != *IntDeque_at(x, i)) {
			printf("!!!NG!!!\n");
		}
	}
	// stable_sort
	for (i = 0; i < SORT_COUNT; i++) {
		hoge = rand();
		IntDeque_push_back(x, hoge);
		y.push_back(hoge);
	}
	t = get_msec();
	IntDeque_stable_sort(x, 0, IntDeque_size(x), comp);
	printf("cstl: stable_sort[%d]: %g ms\n", SORT_COUNT, get_msec() - t);

	t = get_msec();
	stable_sort(y.begin(), y.end());
	printf("stl : stable_sort[%d]: %g ms\n", SORT_COUNT, get_msec() - t);
	for (i = 0; i < SORT_COUNT; i++) {
		if (y[i] != *IntDeque_at(x, i)) {
			printf("!!!NG!!!\n");
		}
	}

	IntDeque_delete(x);
	return 0;
}

