#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif
#include <cstl/map.h>
#include <map>

//#define malloc(s) ::operator new(s)
//#define free(p) ::operator delete(p)

CSTL_MAP_INTERFACE(IntIntMap, int, int)
CSTL_MAP_IMPLEMENT(IntIntMap, int, int, CSTL_LESS)

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
	IntIntMap *x;
	map<int, int> y;
	IntIntMapIterator xpos;
	map<int, int>::iterator ypos;

	x = IntIntMap_new();

	printf("*** benchmark map<int, int> ***\n");

	// at
	t = get_msec();
	for (i = 0; i < COUNT; i++) {
		*IntIntMap_at(x, i) = COUNT - i;
	}
	printf("cstl: at[%d]: %g ms\n", COUNT, get_msec() - t);

	t = get_msec();
	for (i = 0; i < COUNT; i++) {
		y[i] = COUNT - i;
	}
	printf("stl : at[%d]: %g ms\n", COUNT, get_msec() - t);
	for (xpos = IntIntMap_begin(x), ypos = y.begin(); ypos != y.end(); xpos = IntIntMapIterator_next(xpos), ++ypos) {
		if (ypos->first != *IntIntMapIterator_key(xpos)) {
			printf("!!!NG!!!\n");
		}
		if (ypos->second != *IntIntMapIterator_value(xpos)) {
			printf("!!!NG!!!\n");
		}
	}

	// erase
	t = get_msec();
	for (xpos = IntIntMap_begin(x); xpos != IntIntMap_end(x);) {
		xpos = IntIntMap_erase(x, xpos);
	}
	printf("cstl: erase[%d]: %g ms\n", COUNT, get_msec() - t);

	t = get_msec();
	for (ypos = y.begin(); ypos != y.end();) {
		y.erase(ypos++);
	}
	printf("stl : erase[%d]: %g ms\n", COUNT, get_msec() - t);
	if (!y.empty() || y.size() != IntIntMap_size(x)) {
		printf("!!!NG!!!\n");
	}

	// insert
	t = get_msec();
	for (i = 0; i < COUNT; i++) {
		IntIntMap_insert(x, i, COUNT - i, NULL);
	}
	printf("cstl: insert[%d]: %g ms\n", COUNT, get_msec() - t);

	t = get_msec();
	for (i = 0; i < COUNT; i++) {
		y.insert(make_pair(i, COUNT - i));
	}
	printf("stl : insert[%d]: %g ms\n", COUNT, get_msec() - t);
	if (y.size() != IntIntMap_size(x)) {
		printf("!!!NG!!!\n");
	}
	for (xpos = IntIntMap_begin(x), ypos = y.begin(); ypos != y.end(); xpos = IntIntMapIterator_next(xpos), ++ypos) {
		if (ypos->first != *IntIntMapIterator_key(xpos)) {
			printf("!!!NG!!!\n");
		}
		if (ypos->second != *IntIntMapIterator_value(xpos)) {
			printf("!!!NG!!!\n");
		}
	}

	// erase key
	t = get_msec();
	for (i = 0; i < COUNT; i++) {
		IntIntMap_erase_key(x, i);
	}
	printf("cstl: erase key[%d]: %g ms\n", COUNT, get_msec() - t);

	t = get_msec();
	for (i = 0; i < COUNT; i++) {
		y.erase(i);
	}
	printf("stl : erase key[%d]: %g ms\n", COUNT, get_msec() - t);
	if (!y.empty() || y.size() != IntIntMap_size(x)) {
		printf("!!!NG!!!\n");
	}

	IntIntMap_delete(x);

	return 0;
}


