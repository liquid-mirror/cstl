#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif
#include <cstl/vector.h>
//#include <cstl/algorithm.h>
#include <vector>
#include <algorithm>
#include <functional>


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
	size_t j;
	size_t size;
	double t;
	IntVector *x;
	IntVectorIterator xpos;
	IntVectorIterator end;
	vector<int> y;
	vector<int>::iterator ypos;

	x = IntVector_new_reserve(8);
	y.reserve(8);

	printf("*** benchmark vector<int> ***\n");
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
	for (i = 0; i < COUNT; i++) {
		if (y[i] != *cstl_at(x, i)) {
			printf("!!!NG!!!\n");
		}
	}

	// iterate
	t = get_msec();
#if 1
	end = cstl_end(x);
	for (xpos = cstl_begin(x); cstl_iter_ne(xpos, end); cstl_iter_incr(&xpos)) {
		volatile int tmp = *cstl_iter_data(xpos);
		i = tmp;
	}
#else
	size = cstl_size(x);
	for (j = 0; j < size; j++) {
		volatile int tmp = *cstl_at(x, i);
		i = tmp;
	}
#endif
	printf("cstl: iterate[%d]: %g ms\n", COUNT, get_msec() - t);

	t = get_msec();
	for (ypos = y.begin(); ypos != y.end(); ++ypos) {
		volatile int tmp = *ypos;
		i = tmp;
	}
	printf("stl : iterate[%d]: %g ms\n", COUNT, get_msec() - t);

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
	// insert front
	t = get_msec();
	for (i = 0; i < INSERT_COUNT; i++) {
		cstl_insert(x, cstl_begin(x), i, NULL);
	}
	printf("cstl: insert front[%d]: %g ms\n", INSERT_COUNT, get_msec() - t);

	t = get_msec();
	for (i = 0; i < INSERT_COUNT; i++) {
		y.insert(y.begin(), i);
	}
	printf("stl : insert front[%d]: %g ms\n", INSERT_COUNT, get_msec() - t);
	for (i = 0; i < INSERT_COUNT; i++) {
		if (y[i] != *cstl_at(x, i)) {
			printf("!!!NG!!!\n");
		}
	}
	// erase front
	t = get_msec();
	for (i = 0; i < INSERT_COUNT; i++) {
		cstl_erase(x, cstl_begin(x));
	}
	printf("cstl: erase front[%d]: %g ms\n", INSERT_COUNT, get_msec() - t);

	t = get_msec();
	for (i = 0; i < INSERT_COUNT; i++) {
		y.erase(y.begin());
	}
	printf("stl : erase front[%d]: %g ms\n", INSERT_COUNT, get_msec() - t);
	if (!y.empty() || y.size() != cstl_size(x)) {
		printf("!!!NG!!!\n");
	}
	// insert center
	t = get_msec();
	for (i = 0; i < INSERT_COUNT; i++) {
//        cstl_insert(x, cstl_size(x) / 2, i);
		xpos = cstl_begin(x);
		cstl_insert(x, cstl_iter_add(xpos, cstl_size(x) / 2), i, NULL);
//        cstl_insert(x, cstl_iter_add(cstl_begin(x), cstl_size(x) / 2), i, NULL);
	}
	printf("cstl: insert center[%d]: %g ms\n", INSERT_COUNT, get_msec() - t);

	t = get_msec();
	for (i = 0; i < INSERT_COUNT; i++) {
		y.insert(y.begin() + y.size() / 2, i);
	}
	printf("stl : insert center[%d]: %g ms\n", INSERT_COUNT, get_msec() - t);
	for (i = 0; i < INSERT_COUNT; i++) {
		if (y[i] != *cstl_at(x, i)) {
			printf("!!!NG!!!\n");
		}
	}
	// erase center
	t = get_msec();
	for (i = 0; i < INSERT_COUNT; i++) {
//        cstl_erase(x, cstl_size(x) / 2, 1);
		xpos = cstl_begin(x);
		cstl_erase(x, cstl_iter_add(xpos, cstl_size(x) / 2));
//        cstl_erase(x, cstl_iter_add(cstl_begin(x), cstl_size(x) / 2));
	}
	printf("cstl: erase center[%d]: %g ms\n", INSERT_COUNT, get_msec() - t);

	t = get_msec();
	for (i = 0; i < INSERT_COUNT; i++) {
		y.erase(y.begin() + y.size() / 2);
	}
	printf("stl : erase center[%d]: %g ms\n", INSERT_COUNT, get_msec() - t);
	if (!y.empty() || y.size() != cstl_size(x)) {
		printf("!!!NG!!!\n");
	}
	// insert back
	t = get_msec();
	for (i = 0; i < COUNT; i++) {
//        cstl_insert(x, cstl_size(x), i);
		cstl_insert(x, cstl_end(x), i, NULL);
	}
	printf("cstl: insert back[%d]: %g ms\n", COUNT, get_msec() - t);

	t = get_msec();
	for (i = 0; i < COUNT; i++) {
		y.insert(y.end(), i);
	}
	printf("stl : insert back[%d]: %g ms\n", COUNT, get_msec() - t);
	for (i = 0; i < COUNT; i++) {
		if (y[i] != *cstl_at(x, i)) {
			printf("!!!NG!!!\n");
		}
	}
	// erase back
	t = get_msec();
	for (i = 0; i < COUNT; i++) {
//        cstl_erase(x, cstl_size(x) - 1, 1);
		xpos = cstl_end(x);
		cstl_erase(x, cstl_iter_sub(xpos, 1));
//        cstl_erase(x, cstl_iter_sub(cstl_end(x), 1));
	}
	printf("cstl: erase back[%d]: %g ms\n", COUNT, get_msec() - t);

	t = get_msec();
	for (i = 0; i < COUNT; i++) {
		y.erase(y.end() - 1);
	}
	printf("stl : erase back[%d]: %g ms\n", COUNT, get_msec() - t);
	if (!y.empty() || y.size() != cstl_size(x)) {
		printf("!!!NG!!!\n");
	}


