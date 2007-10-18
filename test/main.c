#include "test.h"

double buf[1024*1024/sizeof(double)];
Heap heap;

int main(void)
{
#ifdef MY_MALLOC
	Heap_init(&heap, buf, sizeof buf, sizeof buf[0]);
#endif

	VectorTest_run();
	RingTest_run();
	DequeTest_run();
	ListTest_run();
	SetTest_run();
	MapTest_run();

#ifdef MY_MALLOC
	HEAP_DUMP_LEAK(&heap, 0);
#endif
	return 0;
}
