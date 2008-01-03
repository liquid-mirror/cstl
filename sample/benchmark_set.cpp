#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif
#include <cstl/set.h>
#include <set>

//#define malloc(s) ::operator new(s)
//#define free(p) ::operator delete(p)

CSTL_SET_INTERFACE(IntSet, int)
CSTL_SET_IMPLEMENT(IntSet, int, CSTL_LESS)

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


int main(void)
{
	int i;
	double t;
	IntSet *x;
	set<int> y;
	IntSetIterator xpos;
	set<int>::iterator ypos;

	x = IntSet_new();

	printf("*** benchmark set<int> ***\n");

	// insert
	t = get_msec();
	for (i = 0; i < COUNT; i++) {
		IntSet_insert(x, i, NULL);
	}
	printf("cstl: insert[%d]: %g ms\n", COUNT, get_msec() - t);

	t = get_msec();
	for (i = 0; i < COUNT; i++) {
		y.insert(i);
	}
	printf("stl : insert[%d]: %g ms\n", COUNT, get_msec() - t);
	if (y.size() != IntSet_size(x)) {
		printf("!!!NG!!!\n");
	}
	for (xpos = IntSet_begin(x), ypos = y.begin(); ypos != y.end(); xpos = IntSet_next(xpos), ++ypos) {
		if (*ypos != IntSet_key(xpos)) {
			printf("!!!NG!!!\n");
		}
	}

	// erase
	t = get_msec();
	for (i = 0; i < COUNT; i++) {
		IntSet_erase_key(x, i);
	}
	printf("cstl: erase key[%d]: %g ms\n", COUNT, get_msec() - t);

	t = get_msec();
	for (i = 0; i < COUNT; i++) {
		y.erase(i);
	}
	printf("stl : erase key[%d]: %g ms\n", COUNT, get_msec() - t);
	if (!y.empty() || y.size() != IntSet_size(x)) {
		printf("!!!NG!!!\n");
	}

	IntSet_delete(x);

	return 0;
}