#if 0
	static int buf[SORT_COUNT];
	srand(time(0));
	// sort
	for (i = 0; i < SORT_COUNT; i++) {
		buf[i] = rand();
		cstl_push_back(x, buf[i]);
		y.push_back(buf[i]);
	}
	t = get_msec();
	cstl_algo_sort(x, 0, cstl_size(x), comp);
	printf("cstl: sort[%d]: %g ms\n", SORT_COUNT, get_msec() - t);

	t = get_msec();
	sort(y.begin(), y.end());
	printf("stl : sort[%d]: %g ms\n", SORT_COUNT, get_msec() - t);

	t = get_msec();
	qsort(buf, SORT_COUNT, sizeof(int), comp);
	printf("libc: qsort[%d]: %g ms\n", SORT_COUNT, get_msec() - t);

	for (i = 0; i < SORT_COUNT; i++) {
		if (y[i] != *cstl_at(x, i) || y[i] != buf[i]) {
			printf("!!!NG!!!\n");
		}
	}
	// sort 2
	t = get_msec();
	cstl_algo_sort(x, 0, cstl_size(x), comp);
	printf("cstl: sort2[%d]: %g ms\n", SORT_COUNT, get_msec() - t);

	t = get_msec();
	sort(y.begin(), y.end());
	printf("stl : sort2[%d]: %g ms\n", SORT_COUNT, get_msec() - t);

	t = get_msec();
	qsort(buf, SORT_COUNT, sizeof(int), comp);
	printf("libc: qsort2[%d]: %g ms\n", SORT_COUNT, get_msec() - t);

	for (i = 0; i < SORT_COUNT; i++) {
		if (y[i] != *cstl_at(x, i) || y[i] != buf[i]) {
			printf("!!!NG!!!\n");
		}
	}
	// sort 3
	t = get_msec();
	cstl_algo_sort(x, 0, cstl_size(x), greater_comp);
	printf("cstl: sort3[%d]: %g ms\n", SORT_COUNT, get_msec() - t);

	t = get_msec();
	sort(y.begin(), y.end(), std::greater<int>());
	printf("stl : sort3[%d]: %g ms\n", SORT_COUNT, get_msec() - t);

	t = get_msec();
	qsort(buf, SORT_COUNT, sizeof(int), greater_comp);
	printf("libc: qsort3[%d]: %g ms\n", SORT_COUNT, get_msec() - t);

	for (i = 0; i < SORT_COUNT; i++) {
		if (y[i] != *cstl_at(x, i) || y[i] != buf[i]) {
			printf("!!!NG!!!\n");
		}
	}

	cstl_clear(x);
	y.clear();
	// stable_sort
	for (i = 0; i < SORT_COUNT; i++) {
		buf[i] = rand();
		cstl_push_back(x, buf[i]);
		y.push_back(buf[i]);
	}
	t = get_msec();
	IntVector_stable_sort(x, 0, cstl_size(x), comp);
	printf("cstl: stable_sort[%d]: %g ms\n", SORT_COUNT, get_msec() - t);

	t = get_msec();
	stable_sort(y.begin(), y.end());
	printf("stl : stable_sort[%d]: %g ms\n", SORT_COUNT, get_msec() - t);
	for (i = 0; i < SORT_COUNT; i++) {
		if (y[i] != *cstl_at(x, i)) {
			printf("!!!NG!!!\n");
		}
	}
	// stable_sort 2
	t = get_msec();
	IntVector_stable_sort(x, 0, cstl_size(x), comp);
	printf("cstl: stable_sort2[%d]: %g ms\n", SORT_COUNT, get_msec() - t);

	t = get_msec();
	stable_sort(y.begin(), y.end());
	printf("stl : stable_sort2[%d]: %g ms\n", SORT_COUNT, get_msec() - t);
	for (i = 0; i < SORT_COUNT; i++) {
		if (y[i] != *cstl_at(x, i)) {
			printf("!!!NG!!!\n");
		}
	}
	// stable_sort 3
	t = get_msec();
	IntVector_stable_sort(x, 0, cstl_size(x), greater_comp);
	printf("cstl: stable_sort3[%d]: %g ms\n", SORT_COUNT, get_msec() - t);

	t = get_msec();
	stable_sort(y.begin(), y.end(), greater<int>());
	printf("stl : stable_sort3[%d]: %g ms\n", SORT_COUNT, get_msec() - t);
	for (i = 0; i < SORT_COUNT; i++) {
		if (y[i] != *cstl_at(x, i)) {
			printf("!!!NG!!!\n");
		}
	}

	cstl_clear(x);
	y.clear();

	// partial_sort
	for (i = 0; i < SORT_COUNT; i++) {
		buf[i] = rand();
		cstl_push_back(x, buf[i]);
		y.push_back(buf[i]);
	}
	t = get_msec();
	IntVector_partial_sort(x, 0, cstl_size(x), cstl_size(x), comp);
	printf("cstl: partial_sort[%d]: %g ms\n", SORT_COUNT, get_msec() - t);

	t = get_msec();
	partial_sort(y.begin(), y.end(), y.end());
	printf("stl : partial_sort[%d]: %g ms\n", SORT_COUNT, get_msec() - t);

	t = get_msec();
	qsort(buf, SORT_COUNT, sizeof(int), comp);
	printf("libc: qsort[%d]: %g ms\n", SORT_COUNT, get_msec() - t);

	for (i = 0; i < SORT_COUNT; i++) {
		if (y[i] != *cstl_at(x, i) || y[i] != buf[i]) {
			printf("!!!NG!!!\n");
		}
	}
	// partial_sort 2
	t = get_msec();
	IntVector_partial_sort(x, 0, cstl_size(x), cstl_size(x), comp);
	printf("cstl: partial_sort2[%d]: %g ms\n", SORT_COUNT, get_msec() - t);

	t = get_msec();
	partial_sort(y.begin(), y.end(), y.end());
	printf("stl : partial_sort2[%d]: %g ms\n", SORT_COUNT, get_msec() - t);

	t = get_msec();
	qsort(buf, SORT_COUNT, sizeof(int), comp);
	printf("libc: qsort2[%d]: %g ms\n", SORT_COUNT, get_msec() - t);

	for (i = 0; i < SORT_COUNT; i++) {
		if (y[i] != *cstl_at(x, i) || y[i] != buf[i]) {
			printf("!!!NG!!!\n");
		}
	}
	// partial_sort 3
	t = get_msec();
	IntVector_partial_sort(x, 0, cstl_size(x), cstl_size(x), greater_comp);
	printf("cstl: partial_sort3[%d]: %g ms\n", SORT_COUNT, get_msec() - t);

	t = get_msec();
	partial_sort(y.begin(), y.end(), y.end(), std::greater<int>());
	printf("stl : partial_sort3[%d]: %g ms\n", SORT_COUNT, get_msec() - t);

	t = get_msec();
	qsort(buf, SORT_COUNT, sizeof(int), greater_comp);
	printf("libc: qsort3[%d]: %g ms\n", SORT_COUNT, get_msec() - t);

	for (i = 0; i < SORT_COUNT; i++) {
		if (y[i] != *cstl_at(x, i) || y[i] != buf[i]) {
			printf("!!!NG!!!\n");
		}
	}
#endif

	cstl_delete(x);
	return 0;
}
