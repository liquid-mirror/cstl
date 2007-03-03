#include "test.h"

char buf[1024*1024];
Heap heap;

int main(void)
{
#ifdef MY_MALLOC
	Heap_init(&heap, buf, sizeof buf);
#endif

	VectorTest_run();
	DequeTest_run();
	ListTest_run();
	SetTest_run();
	MapTest_run();

#ifdef MY_MALLOC
	DUMP_MEMORY_LEAK(&heap, 0);
#endif
	return 0;
}
