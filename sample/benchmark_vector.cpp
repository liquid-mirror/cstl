#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif
#include <cstl/vector.h>
#include <cstl/algorithm.h>
#include <vector>
#include <algorithm>


CSTL_VECTOR_INTERFACE(IntVector, int)
CSTL_VECTOR_IMPLEMENT(IntVector, int)


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
	IntVector *x;
	vector<int> y;

	x = IntVector_new(8);
	y.reserve(8);

	printf("*** benchmark vector<int> ***\n");
	// push_back
	t = get_msec();
	for (i = 0; i < COUNT; i++) {
		IntVector_push_back(x, i);
	}
	printf("cstl: push_back[%d]: %g ms\n", COUNT, get_msec() - t);

	t = get_msec();
	for (i = 0; i < COUNT; i++) {
		y.push_back(i);
	}
	printf("stl : push_back[%d]: %g ms\n", COUNT, get_msec() - t);
	if (y.size() != IntVector_size(x)) {
		printf("!!!NG!!!\n");
	}
	for (i = 0; i < COUNT; i++) {
		if (y[i] != *IntVector_at(x, i)) {
			printf("!!!NG!!!\n");
		}
	}
	// pop_back
	t = get_msec();
	for (i = 0; i < COUNT; i++) {
		IntVector_pop_back(x);
	}
	printf("cstl: pop_back[%d]: %g ms\n", COUNT, get_msec() - t);

	t = get_msec();
	for (i = 0; i < COUNT; i++) {
		y.pop_back();
	}
	printf("stl : pop_back[%d]: %g ms\n", COUNT, get_msec() - t);
	if (!y.empty() || y.size() != IntVector_size(x)) {
		printf("!!!NG!!!\n");
	}
	// insert front
	t = get_msec();
	for (i = 0; i < INSERT_COUNT; i++) {
		IntVector_insert(x, 0, i);
	}
	printf("cstl: insert front[%d]: %g ms\n", INSERT_COUNT, get_msec() - t);

	t = get_msec();
	for (i = 0; i < INSERT_COUNT; i++) {
		y.insert(y.begin(), i);
	}
	printf("stl : insert front[%d]: %g ms\n", INSERT_COUNT, get_msec() - t);
	for (i = 0; i < INSERT_COUNT; i++) {
		if (y[i] != *IntVector_at(x, i)) {
			printf("!!!NG!!!\n");
		}
	}
	// erase front
	t = get_msec();
	for (i = 0; i < INSERT_COUNT; i++) {
		IntVector_erase(x, 0, 1);
	}
	printf("cstl: erase front[%d]: %g ms\n", INSERT_COUNT, get_msec() - t);

	t = get_msec();
	for (i = 0; i < INSERT_COUNT; i++) {
		y.erase(y.begin());
	}
	printf("stl : erase front[%d]: %g ms\n", INSERT_COUNT, get_msec() - t);
	if (!y.empty() || y.size() != IntVector_size(x)) {
		printf("!!!NG!!!\n");
	}
	// insert center
	t = get_msec();
	for (i = 0; i < INSERT_COUNT; i++) {
		IntVector_insert(x, IntVector_size(x) / 2, i);
	}
	printf("cstl: insert center[%d]: %g ms\n", INSERT_COUNT, get_msec() - t);

	t = get_msec();
	for (i = 0; i < INSERT_COUNT; i++) {
		y.insert(y.begin() + y.size() / 2, i);
	}
	printf("stl : insert center[%d]: %g ms\n", INSERT_COUNT, get_msec() - t);
	for (i = 0; i < INSERT_COUNT; i++) {
		if (y[i] != *IntVector_at(x, i)) {
			printf("!!!NG!!!\n");
		}
	}
	// erase center
	t = get_msec();
	for (i = 0; i < INSERT_COUNT; i++) {
		IntVector_erase(x, IntVector_size(x) / 2, 1);
	}
	printf("cstl: erase center[%d]: %g ms\n", INSERT_COUNT, get_msec() - t);

	t = get_msec();
	for (i = 0; i < INSERT_COUNT; i++) {
		y.erase(y.begin() + y.size() / 2);
	}
	printf("stl : erase center[%d]: %g ms\n", INSERT_COUNT, get_msec() - t);
	if (!y.empty() || y.size() != IntVector_size(x)) {
		printf("!!!NG!!!\n");
	}
	// insert back
	t = get_msec();
	for (i = 0; i < COUNT; i++) {
		IntVector_insert(x, IntVector_size(x), i);
	}
	printf("cstl: insert back[%d]: %g ms\n", COUNT, get_msec() - t);

	t = get_msec();
	for (i = 0; i < COUNT; i++) {
		y.insert(y.end(), i);
	}
	printf("stl : insert back[%d]: %g ms\n", COUNT, get_msec() - t);
	for (i = 0; i < COUNT; i++) {
		if (y[i] != *IntVector_at(x, i)) {
			printf("!!!NG!!!\n");
		}
	}
	// erase back
	t = get_msec();
	for (i = 0; i < COUNT; i++) {
		IntVector_erase(x, IntVector_size(x) - 1, 1);
	}
	printf("cstl: erase back[%d]: %g ms\n", COUNT, get_msec() - t);

	t = get_msec();
	for (i = 0; i < COUNT; i++) {
		y.erase(y.end() - 1);
	}
	printf("stl : erase back[%d]: %g ms\n", COUNT, get_msec() - t);
	if (!y.empty() || y.size() != IntVector_size(x)) {
		printf("!!!NG!!!\n");
	}


	static int buf[SORT_COUNT];
	srand(time(0));
	// sort
	for (i = 0; i < SORT_COUNT; i++) {
		buf[i] = rand();
		IntVector_push_back(x, buf[i]);
		y.push_back(buf[i]);
	}
	t = get_msec();
	IntVector_sort(x, 0, IntVector_size(x), comp);
	printf("cstl: sort[%d]: %g ms\n", SORT_COUNT, get_msec() - t);

	t = get_msec();
	sort(y.begin(), y.end());
	printf("stl : sort[%d]: %g ms\n", SORT_COUNT, get_msec() - t);

	t = get_msec();
	qsort(buf, SORT_COUNT, sizeof(int), comp);
	printf("libc: qsort[%d]: %g ms\n", SORT_COUNT, get_msec() - t);

	for (i = 0; i < SORT_COUNT; i++) {
		if (y[i] != *IntVector_at(x, i) || y[i] != buf[i]) {
			printf("!!!NG!!!\n");
		}
	}
	IntVector_clear(x);
	y.clear();
	// stable_sort
	for (i = 0; i < SORT_COUNT; i++) {
		buf[i] = rand();
		IntVector_push_back(x, buf[i]);
		y.push_back(buf[i]);
	}
	t = get_msec();
	IntVector_stable_sort(x, 0, IntVector_size(x), comp);
	printf("cstl: stable_sort[%d]: %g ms\n", SORT_COUNT, get_msec() - t);

	t = get_msec();
	stable_sort(y.begin(), y.end());
	printf("stl : stable_sort[%d]: %g ms\n", SORT_COUNT, get_msec() - t);
	for (i = 0; i < SORT_COUNT; i++) {
		if (y[i] != *IntVector_at(x, i)) {
			printf("!!!NG!!!\n");
		}
	}

	IntVector_delete(x);
	return 0;
}
